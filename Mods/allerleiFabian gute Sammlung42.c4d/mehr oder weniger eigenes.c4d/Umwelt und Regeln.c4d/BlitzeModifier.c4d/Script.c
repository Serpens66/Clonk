/*-- BlitzeModifier --*/

#strict 3


protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition(1,1);
  return(1);
}

// Hilfsobjekt. Eigentlich nur Symbol fuer die BlitzeModifier Einstellung im Chooser.

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}
// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }