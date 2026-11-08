"""Build the LegacyClonk C4Script catalogue from the public SDK and Git objects."""

import argparse
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
import hashlib
import html
from html.parser import HTMLParser
import json
from pathlib import Path
import re
import subprocess
from urllib.error import URLError
from urllib.parse import urldefrag, urljoin, urlsplit
from urllib.request import Request, urlopen

INDEX = "https://crdocs.clonkspot.org/de/content.html"
UPSTREAM = "https://github.com/legacyclonk/LegacyClonk"
ROOT = Path(__file__).resolve().parents[2]
SCRIPT = "src/C4Script.cpp"
BUILTIN = re.compile(r'^\s*AddFunc\(pEngine,\s*(?:"([^"]+)"|(PSF_\w+)),\s*(\w+)(?:,\s*(true|false))?\);', re.M)
TEMPLATE_BUILTIN = re.compile(
    r'^\s*new (C4Aul(?:DefCastFunc|EngineFuncParArray))<([^>]+)>\s*\{pEngine,\s*"([^"]+)"([^}\n]*)\};',
    re.M,
)
CONSTANT = re.compile(r'^\s*\{\s*"([^"]+)",\s*(C4V_\w+),\s*(.*?)\s*\},?', re.M)
SYSTEM_FUNC = re.compile(r'^\s*global\s+func\s+(\w+)\s*\(', re.M)


def plain(markup):
    return " ".join(html.unescape(re.sub(r"<[^>]+>", "", markup)).split())


class IndexLinks(HTMLParser):
    def __init__(self):
        super().__init__()
        self.entries = {}

    def handle_starttag(self, tag, attrs):
        href = dict(attrs).get("href") or ""
        if tag != "a" or not re.fullmatch(r"sdk/script/fn/[\w.-]+\.html(?:#[\w.-]+)?", href):
            return
        url = urljoin(INDEX, href)
        page, anchor = urldefrag(url)
        name = anchor or Path(urlsplit(page).path).stem
        if self.entries.setdefault(name, url) != url:
            raise ValueError(f"Mehrere SDK-Links für {name}")


def sdk_html(url, cache, offline):
    name = hashlib.sha256(url.encode()).hexdigest()
    cached = cache / f"{name}.html"
    if cached.exists():
        return cached.read_text(encoding="utf-8")
    if offline:
        raise FileNotFoundError(url)
    for attempt in range(2):
        try:
            request = Request(url, headers={"User-Agent": "ClonkWorkspace-SDK-Catalogue/1.0"})
            with urlopen(request, timeout=15) as response:
                result = response.read().decode(response.headers.get_content_charset() or "utf-8")
            result = result.replace("\r\n", "\n").replace("\r", "\n")
            cached.write_text(result, encoding="utf-8", newline="\n")
            return result
        except (URLError, TimeoutError) as error:
            if attempt == 1:
                raise error


def sdk_entry(url, cache, offline):
    try:
        page = sdk_html(urldefrag(url)[0], cache, offline)
    except (OSError, UnicodeError, ValueError):
        return {"sdk_url": url, "sdk_status": "Seite nicht verfügbar"}

    def metadata(label):
        match = re.search(r"<b>\s*" + label + r":\s*</b>(.*?)(?=<br\b|</div>)", page, re.S | re.I)
        return plain(match[1]) if match else None

    signatures = [
        re.sub(r"\s+\(", "(", plain(match))
        for match in re.findall(r'<div class="[^"]*\bfnsyntax\b[^"]*">(.*?)</div>', page, re.S)
    ]
    return {
        "sdk_url": url,
        "sdk_status": "Signatur erfasst" if signatures else "Ohne Syntaxblock",
        "sdk_category": metadata("Kategorie"),
        "sdk_since": metadata("Ab Engineversion"),
        "sdk_signatures": signatures,
        "sdk_constant": "Engine-Konstante definiert" in page,
    }


def git(repo, *args):
    return subprocess.check_output(["git", "-C", str(repo), *args]).decode("utf-8", errors="replace")


def at(commit, path, line):
    return f"{UPSTREAM}/blob/{commit}/{path}#L{line}"


def line_at(text, offset):
    return text.count("\n", 0, offset) + 1


def match_line(text, match):
    return line_at(text, match.start()) + match[0].count("\n")


