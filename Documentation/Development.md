# Entwicklung und Tests mit LegacyClonk

## C4Script und Spieldaten

1. Für ein einzelnes Szenario mit einer offenen `.c4s`-Gruppe beginnen. `Scenario.txt` beschreibt das Szenario, `Script.c` enthält seine Logik. Die benötigten Definitionspakete beim Start erfassen. [Szenariodaten](https://crdocs.clonkspot.org/de/sdk/scenario/index.html), [Szenarioskripte](https://crdocs.clonkspot.org/de/sdk/scenario/script.html)
2. Eigenständige Objektdefinitionen in `.c4d`-Gruppen führen. `DefCore.txt` enthält unter anderem die Objekt-ID, `ActMap.txt` die Aktionen und `Script.c` die Skriptlogik. Für eine gezielte Erweiterung einer vorhandenen Definition kommt ein `#appendto`-Skript infrage. Die Ziel-ID und die tatsächlich geladenen Pakete prüfen. [Objektdefinitionen](https://crdocs.clonkspot.org/de/sdk/definition/index.html), [Objektskripte](https://crdocs.clonkspot.org/de/sdk/definition/script.html), [#appendto](https://crdocs.clonkspot.org/de/sdk/script/AppendTo.html)
3. Bei einem API-Aufruf zuerst den [Katalog](Reference/C4Script_API.md) und die verlinkte SDK-Seite lesen. Beispielsweise dokumentiert `CreateObject` absolute Landschaftskoordinaten im globalen Aufruf und Offsets zum Objekt im lokalen Aufruf. Bei Suchfunktionen, Effekten und Callbacks auch den Objektkontext und die Rückgabewerte prüfen. [C4Script-Grundlagen](https://crdocs.clonkspot.org/de/sdk/script/index.html), [CreateObject](https://crdocs.clonkspot.org/de/sdk/script/fn/CreateObject.html), [Effekte](https://crdocs.clonkspot.org/de/sdk/script/Effects.html)
4. Bei neuem Code, der ausdrücklich auf LegacyClonk zielt, `#strict 3` erwägen. Bei Altcode zuerst den ursprünglichen Strict-Modus beibehalten. Gemischte Strict-Stufen können sich bei `nil`-Konvertierungen und Zugriffen unterschiedlich verhalten. Syntax und APIs anhand von LegacyClonk prüfen, da andere Clonk-Engines eigene Varianten von C4Script haben. [Ältere SDK-Strict-Referenz](https://crdocs.clonkspot.org/de/sdk/script/Strict.html), [LegacyClonk-Strict-3-Erläuterung](https://forum.clonkspot.org/t/modernes-scripting-in-legacyclonk-mit-strict-3/1462)

**Quellcodeprüfung für Build [365] (Tag `v365`).** `#strict 3` wird am Anfang der jeweiligen Skriptdatei gesetzt. Ohne Direktive startet das Skript im Modus `NONSTRICT`. `#strict` ohne Zahl wählt Stufe 1. Der Parser akzeptiert als explizite Zahlen nur 2 und 3. [Initialisierung](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Aul.cpp#L182-L194), [Parser](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1491-L1506)

Syntax, Dateistruktur und Aufrufkontext stehen in der [Sprachreferenz](C4Script.md). Für die Änderung bestehender Strict-2-Dateien den [Migrationsleitfaden](Strict3Migration.md) verwenden.

## Pakete und Werkzeuge

`.c4s` steht für Szenarien, `.c4d` für Objektdefinitionen oder Definitionspakete, `.c4f` für Szenarioordner und `.c4g` für weitere Gruppen wie Systemdaten. Die Engine kann gepackte und offene Gruppen verarbeiten. Offene Gruppen erleichtern das Bearbeiten und Versionieren. [SDK-Spieldaten](https://crdocs.clonkspot.org/de/sdk/files.html)

Das installierte `c4group.exe` zeigt seine Hilfe beim Aufruf ohne Argumente. In PowerShell Pfade als getrennte Argumente übergeben. `-p` und `-u` ersetzen die angegebene Gruppe am selben Pfad. Bei verschachtelten Archivgruppen den Vorgang auf der Kopie je Ebene durchführen. [SDK-Kommandozeile](https://crdocs.clonkspot.org/de/sdk/cmdline.html), [CLI-Quellcode](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/c4group_ng.cpp)

```powershell
$c4groupExe = 'C:\Users\Serpens66\Desktop\Spiele\LegacyClonk\c4group.exe'
# Probe.c4d ist hier eine offene Testgruppe unter .work.
$probeGroup = '.work\Probe.c4d'
& $c4groupExe $probeGroup -l
& $c4groupExe $probeGroup -p
& $c4groupExe $probeGroup -l
& $c4groupExe $probeGroup -u
```

**Werkzeugtest vom 8. November 2026:** `Probe.c4d` mit `Names.txt` wurde als offener Ordner und gepackte Datei aufgelistet. `-p` erzeugte die Gruppendatei, `-u` stellte den Ordner wieder her. SHA-256 von `Names.txt` stimmte überein (`5CD1B83547C1328E83A80FB343AF9C43E558D3847EF084D47809084CE0DAEDA6`). Das Ergebnis belegt den Durchlauf des Archivwerkzeugs.

### Eigene Pakete im Spiel verfügbar machen

Grundlage ist eine Quellcodeprüfung für LegacyClonk [365] am Commit `31b2cde5d1d68a86765c68c8252184ca38469c0d`.

- Die Szenarioauswahl beginnt im Verzeichnis der Engine. Dort werden `.c4s` und `.c4f` angezeigt. Auch gewöhnliche Unterordner ohne Dateiendung erscheinen, wenn sie Szenarien oder Szenarioordner enthalten. Der Windows-Standardpfad `%APPDATA%\LegacyClonk` dient als Benutzerpfad. Es gibt in der Szenarioauswahl keinen zusätzlichen Suchpfad für Benutzerpakete. [Szenarioauswahl](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4StartupScenSelDlg.cpp#L1445-L1454), [Pfadkonfiguration](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Config.cpp#L72-L83)
- Gemeinsam nutzbare `.c4d`-Pakete können neben der Engine liegen. Ein Szenario kann die benötigten Pakete über `Definitions` im Abschnitt `[Definitions]` seiner `Scenario.txt` vorgeben. Wenn die Szenarioauswahl die Option „Definitionen wählen“ anbietet, lassen sich beim Start weitere Pakete hinzufügen. Der Auswahldialog verwendet bei leerem `General.DefinitionPath` den Engine-Ordner. [SDK zu Scenario.txt](https://crdocs.clonkspot.org/de/sdk/scenario/scenario.html), [Startlogik](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4StartupScenSelDlg.cpp#L1649-L1674), [Definitionsauswahl](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4FileSelDlg.cpp#L389-L399)
- Ein `.c4d` innerhalb eines `.c4s` wird beim Start dieses Szenarios lokal geladen. Ein `.c4d` in einem übergeordneten `.c4f` steht den darin liegenden Szenarien zur Verfügung. Damit lassen sich Szenarien und ihre gemeinsamen Definitionen in einer `.c4f` zusammen weitergeben. [SDK zu Szenariodaten](https://crdocs.clonkspot.org/de/sdk/scenario/index.html), [Laden der Definitionen](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Game.cpp#L81-L102), [Suche in übergeordneten Szenarioordnern](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Game.cpp#L3976-L4007)

### Projekte getrennt ablegen

Der Szenariopfad kann beim Start von `clonk.exe` absolut übergeben werden. So kann eine Arbeitskopie außerhalb des Spielordners liegen. Externe Objektpakete, die in `Scenario.txt` nur mit einem Namen wie `Objects.c4d` referenziert werden, werden vom Arbeitsverzeichnis der Engine aus geöffnet. Für zusammengehörende eigene Szenarien und Definitionen eignet sich ein gemeinsamer `.c4f`-Ordner. `General.DefinitionPath` betrifft Objektdefinitionen und ändert die Wurzel der Szenarioauswahl nicht. [Kommandozeile](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Game.cpp#L3114-L3138), [Arbeitsverzeichnis](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Config.cpp#L614-L650), [Laden deklarierter Definitionen](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4GameParameters.cpp#L192-L210)

**Spieltest vom 9. November 2026.** LegacyClonk 4.9.11.2 [365] win64 startete unter `.work/paketpfade-0328dabf/` aus `Engine/` mit dem absoluten Pfad zu `Projekte/Pfadprobe.c4s`. `Direkt.log` meldet 345 Definitionen aus `Objects.c4d` im Engine-Verzeichnis, die Ausgabe `PFADPROBE_INITIALIZE` und den erfolgreichen Spielstart. Das Testszenario lag außerhalb des Engine-Verzeichnisses und deklarierte `Objects.c4d` per Dateiname.

**Archivierte Fremdkarten unverändert laden.** Ein NTFS-Hardlink mit dem erwarteten Paketnamen im Engine-Ordner kann auf ein gepacktes Paket unter `custom/` auf demselben Laufwerk zeigen. LegacyClonk öffnet diesen Namen wie eine normale Datei. Bei der Prüfung mit Build [365] lud `SevenKeysSerp.c4s` so das unveränderte `Serpens66Packv41.c4d` und erreichte den Spielstart. Der Kontrolllauf mit `General.DefinitionPath` auf `custom\` und ohne Hardlink brach beim weiterhin verlangten bloßen Dateinamen ab. Die Protokolle liegen unter `.work/custom-f41a0c4f/`. [NTFS-Hardlinks](https://learn.microsoft.com/en-us/windows/win32/fileio/hard-links-and-junctions), [Pfadbehandlung in Build 365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Game.cpp#L182-L208)

## Spieltest

- Eine Arbeitskopie mit dem kleinstmöglichen Szenario und seinen deklarierten Abhängigkeiten starten. Den Start mit Zielbuild und erwarteter Wirkung dokumentieren.
- Skriptfehler und gezielte `Log()`-Ausgaben in `Clonk.log` dem konkreten Lauf zuordnen. Beim Erfolg zusätzlich die sichtbare Wirkung prüfen, etwa ob das erwartete Objekt an der richtigen Stelle entsteht oder ein Callback nur im vorgesehenen Kontext auslöst.
- Speichern, Laden und Mehrspieler jeweils prüfen, wenn die Mod diese Modi unterstützen soll. Dazu den Paketstand einschließlich `System.c4g` für alle beteiligten Spielinstanzen erfassen. [Engine-README](https://github.com/legacyclonk/LegacyClonk#play)

## Quellcode und Beiträge

| Frage | Einstieg am Release-Stand |
| --- | --- |
| Wie ist die Funktion registriert und implementiert? | [C4Script.cpp](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp), [System.c4g](https://github.com/legacyclonk/LegacyClonk/tree/v365/planet/System.c4g) |
| Wie werden Skripte geparst, verknüpft und ausgeführt? | [C4AulParse.cpp](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp), [C4AulLink.cpp](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulLink.cpp), [C4AulExec.cpp](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp) |
| Wo entstehen Objekt-, Szenario- oder Effekt-Callbacks? | [Callback-Namen](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.h), [Szenarioablauf](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Game.cpp), [Effekte](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Effect.cpp) |
| Wie lädt oder verarbeitet die Engine C4-Gruppen? | [Gruppenimplementierung](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Group.cpp), [CLI-Einstieg](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/c4group_ng.cpp) |

Für einen Engine-Fix zuerst prüfen, ob der Fehler am vorgesehenen Upstream-Stand reproduzierbar ist. Die Änderung in einer eigenen Git-Arbeitskopie unter `.work/` vorbereiten und den Buildablauf aus der dortigen README verwenden. In einem beauftragten PR den reproduzierten Fall, den getesteten Quellstand und das beobachtete Ergebnis angeben. [Upstream-Buildanleitung](https://github.com/legacyclonk/LegacyClonk#compiling---quick-start)
