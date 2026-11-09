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

## Entpackte Arbeitsfassung

Die damalige Arbeitsfassung von Sammlung40 enthielt **2.325 entpackte Gruppen** einschließlich der verschachtelten Gruppen und **13.272 Dateien**. Die Ordner behalten ihre C4-Endungen, beispielsweise `aus Bauklotzpack.c4d`. Damit kann LegacyClonk sie als offene Gruppen erkennen. [SDK zu Objektdefinitionen](https://crdocs.clonkspot.org/en/sdk/definition/index.html)

Bei der Prüfung am 8. November 2026 wurden Namen, Anzahl und Inhalt aller extrahierten Archiveinträge gegen die ursprünglichen C4-Gruppen geprüft. Die SHA-256-Prüfsummen aller Dateien stimmen mit dem entpackten Ausgangsstand überein, abgesehen von den fünf unten aufgeführten Kommentarkorrekturen. Auch die SHA-256-Prüfsummen der historischen Ausgangsdateien wurden erneut geprüft. Mit dem installierten `c4group.exe` ließen sich `aus Bauklotzpack.c4d` und die verschachtelte Gruppe `Erweiterungspack.c4d/Appends.c4d/Clonk.c4d` als offene Ordner auflisten. In letzterer wurde `Script.c` gefunden. Die Spielbarkeit der Sammlung unter `[365]` ist weiterhin offen.

Der zweite Kodierungsscan umfasste **10.139 Textdateien**, einschließlich sechs Textsicherungen. Davon sind 7.309 ASCII, 2.795 Windows-1252, 26 UTF-8 und neun RTF. Die RTF-Dateien wurden anhand ihrer Codepage geprüft. In Notepad++ bei falsch dargestellten Umlauten die Kodierung der jeweiligen Datei wählen. Die bestehende Dateikodierung und die Zeilenenden beim Speichern beibehalten.

| Datei in der Arbeitsfassung | Zeile | Korrektur |
| --- | ---: | --- |
| `aus BlacksitePack.c4d/Living.c4d/Mireok.c4d/Script.c` | 2327 | „Bewaffnung prüfen“ in Windows-1252 |
| `mehr oder weniger eigenes.c4d/Umwelt und Regeln.c4d/Settings.c4d/Script.c` | 299 | „Hauptmenü“ in Windows-1252 |
| `Space Empire.c4d/Hazard.c4d/Crew.c4d/HazardClonk.c4d/Script.c` | 890 | „Bewaffnung prüfen“ in Windows-1252 |
| `Space Empire.c4d/Hazard.c4d/Crew.c4d/HazardClonk.c4d/Script.c~` | 681 | Dieselbe Korrektur in der historischen Textsicherung |
| `Space Empire.c4d/Hazard.c4d/Items.c4d/Weapons.c4d/Weapon.c4d/Script.c` | 361 | „Schüssen“ in UTF-8 |

## Arbeitsfassung Sammlung42

Am 9. November 2026 wurde die Sicherung `alte Projekte/Clonk Rage/allerleiFabian gute Sammlung42.c4d` als [bearbeitbare Arbeitsfassung](../Mods/allerleiFabian%20gute%20Sammlung42.c4d/) abgelegt. `c4group` öffnete 2.330 gepackte Gruppen einschließlich der Untergruppen. Alle 13.321 Dateien wurden gegen die ursprünglichen Archiveinträge und offenen Dateien geprüft. Die Ordner behalten ihre C4-Endungen. Die Prüfung erfasst die Archivstruktur und Dateiinhalte. Die Spielkompatibilität mit LegacyClonk bleibt offen.

Das `Serpens66Packv41.c4d` aus derselben Sicherung ist bytegleich mit dem öffentlichen Release. Sammlung42 enthält dieselben 13.321 relativen Dateipfade. Bei 15 Dateien weicht der Inhalt ab, darunter Skripte für Schachtsperre, Aufzug und Forschung. [Serpens66Pack v41 bei CCAN](https://ccan.de/cgi-bin/ccan/ccan-view.pl?a=view&i=6256)

## Serpens66Pack v41 getrennt starten

**Archivbefund.** Das Archiv `alte Projekte/Clonk Rage` enthält sowohl `Serpens66Packv41.c4d` als auch `Serpens66Mapsv41.c4f`. Alle sechs mitgelieferten Szenarien haben in ihrer `Scenario.txt` den Eintrag `Definition1=Serpens66Packv41.c4d`. Die Notiz `Andere Szenarien integrieren.txt` im Kartenpaket beschreibt den Ersatz von `Objects.c4d` durch das Pack bei der Integration anderer Szenarien. Das Pack enthält eine eigene `Objects.c4d` als Untergruppe. [SDK zu Scenario.txt](https://crdocs.clonkspot.org/de/sdk/scenario/scenario.html#L127)

**Spieltest vom 9. November 2026 mit LegacyClonk 4.9.11.2 [365] win64.** In der Spielkopie unter `.work/paketpfade-0328dabf/Engine/` lagen das unveränderte Definitionspaket und die entpackten Karten unter `Eigene Projekte/Serpens66 v41/`. In den sechs `Scenario.txt` der Arbeitskopie wurde jeweils nur der Paketverweis auf `Definition1=Eigene Projekte\Serpens66 v41\Serpens66Packv41.c4d` geändert. Der direkte Start von `SevenKeysSerp.c4s` im Konsolenmodus lud 1.955 Definitionen aus dem Pack und 14 lokale Definitionen. Das Spiel meldete einen erfolgreichen Start. Das Protokoll liegt unter `.work/paketpfade-0328dabf/SevenKeysSerp-v41.log`. Geprüft wurde der Start dieser einen Karte.

**Startprüfung des veröffentlichten v41-Paars am 9. November 2026.** In einer isolierten Kopie von LegacyClonk 4.9.11.2 [365] win64 wurden die beiden Dateien aus `custom/` der Spielinstallation unverändert verwendet. Mit beiden Archiven unter `custom/` brach `BasenMelee.c4s` ab, weil die in `Scenario.txt` geforderte Datei `Serpens66Packv41.c4d` nicht gefunden wurde. Mit dem Definitionspaket neben `clonk.exe` und dem Kartenpaket weiter unter `custom/` starteten alle sechs Szenarien offline im Konsolenmodus. Das Log meldete jeweils 1.955 Definitionen aus dem Pack, vier Warnungen und null Fehler beim Verknüpfen der Skripte, einen erfolgreichen Spielstart und den Beitritt des Testspielers.

Die Logs enthalten außerdem Warnungen zu ungültigen Versionsangaben und fehlenden Textschlüsseln in `Deathcrow.c4d`. Ein Kontrolllauf mit einer gepackten Testspielerdatei beseitigte bei `BasenMelee` die Fehlermeldung des ersten Testaufbaus zur offenen Spielergruppe. Die Logs liegen unter `.work/v41-smoke-1a161beb/` und `.work/v41-packed-player-b687ae04/Clonk.log`. Geprüft wurden Laden, Skriptverknüpfung und die ersten Sekunden der Partie.
