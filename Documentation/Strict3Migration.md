# C4Script von #strict 2 auf #strict 3 umstellen

In LegacyClonk kann jede Skriptdatei ihre Strict-Stufe separat wählen. Bei `#include` und `#appendto` behält eine Funktion die Stufe ihrer Ursprungsdatei. Eine Migration kann daher schrittweise erfolgen. Ziel dieses Leitfadens ist LegacyClonk 4.9.11.2 [365], Tag `v365`, Commit `31b2cde5d1d68a86765c68c8252184ca38469c0d`. Für Syntax und API-Aufrufe siehe die [Sprachreferenz](C4Script.md) und den [API-Katalog](Reference/C4Script_API.md). [SDK zu #strict](https://crdocs.clonkspot.org/en/sdk/script/Strict.html), [Entwicklerbeitrag mit Beispielen](https://forum.clonkspot.org/t/modernes-scripting-in-legacyclonk-mit-strict-3/1462)

## Ablauf für eine Migration

1. Zielbuild und betroffene Dateien bestimmen. Bei Clonk-Rage-Kompatibilität bleibt `#strict 2` die passende Basis. Vor der Änderung ermitteln, wie die Funktionen aufgerufen werden und welche Definitionen, Includes, Appends, Systemskripte und Stringtabellen das Szenario lädt.
2. Altverhalten festhalten. Für jeden betroffenen Wert klären, ob er eine echte Zahl, ein Wahrheitswert, eine ID oder „fehlt“ bedeutet. Suchstellen sind `0`, `false`, `NONE`, `true`/Vergleiche mit `1`, ungesetzte Variablen, `return;`, Rückgaben aus Suchfunktionen, Platzhalter in Aufrufen, `GetType`, `ChangeDef` und Code in Zeichenketten.
3. In einer Arbeitskopie eine Datei oder zusammengehörige Funktionen umstellen. `#strict 3` an den Anfang der Datei setzen. Dateikodierung, Zeilenenden, Callbacks, IDs und Format der Gruppendateien erhalten. Argumente und Rückgaben anhand ihrer Bedeutung korrigieren. Standardwerte pro Parameter prüfen.
4. Die normale Einstiegsstelle im Zielbuild laden. Im Log nach Parser- und Laufzeitfehlern suchen und dieselben Aktionen mit Alt- und Neufassung vergleichen. Mindestens fehlende und gesetzte Werte, `0`, `false` sowie Aufrufe zwischen beiden Strict-Stufen prüfen. Bei betroffenem Zustand auch Laden bestehender Spielstände, Sektionswechsel und Mehrspieler testen. [Spieltest im Entwicklungsleitfaden](Development.md#spieltest)

## Werte, Vergleiche und Rechnen

| Bedeutung | Schreibweise in Stufe 3 | Bei der Umstellung prüfen |
| --- | --- | --- |
| Ein Wert fehlt | `nil`, `x == nil`, `x != nil` | `x == 0`, `x == false` oder `x == NONE` nur ändern, wenn der Test tatsächlich „fehlt“ meinte. |
| Zahl null | `0` vom Typ `int` | Zähler, Koordinaten, Indizes, Preise, Farben und Spielernummern können gültig null sein. |
| Falscher Wahrheitswert | `false` vom Typ `bool` | `false != 0` und `true != 1`. Boolesche Funktionsresultate als solche behandeln. |
| Keine Definitions-ID | `NONE` vom Typ `id` | Als ID-Sentinel beibehalten, wenn die Schnittstelle genau diesen Wert meint. Als typfremden Platzhalter durch `nil` ersetzen. |
| Bedingung für mehrere falsche Werte | `if (!x)` | `nil`, `0`, `false` und `NONE` gelten in Bedingungen als falsch. Für einen reinen Existenztest explizit mit `nil` vergleichen. |

`==` und `!=` unterscheiden in Stufe 3 die Typen der beiden Operanden. In Stufe 2 gelten beispielsweise `0 == false` und `1 == true`. Stufe 3 vergleicht Strings nach Inhalt und Objekte nach Identität. Array- und Map-Vergleiche delegieren an die jeweiligen Container. Für typgenaue Tests verschachtelter Werte die betroffenen Elemente direkt prüfen. `GetType(0)` und `GetType(false)` liefern in Stufe 3 `C4V_Int` bzw. `C4V_Bool`. `GetType(nil)` liefert `C4V_Any`. `Format` und `Log` mit `%v` können Werte typabhängig darstellen. Strings, die für Menüs oder andere Schnittstellen ausgewertet werden, entsprechend prüfen. [Quellcode v365 zu Gleichheit](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Value.cpp#L824-L893), [GetType](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L3959-L3963), [Format](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L138-L151)

Initialisiere Zähler und andere Zahlen, die unmittelbar in Rechnungen eingehen, explizit mit ihrem Startwert. Eine ungesetzte Variable kann `nil` sein. Bei absichtlich optionalen Zahlen verwende etwa `(amount ?? 0) + 1`, sofern „fehlt“ fachlich als null zählt. Integeroperatoren wie `+` und `++` können mit `nil` oder einem unpassenden Typ einen Laufzeitfehler auslösen. `x ?? fallback` und `x ??= fallback` werten den Ersatz nur für `nil` aus. `x ??= fallback` braucht links eine schreibbare Stelle. Eine echte `0` oder `false` löst den Ersatz nicht aus. [Quellcode v365 zur Operatorprüfung](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L281-L329), [Kurzschlussauswertung im Parser](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L3013-L3035)

## Argumente, Rückgaben und Aufrufgrenzen

In Stufe 3 bleibt ein übergebenes `0`, `false` oder `NONE` typisiert. Wurde damit ein fehlendes `object`, `string`, `array` oder `map` bezeichnet, verwende `nil` oder lass das Argument aus. Beispiel für die Suche nach dem Wert einer Definition mit der Signatur `GetValue(object, id, object, int)`:

```c
// #strict 2
var value = GetValue(0, ROCK);

// #strict 3
var value = GetValue(nil, ROCK);
```

`return;` und das Erreichen des Funktionsendes liefern in Stufe 3 `nil`. Bei expliziten Rückgaben `return 0;` und `return false;` anhand der Aufrufer entscheiden, ob sie einen echten Wert oder das Fehlen eines Ergebnisses ausdrücken. Boolesche Engine-Callbacks und numerische Statuscodes haben eigene Verträge. Bei APIs wie `GetValue` kann ein fehlgeschlagener Aufruf `nil` liefern, auch wenn die SDK-Signatur einen Integer beschreibt. Verarbeite das Ergebnis nach dem konkreten API-Vertrag. [Quellcode v365 zu GetValue](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L1441-L1462), [Implizite Rückgabe](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1878-L1886), [Leeres return](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2091-L2102)

| Aufruf | Verhalten in v365 |
| --- | --- |
| Stufe 3 → Stufe 3 | Ausgelassene Argumente kommen bei Skriptfunktionen als `nil` an, auch bei `int` und `bool`. Für bewusst optionale Parameter z. B. `amount ??= 10;` und `active ??= true;` verwenden. Explizite `0` und `false` bleiben erhalten. |
| Stufe 2 → Stufe 3 | Beim direkten Skriptaufruf normalisiert die Engine falsche Werte vor der Typprüfung. Fehlende bzw. normalisierte Werte werden für `int` zu `0`, für `bool` zu `false`. Bei einem untypisierten Parameter kann `nil` ankommen. Ein Standardwert mit `??=` kann deshalb bei einem alten Aufrufer anders greifen als bei einem neuen. |
| Stufe 3 → Stufe 2 | Die Argumentprüfung am direkten Aufruf richtet sich nach dem Aufrufer. Eine alte Zielfunktion garantiert keine alten Konvertierungen am Aufruf. In ihrem Rumpf gelten die alten Regeln. Ein dort geschriebenes `return 0;` oder `return false;` kann beim neuen Aufrufer als `nil` ankommen. Normalisiere bei Bedarf am Aufruf, etwa mit `LegacyCount() ?? 0`. |
| Indirekt oder Engine → Skript | `Call`, `ObjectCall`, `DefinitionCall`, `GameCall` und Effektaufrufe haben eigene Pfade. Beispielsweise kann `ObjectCall` Konvertierungsfehler bei alten Zielfunktionen als Warnung behandeln. Engine-Callbacks separat mit den tatsächlichen Argumenten testen. |

Die Tabelle beschreibt die Aufrufstellen und typischen Werte. Die Engine konvertiert Rückgaben nicht pauschal beim Übergang zwischen Stufen. Auch innerhalb einer Stufe entscheidet bei einem API-Aufruf dessen Signatur, welche Konvertierungen und Defaults gelten. [Quellcode v365 zur Argumentprüfung](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L1381-L1413), [Direkte Aufrufe](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L1449-L1456), [Alte Null-Literale](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1114-L1122), [Indirekte Aufrufe](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L3581-L3691)

## Weitere Stellen mit geändertem Verhalten

- **Farben.** Der optionale erste Parameter von `ModulateColor` und der Farbparameter von `CustomMessage` verwenden bei `nil` den bisherigen weißen Standard. Die explizite Farbe `0` steht in Stufe 3 für Schwarz. In den älteren Release-Notizen heißt die erste Funktion `ModulateClr`. Die Registrierung in `v365` lautet `ModulateColor`. [Release [348]](https://clonkspot.org/blog/2021/04/12/legacyclonk-4-9-10-11-348/), [Registrierung](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L7175), [CustomMessage](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L6170-L6198)
- **Definitionswechsel.** Nach einem erfolgreichen `ChangeDef` auf eine andere Definition kann der laufende Funktionsrumpf in Stufe 3 nicht mehr über die alten Namen direkt auf Objekt-`local`-Variablen zugreifen. Benötigte Werte vorher in `var` sichern und die weitere Verarbeitung in eine Funktion der Zieldefinition verlagern. [Quellcode v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L434-L455)
- **Lange Stringliterale und dynamischer Code.** Der Parser meldet bei mehr als 1024 Bytes im Puffer eines Literals in Stufe 3 einen Fehler, in Stufe 2 eine Warnung. Bei `eval` erbt der ausgewertete Code die Strict-Stufe des Aufrufers. `/script` und Konsoleneingaben richten sich dagegen nach `Developer.ConsoleScriptStrictness`, standardmäßig der höchsten Stufe. [Literalparser](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L773-L802), [eval](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L4668-L4679), [Konsolenkonfiguration](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Config.cpp#L121-L144)
- **Objektbefehle.** `PlayerObjectCommand("Call", ...)` hat keinen implementierten Ausführungspfad und kann je nach Strict-Kontext eine Warnung oder einen Fehler erzeugen. Die beabsichtigte Aktion über die passende Schnittstelle abbilden. [Quellcode v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L991-L1016)
- **Sichtbarkeit.** Das SDK beschreibt eine Prüfung von `private` und `protected` für `->` zwischen zwei Strict-3-Skripten. In `v365` übergibt der dynamische Aufrufpfad bei der Prüfung das Ursprungsskript der Zielfunktion als Aufrufer. Für diesen Fall liefert die Hilfsfunktion `AA_PRIVATE` und der Check sperrt den Aufruf nicht. Sichtbarkeit beim Portieren gemäß der beabsichtigten Schnittstelle beibehalten. Wenn Zugriffsschutz für den Mod relevant ist, den konkreten Aufrufpfad auf dem Zielbuild testen. [SDK](https://crdocs.clonkspot.org/en/sdk/script/Strict.html), [Aufrufpfad](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L1298-L1304), [GetAllowedAccess](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Aul.cpp#L394-L416)

## Neue Syntax gezielt verwenden

| Schreibweise | Einsatz |
| --- | --- |
| `var options = {count = 0, active = false};` | Map mit Stringschlüsseln. `options.count` und `options["count"]` greifen auf denselben Eintrag zu. Ein berechneter Schlüssel steht im Literal in eckigen Klammern, etwa `{[key] = value}`. Build [365] erwartet im Literal `=`. Das SDK zeigt teils ein Beispiel mit Doppelpunkt. |
| `obj.energy` | Benanntes Objekt-`local`, vergleichbar mit `obj["energy"]`. Auf vorhandene Variablen und den Zielkontext achten. |
| `obj?.energy`, `items?[i]`, `obj?->GetFoo()`, `obj?->~GetFoo()` | Bei `nil` als Basis wird die Navigationskette übersprungen und liefert `nil`. Das Ergebnis ist ein Wert, keine schreibbare Referenz. `~` behandelt zusätzlich eine fehlende Funktion auf einem vorhandenen Ziel. Bei mehreren möglicherweise fehlenden Zwischenergebnissen den jeweiligen Zugriff absichern. |
| `global->CreateObject(ROCK, 50, 30)` | Aufruf ohne Objekt- und Definitionskontext. Koordinaten im Beispiel sind absolut. Die optionale Aufrufform lautet `global->~Name(...)`. Eine solche Änderung nur bei gewünschtem Kontextwechsel einsetzen. |

`??`, `??=`, `items[] = value`, `..`, `..=`, Stringindexierung und erweiterte Zuweisungsoperatoren gehören ebenfalls zu LegacyClonk. Der Parser von `v365` reserviert diese Formen nicht ausschließlich für Stufe 3. Bei einer Migration können sie für getrennte Aufräumarbeiten genutzt werden. Beim Ersetzen einer alten Standardwertprüfung durch `??` muss klar sein, ob `0` und `false` weiterhin als fehlend gelten sollen. [Map-Literalparser](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2410-L2464), [Navigation](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L3115-L3164), [Operatoren](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L435-L479), [LegacyClonk-Release-Notizen](https://forum.clonkspot.org/t/legacyclonk-330-337-alte-versionen/1245)

## Spieltest

Am 9. November 2026 lief `StrictProbe.c4s` mit Build [365] win64 im lokalen Konsolenmodus, mit `Objects.c4d` und einem lokalen Strict-2-Systemskript. Der Linker meldete 0 Warnungen und 0 Fehler. Die Ausgaben bestätigten `0 == false` in Stufe 2, die Unterscheidung in Stufe 3 und `nil` aus einem alten `return 0;`. Ein alter Aufrufer übergab ausgelassene `int`-/`bool`-Parameter als `0`/`false`, ein neuer Aufrufer als `nil`. Map-Literal, `??`, `??=` und `?.` lieferten die erwarteten Werte. Skripte und `Clonk.log` liegen unter `.work/strict3-probe-ae6de4b9/`. Die übrigen Regeln oben beruhen auf den jeweils verlinkten SDK- und Quellcodeprüfungen.
