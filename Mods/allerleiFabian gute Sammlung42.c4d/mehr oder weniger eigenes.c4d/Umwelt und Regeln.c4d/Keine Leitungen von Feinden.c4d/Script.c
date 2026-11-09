/*-- Keine Leitungen von Feinden --*/

#strict 3


func Initialize()
{
SetPosition(1,1);
}

// wird direkt in Leitungsbausatz Skript geregelt.

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }