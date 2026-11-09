# LegacyClonk-C4Script lesen und schreiben

Diese Sprachreferenz gilt für LegacyClonk 4.9.11.2 [365], Tag `v365`, Commit `31b2cde5d1d68a86765c68c8252184ca38469c0d`. Für Funktionen und Konstanten verwende den [API-Katalog](Reference/C4Script_API.md). Für Altcode gibt es den [Migrationsleitfaden für #strict 3](Strict3Migration.md). Die [Quellenübersicht](Sources.md) und der [Entwicklungsleitfaden](Development.md) beschreiben den Versionsabgleich und die Paket- und Spieltests.

## Datei und Aufrufstelle bestimmen

| Fundort | Rolle | Beim Lesen und Schreiben prüfen |
| --- | --- | --- |
| `Szenario.c4s/Script.c` | Ablauf der Runde, z. B. `Initialize()` und `InitializePlayer(int player)` | `Scenario.txt`, geladene Definitionspakete, Initialisierung von Runde und Spielern, Aufruf ohne aktuelles Objekt. |
| `Objekt.c4d/Script.c` | Verhalten einer Definition oder ihrer Objektinstanzen | ID aus `DefCore.txt`, Aktionen und Callbacknamen aus `ActMap.txt`, Objekt- oder Definitionskontext. |
| `System.c4g/*.c` | Systemfunktionen und Erweiterungen anderer Skripte | Wo die Gruppe geladen wird, Namen globaler Helfer, tatsächliche Ziel-IDs und deren Overrides. |
| `StringTblDE.txt`, `StringTblUS.txt` | Sprachabhängiger Text | `"$TextKey$"` im Skript verweist auf `TextKey=…` in der Stringtabelle der Gruppe. Übersetzungen und Platzhalter bei Änderungen zusammen prüfen. |

`Script.c` ist C4Script-Quelltext. Ein `.c4s` oder `.c4d` kann ein Ordner oder eine gepackte Gruppe sein. Bearbeite gepackte Inhalte in entpackten Arbeitskopien. Beim Speichern vorhandener Dateien Kodierung, Zeilenenden und bei Bedarf Sprachdateien erhalten. Für einen neuen Minimaltest reichen ASCII-Skripttexte. Sonderzeichen in Skripten und Sprachdateien mit der Kodierung der jeweiligen Gruppe im Zielbuild prüfen. [SDK zu Szenarioskripten](https://crdocs.clonkspot.org/en/sdk/scenario/script.html), [Objektskripte](https://crdocs.clonkspot.org/de/sdk/definition/script.html), [Lokalisierung](https://crdocs.clonkspot.org/en/sdk/lang.html), [Laden von Skript und Stringtabelle in v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4ScriptHost.cpp#L43-L77)

## Direktiven und Funktionen

Setze die gewählte Strict-Direktive oben in jede Skriptdatei. Für neue LegacyClonk-Skripte ist `#strict 3` der Ausgangspunkt. `#strict 2` kommt für bestehende Skripte und Clonk-Rage-Kompatibilität infrage. `#strict` ohne Zahl bedeutet Stufe 1. Ohne Direktive gilt der alte Modus. `#include C4ID` bindet das Skript der geladenen Definition ein. `#appendto C4ID` ergänzt oder überlädt dieses Skript aus einer anderen Datei. `#appendto C4ID nowarn` unterdrückt die Warnung, wenn die Zieldefinition fehlt. Den Joker `#appendto *` nur verwenden, wenn alle geladenen Definitionen Ziel sein sollen. Die Strict-Stufe einer eingebundenen Funktion stammt aus ihrer Ursprungsdatei. [SDK zu #strict](https://crdocs.clonkspot.org/en/sdk/script/Strict.html), [Parser v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1451-L1506)

Schreibe Funktionen als `func Name(int count, object target)` mit einem Rumpf in `{ ... }`. Typangaben stehen an Parametern und sind optional. Die SDK-Signatur `object FindObject2(...)` dokumentiert den Rückgabetyp. Bei einer eigenen Skriptfunktion schreibt man dafür `func Name(...)`. Der Build unterstützt bis zu zehn Argumente. `...` in einer Deklaration lässt zusätzliche Argumente zu. `Par(0)` liest das erste Argument. In einem Aufruf reicht `...` die unbenannten Argumente der aktuellen Funktion weiter. Benannte Parameter bei Bedarf ausdrücklich voranstellen, etwa `return _inherited(player, ...);`. Bei `return;` oder am Funktionsende liefert Strict 3 `nil`. [Parser zu Funktionsparametern](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1638-L1701), [Aufrufargumente](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2272-L2350)

`public func` bezeichnet die normale Schnittstelle und ist die Voreinstellung. `protected func` und `private func` kennzeichnen engere Sichtbarkeit. `global func` registriert eine skriptübergreifend verfügbare Funktion. Gib neuen globalen Funktionen und Variablen einen zum Mod passenden Präfix. Beim Überladen ruft `inherited(...)` die vorige Implementierung derselben Funktion auf. `_inherited(...)` lässt eine fehlende Vorgängerfunktion zu. Ein Aufruf der Funktion über ihren eigenen Namen ist rekursiv. Prüfe beim Lesen eines Overrides, welche Implementierung durch die tatsächlich geladenen Pakete erreicht wird. Zur Abweichung bei der Laufzeitprüfung von `->` siehe den [Migrationsleitfaden](Strict3Migration.md#weitere-stellen-mit-geändertem-verhalten). [SDK zu inherited](https://crdocs.clonkspot.org/en/sdk/script/fn/inherited.html), [Parser zu inherited](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2785-L2808)

## Werte, Variablen und Referenzen

| Syntax oder Typ | Regel |
| --- | --- |
| `var n = 0;` | Funktionsvariable für den aktuellen Aufruf. Der Name ist im Funktionsrumpf sichtbar. Eine Deklaration in einem Block führt keinen eigenen Namensraum ein. |
| `local energy;` | Benannte Variable pro Objektinstanz. Im Objektkontext direkt verwendbar. Von außen mit `obj["energy"]` bzw. in Strict 3 mit `obj.energy` auf das konkrete Objekt zugreifen. |
| `static MyModState;` | Benannte globale Variable für alle Skripte der Runde. Initialisierung zu einem definierten Zeitpunkt vornehmen. |
| `static const MyModLimit = 10;` | Globale Konstante. `v365` akzeptiert ein direktes Literal oder den Namen einer bekannten Konstante. Ausdrücke wie `1 << 5` gehören in ausführbaren Code. |
| `int`, `bool` | 32-Bit-Ganzzahl bzw. Wahrheitswert. `0`, `false` und `nil` sind in Strict 3 verschieden. |
| `id`, `object` | `ROCK` ist ein unquotiertes Literal für eine Definitions-ID. `"ROCK"` ist ein String. Ein konkreter Stein aus `FindObject2(Find_ID(ROCK))` ist ein Objekt. `NONE` bezeichnet die leere Definitions-ID. |
| `string` | Doppelte Anführungszeichen für Literale. `\"` maskiert ein Anführungszeichen, `\\` einen Backslash. Der Lexer behandelt diese beiden Escapeformen. Stringindexierung ist lesend und beginnt bei 0. Negative Stringindizes zählen vom Ende. |
| `array`, `map` | `[10, 20]` hat numerische Indizes ab 0. `{price = 10}` speichert Werte unter Schlüsseln. In Strict 3 liefert ein fehlender Map-Eintrag `nil` und Zuweisung von `nil` entfernt den Eintrag. |
| `any`, `nil` | `any` als Parametertyp akzeptiert beliebige Werte. `nil` ist der fehlende Wert. Der Test auf seinen Typ lautet `GetType(x) == C4V_Any`. |
| `func SetValue(&slot)` | Referenzparameter. `SetValue(items[0])` kann den Eintrag beim Aufrufer ändern, wenn die Funktion `slot` zuweist. Übergib eine schreibbare Stelle. Der Aufruf braucht kein `&`. |

Arrays und Maps haben Wertsemantik mit verzögertem Kopieren. Wenn ein Helfer den Container des Aufrufers verändern soll, verwende eine Referenz oder gib den geänderten Wert zurück und weise ihn zu. In einem Container enthaltene Objekte bezeichnen weiterhin dieselben Instanzen. Für Arrays stehen `GetLength`, `SetLength` und `items[] = value` zur Verfügung. Für Maps gibt es `GetKeys`, `GetValues` und `GetLength`. Historische Skripte können nummerierte Speicherstellen über `Var(i)`, `Local(i, obj)`, `Global(i)` und Parameter über `Par(i)` verwenden. Beim Lesen die jeweilige Speicherstelle und ihren Kontext nachverfolgen. [SDK zu Datentypen](https://crdocs.clonkspot.org/en/sdk/script/Typechecks.html), [Benannte Variablen](https://crdocs.clonkspot.org/en/sdk/script/NamedVar.html), [Konstantenparser v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L3385-L3432), [Stringparser v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L773-L812)

## Anweisungen und Ausdrücke

Schließe Deklarationen, Zuweisungen, Aufrufe als Anweisung und `return` mit einem Semikolon ab. Blöcke verwenden geschweifte Klammern. Bezeichner in Stufe 2 und 3 aus ASCII-Buchstaben, Ziffern und Unterstrich bilden. Für eigene Namen mit einem Buchstaben oder Unterstrich beginnen. `//` kommentiert bis Zeilenende und `/* ... */` einen Block. Alte Skripte können Beschreibungen in eckigen Klammern direkt nach der Funktionssignatur enthalten. Diese gehören zum Skript, etwa für Kontextmenüs. [SDK-Skripteinführung](https://crdocs.clonkspot.org/en/sdk/script/index.html), [SDK zu #strict](https://crdocs.clonkspot.org/en/sdk/script/Strict.html)

- `if (condition) { ... } else { ... }`, `while (condition) { ... }` und `for (var i = 0; i < length; ++i) { ... }` sind die normalen Kontrollstrukturen. `for (var item in items)` iteriert über ein Array. `for (var key, value in options)` liefert Schlüssel und Wert einer Map. Die Reihenfolge einer Map beim Schreiben von Spiellogik als ungeordnet behandeln. `break` verlässt die innere Schleife, `continue` setzt sie mit der nächsten Iteration fort. LegacyClonk-Code sollte sich auf diese vom Parser angebotenen Schleifen und Bedingungen stützen.
- `=` weist zu. `==`, `!=`, `<`, `<=`, `>`, `>=` vergleichen. `!`, `&&` und `||` arbeiten mit Wahrheitswerten. In Stufe 2 und 3 werten `&&` und `||` rechts nur bei Bedarf aus. `if (!target)` prüft im booleschen Kontext. `target == nil` fragt in Stufe 3 gezielt nach einem fehlenden Wert.
- `+`, `-`, `*`, `/`, `%` und `**` rechnen mit Ganzzahlen. `&`, `|`, `^`, `~`, `<<` und `>>` arbeiten auf Bits. `++`, `--` und kombinierte Zuweisungen wie `+=` brauchen einen schreibbaren linken Operanden bzw. Zielwert. Rechenwerte initialisieren und bei ganzzahliger Division Rundung und Division durch null berücksichtigen.
- `..` verbindet unterstützte String-, Array- oder Map-Operanden. `??` und `??=` verwenden einen Ersatz für `nil`. Bei gemischten Ausdrücken Klammern zur Verdeutlichung setzen. Für `?.`, `?[]`, `?->` und `?->~` siehe die [Navigationsbeispiele](Strict3Migration.md#neue-syntax-gezielt-verwenden).
- `{key = value}` ist in einem Ausdruck ein Map-Literal. `key` steht dort für den String `"key"`. Für den Inhalt einer Variable als Schlüssel schreibt man `{[key] = value}`. Für komplexe Bedingungen verwende `if` und `else`. Die Schreibweise `condition ? a : b` gehört nicht zum Parser von `v365`.

Die Operatoren binden in `v365` in dieser Reihenfolge, von stark nach schwach. Zuerst kommen Navigation und Aufruf, dann Postfix-`++`/`--`, Präfixoperatoren, `**`, `*`/`/`/`%`, `+`/`-`, Schieben, Größenvergleiche, `..`, Gleichheitsvergleiche, bitweises `&`, bitweises `^` und `|` auf derselben Stufe, `&&`, `||`, `??` und Zuweisungen. Zuweisungen gruppieren von rechts. Für Lesbarkeit insbesondere Potenzen und gemischte Bitmasken klammern. [Operatortabelle v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L423-L485), [Map-Schleifen im Parser](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2643-L2688) Das SDK erklärt [Schleifenkontrolle](https://crdocs.clonkspot.org/en/sdk/script/BreakContinue.html) und [Datentypen und Referenzen](https://crdocs.clonkspot.org/en/sdk/script/Typechecks.html). Bei Beispielen aus OpenClonk zuerst die Schnittstellen prüfen. Dessen Proplists, Effektobjekte und gleichnamige Suchfunktionen können andere Verträge haben.

## Aufrufkontext, Suche und Callbacks

| Aufruf | Kontext |
| --- | --- |
| `Name(args)` | Sucht eine Funktion im aktuellen Skriptkontext. Im Objekt bleibt der aktuelle Empfänger erhalten. |
| `obj->Name(args)` | Führt die Funktion im Zielobjekt aus. `this` bezeichnet dort das Ziel. Vorher sicherstellen, dass das Objekt existiert. `obj->~Name(args)` erlaubt eine fehlende Funktion auf einem vorhandenen Ziel. `obj?->~Name(args)` sichert zusätzlich ein fehlendes Ziel ab. |
| `ROCK->Name(args)` | Definitionsaufruf. Der Empfänger ist die Definition, ohne Objektinstanz. Benannte Objekt-`local`-Variablen setzen eine Instanz voraus. |
| `global->Name(args)` | In Strict 3 global ohne Objekt und ohne Definition. Zwischen `global` und `->` steht kein Leerzeichen. |
| `Call("Name", ...)`, `ObjectCall(obj, "Name", ...)`, `GameCall("Name", ...)` | Dynamische Aufrufe mit Namen zur Laufzeit. Nach dem konkreten Ziel, Fehlerverhalten und der Argumentbehandlung im API-Katalog suchen. |

Historischer Code kann die Form `obj->C4ID::Name()` enthalten. Der Parser von `v365` löst den Definitionsnamen auf, der Executor überspringt aber die Namensraum-Instruktion. Bei einer Reparatur zuerst prüfen, welche Implementierung tatsächlich läuft. [Parser](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L3170-L3199), [Executor](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L1229-L1237)

Viele Koordinatenparameter, etwa bei `CreateObject`, sind im Szenariokontext absolut und im Objektkontext Offsets zur Instanz. Das pro Funktion prüfen. Bei der Objektsuche in LegacyClonk nimmt `FindObject` die klassische Positionssignatur an. Für kombinierbare Kriterien verwendet man `FindObject2(Find_ID(ROCK), Find_Distance(30, 0, 0))` oder `FindObjects(...)` für ein Array. Die Kriterien sind teilweise Funktionen des geladenen `System.c4g`. Eine ergebnislose Einzelsuche vor einem Objektaufruf abfangen. Besitzer, Controller, Definition und Spieler sind unterschiedliche Angaben. `0` kann eine gültige Spielernummer sein. Für „kein Besitzer“ die Konstante `NO_OWNER` gemäß API verwenden. [SDK zu Aufrufen](https://crdocs.clonkspot.org/en/sdk/script/FuncCall.html), [Suchhelfer in v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/planet/System.c4g/FindObject.c), [API-Katalog](Reference/C4Script_API.md)

Callbacks werden von Engine, Skripten, Aktionen oder Effekten nach Namen aufgerufen. Signatur, Aufrufzeitpunkt, Empfänger und Rückgabewert vor dem Implementieren am Zielbuild prüfen. `Initialize()` im Szenario läuft am Rundenstart, `InitializePlayer(int player)` bei Spielerinitialisierung. Ein gleichnamiges `Initialize()` in einer Objektdefinition läuft für die Instanz. `InitializeDef(string section)` läuft im Definitionskontext. Aktionscallbacks stehen in `ActMap.txt`. In LegacyClonk verwendet ein mit `AddEffect` angelegter Effekt Funktionen wie `FxNameStart`, `FxNameTimer` und `FxNameStop`. Der Timer erhält Zielobjekt, Effektnummer und Zeit, Effektvariablen sind über `EffectVar(index, target, number)` erreichbar. Intervalle, Rückgabecodes, temporäre Stop-/Start-Aufrufe und Entfernung mit dem [SDK zu Effekten](https://crdocs.clonkspot.org/en/sdk/script/Effects.html) und den [Aufrufstellen in v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Effect.cpp) abgleichen. Für Aufgaben über mehrere Frames die im Paket vorhandenen Effekte oder `ScheduleCall` verwenden. Bei Mehrspieler gemeinsame Spielzustände nur über synchron ausgeführte Spielpfade ändern.

## Minimalbeispiel für ein Szenario

Das Beispiel gehört als `Script.c` in eine offene `.c4s`-Gruppe. Das Szenario muss über seine `Scenario.txt` ein Paket mit der Definition `ROCK` laden. Die feste Position muss im Test in der Landschaft liegen.

```c
#strict 3

func Initialize()
{
    var rock = CreateObject(ROCK, 50, 30, NO_OWNER);
    if (rock == nil)
    {
        Log("Beispiel: Stein konnte nicht erstellt werden.");
        return;
    }

    var entries = [rock];
    var stats = {count = CountObjectsInArray(entries)};
    Log("Beispiel: %d Objekt(e), Stein bei %d/%d", stats.count, GetX(rock), GetY(rock));
}

func CountObjectsInArray(array entries)
{
    var count = 0;
    for (var entry in entries)
    {
        if (entry != nil)
            ++count;
    }
    return count;
}
```

**Spieltest vom 9. November 2026.** Das Beispiel lief mit Build [365] win64 im lokalen Konsolenmodus und mit `Objects.c4d`. Der Linker meldete 0 Warnungen und 0 Fehler. `Clonk.log` meldete `Beispiel: 1 Objekt(e), Stein bei 50/27`. Die Logzeile zeigt die tatsächliche Objektposition nach der Platzierung. Die Arbeitskopie liegt unter `.work/c4script-example-c0939518/`.

Beim Lesen zuerst den Einstiegspunkt suchen, dann die beteiligten Dateien und den Kontext von `this` verfolgen. Bei jeder API den Typ des Ergebnisses und die Bedeutung fehlender Werte prüfen. Beim Schreiben den kleinsten ausführbaren Fall erstellen und im Zielbuild laden. In `Clonk.log` sowohl das Linkergebnis als auch die erwartete Aktion oder Logzeile prüfen. Danach die tatsächlich unterstützten Spielpfade einschließlich Fehlerfall und Aufräumen prüfen. [Anleitung zu Spieltests](Development.md#spieltest)
