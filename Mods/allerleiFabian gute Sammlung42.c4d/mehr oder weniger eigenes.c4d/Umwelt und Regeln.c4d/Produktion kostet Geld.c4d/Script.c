/*-- Baustoffe zurück --*/

#strict 3

// wird in Destruction Objekt von SpaceEmpire genereal geregelt.

func Initialize()
{
SetPosition(1,1);
}


func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }