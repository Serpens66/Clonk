/*-- keine Lichter --*/

#strict 3

// solange diese Regel vorhanden, sind alle neuen Lichter automatisch turned off

func Initialize()
{
SetPosition(1,1);
Kontrolle();
}

func Kontrolle() // findet alle IsLight objekte außer LGFL (feuerlicht) und schaltet sie ab
{
var llight;
for (llight in FindObjects(Find_Func("IsLight"),Find_Not(Find_ID(LGFL))))
 {
 llight->~TurnOff();
 }
}

func Destruction()  // Lichter wieder sichtbar machen
{
var llight;
for (llight in FindObjects(Find_Func("IsLight"),Find_Not(Find_ID(LGFL))))
 {
 llight->~TurnOn();
 }
}

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsAntiLagChooseable() { return(1); }