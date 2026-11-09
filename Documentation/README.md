# LegacyClonk-Wissensbasis

Die Wissensbasis unterstützt neue Mods, Szenarien und Werkzeuge für LegacyClonk. Der letzte beobachtete lokale Spielstart am 8. Oktober 2026 meldete **4.9.11.2 [365] (Windows x64)**. Für Aussagen über diesen Build dient der Release-Tag `v365` als Quellreferenz. Die beim Einrichten geprüfte Spitze von `master` gehört bereits zu Build `[366]`. Beide Stände stehen im [Quellenregister](Sources.md).

| Aufgabe | Einstieg |
| --- | --- |
| Szenario, Objektdefinition oder C4Script schreiben | [Entwicklungsleitfaden](Development.md) |
| LegacyClonk-C4Script lesen oder schreiben | [Sprachreferenz](C4Script.md) |
| Alte Skripte von `#strict 2` auf `#strict 3` umstellen | [Migrationsleitfaden](Strict3Migration.md) |
| Funktion, Konstante, Callback oder Syntaxform finden | [Alphabetischer API-Katalog](Reference/C4Script_API.md) |
| API-Einträge maschinell durchsuchen oder aktualisieren | [JSON-Katalog](Reference/C4Script_API.json), [Erzeugung](Reference/README.md) |
| Pakete untersuchen, packen oder für einen Spieltest vorbereiten | [Entwicklungsleitfaden](Development.md#pakete-und-werkzeuge) |
| Ein Verhalten im Engine-Code nachvollziehen oder einen Engine-Beitrag vorbereiten | [Quellcode und Beiträge](Development.md#quellcode-und-beiträge) |
| Ein Beispiel aus der alten Sammlung wiederfinden | [Historische Sammlung](Historical_Collection.md) |

## Belege pflegen

- **SDK:** Dokumentierte Schnittstelle mit URL und, soweit angegeben, Versionsangabe.
- **Quellcode:** Beobachtung an einem konkreten Commit. Eine Registrierung belegt Namen und Zuordnung. Details zu Aufrufkontext und Laufzeit können weitere Aufrufstellen oder Tests erfordern.
- **Archiv:** Historischer Quelltext oder eine Projektnotiz. Aus alten Szenarien, Paketen und Codebeispielen ergibt sich eine Fragestellung für den aktuellen Zielbuild.
- **Werkzeugtest:** Nachvollziehbarer Test mit Werkzeug, Eingabe und beobachtetem Ergebnis.
- **Spieltest:** Tatsächlicher Lauf mit Build, Szenario, geladenen Paketen, Modus und beobachtetem Ergebnis.

Neue Erkenntnisse bei dem jeweiligen Thema ergänzen und bestehende Quellen per Commit oder Originalpfad auffindbar halten. Bei widersprüchlichen Quellen die konkreten Unterschiede und offenen Prüfschritte festhalten.
