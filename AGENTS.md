# Arbeitsanweisungen für Clonk

Der Workspace dient C4Script-Mods, Szenarien und Werkzeugen für **LegacyClonk**. Ausgangspunkt sind das Clonk-SDK und der veröffentlichte LegacyClonk-Quellcode. Reverse Engineering gehört nicht zum Projekt.

## Einstieg und Pfade

- Zuerst [Documentation/README.md](Documentation/README.md) und die zum Vorhaben passenden Seiten lesen.
- Spielinstallation: `C:\Users\Serpens66\Desktop\Spiele\LegacyClonk`. Den Zielbuild vor Spieltests aus dem aktuellen Start in `Clonk.log` bestimmen.
- Leseklon: `D:\CDesktopLink\Unterlagen\Mods\Clonk\LegacyClonk`. Engine und `c4group` stammen aus dem [öffentlichen Upstream](https://github.com/legacyclonk/LegacyClonk).
- [Entwicklerdokumentation](https://crdocs.clonkspot.org/de/sdk/index.html), [Download und Projektseite](https://fulgen.itch.io/legacyclonk), [LegacyClonk-Forum](https://forum.clonkspot.org/c/projekte/legacyclonk/30).
- `alte Projekte/` ist das historische Archiv. Analyse und Tests erfolgen mit Arbeitskopien unter `.work/`. Eigene neue Mods und Werkzeuge erhalten beim ersten Bedarf getrennte Projektordner unter `Mods/` bzw. `Tools/`.

## Entwicklung und Nachweise

- Zuerst die SDK-Signatur, den Aufrufkontext und die benötigten Definitionen prüfen. Bei versionsabhängigen Aussagen im passenden Release-Quellstand nachsehen. Clonk Rage, LegacyClonk und OpenClonk als getrennte Referenzen behandeln.
- Bestehende Änderungen, Dateikodierung und Strict-Stufe erhalten. Eine Änderung der Strict-Stufe als eigene Kompatibilitätsänderung prüfen. Neue Workspace-Dokumente als UTF-8 ohne BOM mit CRLF schreiben.
- Nur die für das Vorhaben relevanten Szenarien und Pakete testen. Für `c4group` und lokale Spieltests Arbeitskopien verwenden. Die Spielinstallation und das Archiv dienen als Quellen.
- Neue belastbare Erkenntnisse in `Documentation/` eintragen. Quelle, Build oder Commit und Prüfart angeben. SDK-Aussagen, Quellcodefunde, Altprojektnotizen, Werkzeugtests und Spieltests getrennt kennzeichnen.

## Git und Upstream

- Workspace-Dateien und Engine-Quellcode gehören in getrennte Git-Repositories. `LegacyClonk/`, `alte Projekte/` und `.work/` sind im Workspace ignoriert.
- Im Upstream-Klon lesen und Versionen vergleichen. Für konkrete Engine-Änderungen eine getrennte Arbeitskopie unter `.work/` vom passenden Upstream-Stand anlegen und dort die Vorgaben des Engine-Repos befolgen.
- Commits, Pushes, Forks und GitHub-PRs nur auf Nutzerauftrag. Maßgeblich für Engine-Beiträge ist der GitHub-Upstream.
