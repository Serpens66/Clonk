/*-- keine Jahreszeiten --*/

#strict


func Initialize()
{
SetPosition(1,1);
}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsAntiLagChooseable() { return(1); }