def engine_entries(repo, commit):
    source = git(repo, "show", f"{commit}:{SCRIPT}")
    header = git(repo, "show", f"{commit}:src/C4Script.h")
    effects_path = "src/C4Effect.cpp"
    effects = git(repo, "show", f"{commit}:{effects_path}")
    callback_macros = dict(re.findall(r'^#define\s+(PSF_\w+)\s+"([^"]+)"', header, re.M))

    def implementation(symbol):
        for path, code in ((SCRIPT, source), (effects_path, effects)):
            definition = re.search(r"^[ \t]*[\w *:&<>]+[ *&]" + re.escape(symbol) + r"\s*\(", code, re.M)
            if definition:
                return at(commit, path, line_at(code, definition.start()))
        raise ValueError(f"Keine Implementierung für {symbol} in {commit}")

    entries = {}
    for match in BUILTIN.finditer(source):
        literal, macro, symbol, public = match.groups()
        name = literal if literal is not None else callback_macros[macro]
        entries[name] = {
            "kind": "Engine-Funktion",
            "registration": at(commit, SCRIPT, match_line(source, match)),
            "symbol": symbol,
            "public": public != "false",
            "implementation": implementation(symbol),
        }
    if len(entries) != len(re.findall(r"\bAddFunc\(pEngine,", source)):
        raise ValueError(f"Unbekannte oder doppelte Funktionsregistrierung in {commit}")

    template_entries = list(TEMPLATE_BUILTIN.finditer(source))
    for match in template_entries:
        helper, types, name, arguments = match.groups()
        if name in entries:
            raise ValueError(f"Doppelte Registrierung für {name} in {commit}")
        args = arguments.removeprefix(",").split(",")
        symbol = args[0].strip() if arguments else helper
        entries[name] = {
            "kind": "Engine-Funktion",
            "registration": at(commit, SCRIPT, match_line(source, match)),
            "symbol": symbol,
            "template": f"{helper}<{types}>",
            "arguments": [arg.strip() for arg in args] if arguments else [],
            "implementation": implementation(symbol) if arguments else at(
                commit, SCRIPT, line_at(source, source.index(f"class {helper} "))
            ),
        }
    if len(template_entries) != len(re.findall(r"new C4Aul\w+<[^\n]*?\{pEngine,", source)):
        raise ValueError(f"Unbekannte Template-Registrierung in {commit}")

    const_map = re.search(r"C4ScriptConstMap\[\]\s*=\s*\{(.*?)^\};", source, re.S | re.M)
    if not const_map:
        raise ValueError(f"Konstantentabelle fehlt in {commit}")
    for match in CONSTANT.finditer(const_map[1]):
        name, value_type, expression = match.groups()
        if name in entries:
            raise ValueError(f"Doppelte Registrierung für {name} in {commit}")
        entries[name] = {
            "kind": "Engine-Konstante",
            "registration": at(commit, SCRIPT, line_at(source, const_map.start(1) + match.start()) + match[0].count("\n")),
            "symbol": expression,
            "type": value_type,
        }
    if sum(row["kind"] == "Engine-Konstante" for row in entries.values()) != const_map[1].count('{ "'):
        raise ValueError(f"Unbekannte Form in der Konstantentabelle in {commit}")
    return entries


def system_entries(repo, commit):
    entries = {}
    for path in git(repo, "ls-tree", "-r", "--name-only", commit, "--", "planet/System.c4g").splitlines():
        if path.endswith(".c"):
            source = git(repo, "show", f"{commit}:{path}")
            for match in SYSTEM_FUNC.finditer(source):
                entries[match[1]] = at(commit, path, match_line(source, match))
    return entries


def other_evidence(repo, commit, sdk, registered):
    header = git(repo, "show", f"{commit}:src/C4Script.h")
    parser = git(repo, "show", f"{commit}:src/C4AulParse.cpp")
    for name, record in sdk.items():
        if name in registered:
            continue
        if "callback" in (record.get("sdk_category") or "").casefold() or name == "InputCallback":
            record["kind"] = "Callback"
            macro = re.search(r'^#define\s+(PSF_\w+)\s+"~?' + re.escape(name) + r'"', header, re.M)
            if macro:
                hits = git(repo, "grep", "-n", "-w", "-e", macro[1], commit, "--", "src").splitlines()
                record["implementation"] = next(
                    (at(commit, path, number) for _, path, number, code in (line.split(":", 3) for line in hits)
                     if re.search(r"\b(?:Call|CallEx|GameCall|ObjectCall)\s*\(", code)),
                    at(commit, "src/C4Script.h", line_at(header, macro.start())),
                )
        elif name in {"inherited", "_inherited", "if", "for", "while", "return", "Par"}:
            keyword = re.search(r'^#define\s+C4AUL_\w+\s+"' + re.escape(name) + r'"', parser, re.M)
            if keyword:
                record["kind"] = "Sprachsyntax"
                record["implementation"] = at(commit, "src/C4AulParse.cpp", line_at(parser, keyword.start()))
        elif record.get("sdk_constant"):
            record["kind"] = "SDK-Konstante"


