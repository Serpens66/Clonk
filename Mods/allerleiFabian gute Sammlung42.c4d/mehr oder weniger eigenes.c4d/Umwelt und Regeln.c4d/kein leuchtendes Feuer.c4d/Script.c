/*-- kein leuchtendes Feuer --*/

#strict


func Initialize()
{
SetPosition(1,1);
Kontrolle();
}

func Kontrolle()
{
var llight;
for (llight in FindObjects(Find_ID(LGFL)))
 {
 llight->~TurnOff();
 }
}

func Destruction()  // Lichter wieder sichtbar machen
{
var llight;
for (llight in FindObjects(Find_ID(LGFL)))
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