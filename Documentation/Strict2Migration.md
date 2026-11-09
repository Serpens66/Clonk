# C4Script von #strict auf #strict 2 umstellen

In LegacyClonk wählt `#strict` ohne Zahl Stufe 1. Für Stufe 2 steht `#strict 2` oben in der Skriptdatei. Dieser Leitfaden gilt für Build [365], Tag `v365`, Commit `31b2cde5d1d68a86765c68c8252184ca38469c0d`. Bei `#include` und `#appendto` behält jede Funktion die Stufe ihrer Ursprungsdatei. Dadurch lassen sich die Dateien einzeln umstellen. Eine Datei ohne Direktive startet in Stufe 0 und braucht eine zusätzliche Prüfung der Regeln für den Wechsel zu Stufe 1. [SDK zu #strict](https://crdocs.clonkspot.org/en/sdk/script/Strict.html), [Parser v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1491-L1506)

## Kurzschlussauswertung und Nebenwirkungen

In Stufe 1 werden bei `a && b` und `a || b` beide Operanden ausgewertet. In Stufe 2 überspringt `&&` den rechten Ausdruck, wenn links bereits falsch ist. `||` überspringt ihn, wenn links bereits wahr ist. Prüfe Aufrufe mit Nebenwirkungen, etwa Zuweisungen, Änderungen an Objekten und `Random()` in synchronisierten Spielabläufen. Ziehe eine weiterhin benötigte rechte Berechnung in eine eigene Anweisung **nach** der linken Berechnung. [Parser v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L3013-L3037), [Auswertung in Stufe 1](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L749-L764), [Kurzschluss in Stufe 2](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L1016-L1038)

Wenn das Ergebnis als Wert gespeichert oder zurückgegeben wird, prüfe zusätzlich seinen Typ und Inhalt. In Stufe 1 erzeugen `&&` und `||` einen booleschen Wert. In Stufe 2 bleibt der Operand auf dem Stapel, der das Ergebnis bestimmt. Beispielsweise liefert `2 && 7` in Stufe 2 den Wert `7`. `!!(a && b)` erzwingt einen booleschen Wert, behält aber die Kurzschlussauswertung bei. Das folgende Muster erhält sowohl beide Auswertungen als auch ein boolesches Ergebnis. [Auswertung in v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L749-L764), [Kurzschluss in v365](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L1016-L1038)

```c
var left = CheckCondition();
var right = UpdateState();
return !!(left && right);
```

## Vergleiche und Stringoperatoren

| Stelle in Stufe 1 | Verhalten und Anpassung für Stufe 2 |
| --- | --- |
| `a == b`, `a != b` | Stufe 1 vergleicht Rohwerte. Stufe 2 vergleicht Strings nach Text, Arrays nach Inhalt und Objekte weiterhin nach Identität. Zwei getrennte Arrays `[1, 2]` sind deshalb unter Stufe 2 gleich. Wenn die Logik unterschiedliche Container unterscheiden muss, vergib dafür eigene IDs. |
| `a eq b`, `a S= b`, `a ne b` | Verwende für zwei vorhandene Strings `a == b` bzw. `a != b`. Der alte Stringoperator behandelt einen fehlenden String wie `""`. Falls der Code das ausnutzt, normalisiere fehlende Werte ausdrücklich auf `""`, etwa mit `if (!a) a = "";`, bevor du vergleichst. |
| `GetIndexOf(value, items)` | Stufe 1 sucht nach Rohwerten, Stufe 2 verwendet die neue Gleichheit. Wenn gleichwertige Elemente mehrfach vorkommen, kann sich der gefundene Index ändern. |
| Vergleiche gemischter Typen | Stufe 2 lässt weiterhin manche Zahlen-, Bool- und ID-Vergleiche zwischen verschiedenen Typen zu. Prüfe den Typ bei Bedarf mit `GetType` und verwende für die fachliche Entscheidung Werte desselben Typs. |

`eq`, `ne` und `S=` sind in Stufe 2 keine Operatoren mehr. `S=42;` ist dann eine normale Zuweisung an eine deklarierte Variable `S`. Die explizite Unterscheidung von fehlendem Wert, numerischer `0` und `false` kommt erst mit Stufe 3. [Operatorerkennung](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L487-L509), [Gleichheitsregeln](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Value.cpp#L824-L915), [Alte Stringoperatoren](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulExec.cpp#L707-L724), [GetIndexOf](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4Script.cpp#L3985-L4012)

## Alte Syntax und Parserfehler

| Altes Muster | Anpassung |
| --- | --- |
| Funktionslabel `Name:` oder Funktionsdeklaration ohne öffnende geschweifte Klammer | Als `func Name(...) { ... }` schreiben. Bei `global`, `public`, `protected` und `private` den bisherigen Modifikator übernehmen. Eine vorhandene Funktionsbeschreibung für das Kontextmenü im neuen Rumpf direkt nach `{` erhalten. `Par(0)` und benannte Parameter anhand der Aufrufer zuordnen. |
| `return(a, b)`, `if (a, b)`, `while (a, b)` | Stufe 2 akzeptiert je einen Rückgabeausdruck bzw. eine Bedingung. Alte zusätzliche Ausdrücke konnten ausgeführt werden, obwohl nur der erste Wert verwendet wurde. Falls ihre Wirkung gebraucht wird, zuerst `a` auswerten und speichern, danach `b` ausführen und anschließend den gespeicherten Wert verwenden. Bei `while` diese Reihenfolge vor jeder Prüfung erhalten, auch nach `continue`. |
| `return()` oder `return` als Funktionsargument | Für eine wertlose Rückgabe `return;` verwenden. Einen verschachtelten vorzeitigen Rücksprung als eigene Anweisung an der entsprechenden Stelle formulieren. `return(value);` mit genau einem Ausdruck bleibt gültig. |
| `break` oder `continue` außerhalb einer Schleife | Die beabsichtigte Verzweigung oder Rückgabe ausdrücken. Wenn der alte Parser die Anweisung nach einer Warnung ignorierte, kann sie entfallen. |
| Sonderzeichen in Namen | ASCII-Buchstaben, Unterstrich und nach dem ersten Zeichen auch Ziffern verwenden. Alle Verweise mit umbenennen, einschließlich dynamischer Aufrufe mit Stringnamen, `ActMap.txt` und anderer Callbackangaben. |
| `CONSTANT()` mit einer als Konstante registrierten Bezeichnung | Die Konstante als `CONSTANT` lesen. Wenn ein gleichnamiger Funktionsaufruf beabsichtigt war, dessen Auflösung prüfen. |
| Fehlender Operand oder Namenskonflikt | Konstrukte wie `Foo(42+, 1)` explizit korrigieren. Konflikte von Funktionen, globalen Variablen und Konstanten auflösen. Benenne auch einen untypisierten Parameter um, dessen Name einem Typwort wie `int` entspricht. |

[Funktionsdeklaration](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L1660-L1767), [return und Schleifenkontrolle](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2076-L2158), [if und while](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2486-L2531), [Überschüssige Argumente](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2274-L2355), [Konstantenaufruf](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L2848-L2871), [Fehlender Operand](https://github.com/legacyclonk/LegacyClonk/blob/v365/src/C4AulParse.cpp#L3040-L3068)

## Vorgehen und Spieltest

1. Ausgangsstufe, Zielbuild, betroffene Skripte, ihre Aufrufer und die geladenen Pakete festhalten. Warnungen im alten `Clonk.log` als Suchhinweise verwenden. Auch Ausdrücke in Strings prüfen, die über `eval` oder zeitversetzt ausgeführt werden.
2. In einer Arbeitskopie Syntax und die beabsichtigten Nebenwirkungen korrigieren. Dann die Direktive setzen und die Datei in ihrem normalen Szenario laden. Kodierung, Zeilenenden, IDs und Callbackverträge beibehalten.
3. Im Zielbuild Alt- und Neufassung mit denselben Eingaben vergleichen. Rechte Operanden auf beiden logischen Zweigen, leere bzw. fehlende Strings, gleiche Inhalte in getrennten Arrays und Treffer von `GetIndexOf` gezielt prüfen. Bei Spielständen oder Mehrspieler die tatsächlich unterstützten Pfade nachstellen. [Anleitung zu Spieltests](Development.md#spieltest)

**Spieltest vom 9. November 2026.** `Strict2Probe.c4s` lief unter Build [365] win64 im lokalen Konsolenmodus mit `Objects.c4d` und einem lokalen Strict-1-Systemskript. Der Linker meldete 0 Warnungen und 0 Fehler. Zwei rechts stehende Aufrufe liefen unter Stufe 1 und wurden unter Stufe 2 beide übersprungen. Das alte `return(7, S2ProbeHit());` führte den zweiten Ausdruck aus und gab 7 zurück. Der Vergleich zweier getrennt erzeugter Arrays mit gleichem Inhalt ergab in Stufe 2 wahr. `GetIndexOf` fand dort das inhaltsgleiche Array an Index 0. `0 eq ""` war in Stufe 1 wahr, `0 == ""` in Stufe 2 falsch. Skripte und Log liegen unter `.work/strict2-probe-da3e7785/`.

Der nächste Schritt für ein LegacyClonk-spezifisches Skript steht im [Migrationsleitfaden von #strict 2 auf #strict 3](Strict3Migration.md). Syntax und API-Signaturen stehen in der [Sprachreferenz](C4Script.md) und im [API-Katalog](Reference/C4Script_API.md).
