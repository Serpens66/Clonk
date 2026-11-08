# Historische Mod-Sammlung

Originalpfad ist [alte Projekte/allerleiFabian gute Sammlung40.c4d](../alte%20Projekte/allerleiFabian%20gute%20Sammlung40.c4d/). Der Ordner selbst ist eine offene C4-Gruppe. Er enthält 103 sichtbare Dateien einschließlich Unterordnern. Darunter liegen 56 gepackte `.c4d`-Dateien, 37 offene C4Script-Dateien, acht Textdateien, `Material.c4g` und die Autorentabelle `Serpens66AutorenundObjekte.xlsx`. Die sichtbaren Dateien in den gepackten Gruppen kommen hinzu. Mehrere der geprüften Archivlisten zeigen Bearbeitungsdaten bis März 2016.

## Einstiegspunkte und Abhängigkeiten

Alle Pfade in der Tabelle sind relativ zur Originalsammlung. Inhalte gepackter Gruppen wurden mit dem installierten `c4group` auf Kopien unter `.work/` geprüft. Die Beobachtungen stammen aus Archivlisten und Quelltexten.

| Thema | Originalpfad und beobachteter Zusammenhang |
| --- | --- |
| Umweltschutz | `Wichtige Skripte.txt` verweist auf Ruf der Wipfe. In `RufDerWipfe2.c4d/RdW.c4d/Effekte.c4d/Umweltschutz.c4d/Script.c` behandelt die Definition `_Z2A` unter anderem Klima und Verschmutzung. Einstiegspunkte sind `Klima` und `CheckPollution`. |
| Tageszeit | `RufDerWipfe2.c4d/RdW.c4d/Enivronment.c4d/Time.c4d/Script.c` gehört zu `TIME`. Das Skript enthält `SetTimeTIME` und `RestoreSkyColors`. Der Pfad `Enivronment.c4d` übernimmt die Schreibweise im Archiv. |
| Spielregeln und Einstellungen | `mehr oder weniger eigenes.c4d/Umwelt und Regeln.c4d/Settings.c4d/Script.c` gehört zu `CHO6` und führt `hashSpielRegeln`. Die Clonk-Erweiterung greift für die Tageszeit auf diesen Zustand zu. Als separates Beispiel legt `System.c4g/StandardSelectionRule.c` Standardregeln per `#appendto` für mehrere IDs fest, darunter `RUSC`, `EVSC` und `GOSC`. |
| Eigene Hash-Helfer | `System.c4g/HashTable.c` enthält eine arraybasierte Implementierung mit `CreateHash`, `HashPut` und `HashGet` für `#strict 2`. Der Kopf nennt Nicolas Hake (2008) und enthält die Lizenzbedingungen. Diese Namen gehören zur historischen Skriptbibliothek. Für eine Portierung erst die Aufrufer und das Verhalten bei gemischten Strict-Stufen prüfen. |
| Forschung und Wissensstände | `System.c4g/ForschungsbaumFKT.c` verteilt in `GlobalStartingEra` über `SetPlrKnowledge` Wissen aus den Listen der Sammlung. `System.c4g/Forschungsbaumappend.c` hängt sich mit `#appendto *` an Definitionen und definiert Forschungszeit und Kosten. Die Sonderbehandlung für `SZRE`, die Listen und die Definitionen unter `mehr oder weniger eigenes.c4d/Forschung.c4d` gehören zum Mod. |
| Clonk- und Waffenerweiterungen | `Erweiterungspack.c4d/Appends.c4d/Clonk.c4d/Script.c` kombiniert `#appendto CLNK` und `#include CLNK`. Die gepackte Definition selbst hat die ID `AT1C`. `System.c4g/appendWeapons.c` erweitert `WEPN` und greift auf modinterne Bauarbeiter- und Munitionsgürtel-Funktionen zu. Bei Wiederverwendung zuerst die Zieldefinitionen und deren Aufrufer im geladenen Paket prüfen. |
| Meeresspiegel | `System.c4g/Globale Naturfunktionen.c` verschiebt bei `gChangeMeeresspiegel` Objekte mit ID `DRAI`. `ShowMeeresspiegel` protokolliert außerdem `DRA2`. Das ist ein modbezogener Weg über vorhandene Objekte. Die Herkunft und Verfügbarkeit beider IDs im Testszenario vor einer Portierung prüfen. |
| Weitere gepackte Inhalte | Die Listen von `Material.c4g` und `NeueHazardwaffen.c4d` konnten gelesen werden. Das Materialpaket enthält `TexMap.txt` und `.c4m`-Definitionen. Das Waffenpaket enthält unter anderem `AntiContentRifle.c4d` und `SonicPistol.c4d`. `Credits.txt` führt die Herkunft vieler übernommener Inhalte auf, darunter Hazard und NeueHazardWaffen. |

## Historische Hinweise und Prüfung für [365]

- `Energie.txt` beschreibt eine beobachtete Obergrenze und vermutet einen Zusammenhang mit `[Physical] Energy`. Einheit, Skalierung und Anwendbarkeit auf `[365]` sind offen.
- `ToDo.txt` und `Aufeinmal machen.txt` mischen Pläne und Bearbeitungsstände. Eine erwähnte Funktion zuerst am Skript und an der passenden Definition suchen.
- `Credits.txt` und Lizenzköpfe sind die erste Anlaufstelle für Autorenschaft. Bei einer Übernahme die Berechtigung und Attribution für das konkrete Skript oder Asset prüfen.
- Für einen Kompatibilitätstest zuerst eine einzelne Funktion oder Gruppe in einem separaten Szenario mit den Original-Strict-Stufen und den benötigten IDs laden. Danach die sichtbare Wirkung und die Skriptmeldungen prüfen. Die statische Auswertung dieser Sammlung hat ihren Start und ihre Spielbarkeit unter `[365]` offen gelassen.
