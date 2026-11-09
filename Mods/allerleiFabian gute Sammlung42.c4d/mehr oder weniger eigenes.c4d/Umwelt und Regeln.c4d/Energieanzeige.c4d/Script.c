/*-- Energie Anzeige --*/

#strict


func Initialize()
{
SetPosition(1,1);
}

// wird in den objekten direkt gemacht, wenn diese regel gefunden wird

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }