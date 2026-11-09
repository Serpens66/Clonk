/*-- kein rauch --*/

#strict 3


func Initialize()
{
SetPosition(1,1);
}

// wird durch system script und anpassung der globalen smoke fkt geregelt

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsAntiLagChooseable() { return(1); }