def catalogue(repo, cache, offline, release_ref, master_ref):
    index = IndexLinks()
    index_html = sdk_html(INDEX, cache, offline)
    index.feed(index_html)
    if not index.entries:
        raise ValueError("SDK-Index enthält keine erkennbaren API-Links")
    with ThreadPoolExecutor(max_workers=6) as pool:
        sdk = dict(zip(index.entries, pool.map(
            lambda url: sdk_entry(url, cache, offline), index.entries.values()
        ), strict=True))

    release = git(repo, "rev-parse", "--verify", f"{release_ref}^{{commit}}").strip()
    master = git(repo, "rev-parse", "--verify", f"{master_ref}^{{commit}}").strip()
    base = engine_entries(repo, release)
    tip = engine_entries(repo, master)
    system = system_entries(repo, release)
    other_evidence(repo, release, sdk, base.keys() | system.keys())

    names = sorted(sdk.keys() | base.keys(), key=lambda name: (name.casefold(), name))
    entries = {}
    for name in names:
        entry = {"kind": "Offen", "sdk_status": "Kein Indexeintrag"} | sdk.get(name, {})
        if name in system:
            entry |= {"kind": "Systemskript", "system_script": system[name]}
        entries[name] = entry | base.get(name, {})

    def registration_contract(record):
        return {key: value for key, value in record.items() if key not in {"registration", "implementation"}}

    return {
        "sdk_index": INDEX,
        "sdk_index_sha256": hashlib.sha256(index_html.encode()).hexdigest(),
        "release_commit": release,
        "master_commit": master,
        "counts": {
            "sdk_links": len(sdk),
            "sdk_status": dict(sorted(Counter(record["sdk_status"] for record in sdk.values()).items())),
            "release_registrations": dict(sorted(Counter(record["kind"] for record in base.values()).items())),
            "release_registrations_without_sdk": len(base.keys() - sdk.keys()),
        },
        "master_registrations": {
            "added": {name: tip[name] for name in sorted(tip.keys() - base.keys())},
            "removed": {name: base[name] for name in sorted(base.keys() - tip.keys())},
            "changed": {
                name: {"release": base[name], "master": tip[name]}
                for name in sorted(base.keys() & tip.keys())
                if registration_contract(base[name]) != registration_contract(tip[name])
            },
        },
        "entries": entries,
    }


def markdown(data):
    counts = data["counts"]
    change = data["master_registrations"]
    lines = [
        "# C4Script-Katalog für LegacyClonk",
        "",
        f"Basis ist [der Release-Quellstand]({UPSTREAM}/commit/{data['release_commit']}).",
        f"{counts['sdk_links']} Einträge aus dem [SDK-Inhaltsverzeichnis]({INDEX}).",
        f"{counts['release_registrations_without_sdk']} Engine-Registrierungen haben keinen SDK-Indexeintrag.",
        "Der Abgleich ist statisch. Aufrufkontext, Strict-Modus und Verhalten im Spiel werden pro Vorhaben geprüft.",
        "",
        f"Im [geprüften master-Stand]({UPSTREAM}/commit/{data['master_commit']}) sind "
        f"{len(change['added'])} Registrierungen hinzugekommen, {len(change['removed'])} entfallen "
        f"und {len(change['changed'])} geändert. Der Vergleich umfasst Namen, Symbole, Sichtbarkeit "
        "und Konstantenausdrücke.",
    ]
    for field, label in (("added", "Hinzugekommen"), ("removed", "Entfallen"), ("changed", "Geändert")):
        if change[field]:
            lines.extend(("", f"{label} – " + ", ".join(change[field])))
    lines.extend((
        "", "| Name | Herkunft | SDK-Signatur | Kategorie / seit | Belege |",
        "| --- | --- | --- | --- | --- |",
    ))
    for name, row in data["entries"].items():
        signature = " / ".join(f"<code>{html.escape(s).replace('|', '&#124;')}</code>" for s in row.get("sdk_signatures", []))
        metadata = " / ".join(row[key] for key in ("sdk_category", "sdk_since") if row.get(key)).replace("|", "&#124;")
        links = [
            f"[{label}]({row[key]})"
            for key, label in (("sdk_url", "SDK"), ("registration", "Registrierung"),
                               ("implementation", "Quellcode"), ("system_script", "Systemskript"))
            if row.get(key)
        ]
        lines.append(f"| {name} | {row['kind']} | {signature or row['sdk_status']} | {metadata} | {', '.join(links)} |")
    return "\n".join(lines) + "\n"


def main():
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("--repo", type=Path, default=ROOT / "LegacyClonk")
    cli.add_argument("--cache", type=Path, default=ROOT / ".work" / "sdk-cache")
    cli.add_argument("--output", type=Path, default=Path(__file__).parent)
    cli.add_argument("--release", default="v365")
    cli.add_argument("--master", default="origin/master")
    cli.add_argument("--offline", action="store_true")
    args = cli.parse_args()
    args.cache.mkdir(parents=True, exist_ok=True)
    data = catalogue(args.repo, args.cache, args.offline, args.release, args.master)
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / "C4Script_API.json").write_text(
        json.dumps(data, ensure_ascii=False, indent=2) + "\n", encoding="utf-8", newline="\r\n"
    )
    (args.output / "C4Script_API.md").write_text(markdown(data), encoding="utf-8", newline="\r\n")
    print(json.dumps(data["counts"], ensure_ascii=False))


if __name__ == "__main__":
    main()
