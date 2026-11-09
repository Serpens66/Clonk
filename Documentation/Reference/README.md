# C4Script-Referenz aktualisieren

Der [Markdown-Katalog](C4Script_API.md) dient zum Nachschlagen einzelner Schnittstellen. Syntax und Aufrufkontext stehen in der [Sprachreferenz](../C4Script.md), Änderungen für Strict 3 im [Migrationsleitfaden](../Strict3Migration.md). [Die JSON-Datei](C4Script_API.json) enthält dieselben Einträge mit getrennten Metadatenfeldern und den Details der Engine-Registrierungen. Die 528 API-Links des deutschen SDK-Index sind nach Namen zusammengeführt. Die Registrierungen für `v365` umfassen 461 Engine-Funktionen und 292 Konstanten. Davon haben 282 Registrierungen keinen SDK-Indexeintrag. Außerdem wurden 47 SDK-Einträge in Systemskripten, drei als Callbacks und sieben im Parser zugeordnet.

`public: false` im JSON gibt den Sichtbarkeitsparameter der `AddFunc`-Registrierung wieder. Das Feld `sdk_signatures` zeigt die historische Schreibweise der SDK-Seite, einschließlich der dort als Funktionssyntax dargestellten Konstanten. Für einen konkreten Aufruf zählen außerdem die Zielversion, der Kontext und die aufgerufene Implementierung. Die Belege im Katalog sind statisch geprüft.

## Aufruf

Voraussetzungen sind Git, Python 3.10 oder neuer und der lokale Upstream-Klon mit Tag `v365`. Der Generator verwendet die Python-Standardbibliothek und liest Git-Objekte direkt aus dem Klon. Unter Windows ist Python 3.12 hier verfügbar.

```powershell
$pythonExe = 'D:\CDesktopLink\Portable\Python\WinPy64\python\python.exe'
& $pythonExe Documentation/Reference/generate.py
& $pythonExe Documentation/Reference/generate.py --offline --output .work/catalogue-check
```

Der erste Aufruf lädt die öffentlich verlinkten SDK-Seiten und legt den HTML-Cache unter `.work/sdk-cache/` an. Bei einem erneuten Lauf werden vorhandene Seiten aus diesem Cache gelesen. Für einen frischen SDK-Snapshot `--cache` auf einen neuen leeren Ordner unter `.work/` setzen. Fehlende Seiten erscheinen mit Link und Status in den Ausgaben. Fehlende Git-Quellen oder unbekannte Formen der Engine-Registrierung brechen die Erzeugung ab.

Standardmäßig werden `v365` und `origin/master` verglichen. Für eine Reproduktion mit den ursprünglichen Git-Objekten `--release 31b2cde5d1d68a86765c68c8252184ca38469c0d --master 89e5dae940cb1bdab238e0c42bcd0719ec583d0d` und denselben SDK-Cache angeben. Beim Aktualisieren die aufgelösten Commits und den Katalogdiff prüfen.
