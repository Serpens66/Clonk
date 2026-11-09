/*-- keine abgase --*/

#strict 3


func Initialize()
{
SetPosition(1,1);
}

// wird durch prufen auf vorhandensein in verbrennungsobjekt geregelt. VBWS

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }