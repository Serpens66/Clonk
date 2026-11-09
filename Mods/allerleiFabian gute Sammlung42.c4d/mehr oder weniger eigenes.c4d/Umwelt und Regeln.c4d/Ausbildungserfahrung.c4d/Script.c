/*-- Angepasste Physics --*/

#strict 3


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

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
//public func IsChooseable2() { return(1); }   // geht nicht beides gleichzeiti ( muesste sonst in chooser geändert werden). Also sollte in ausbildungscamp entfernt werden, damits nur noch variabel ist