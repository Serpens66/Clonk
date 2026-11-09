/*-- Kein Metall vor Gebäuden --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}


// Kann mittels des neuen Spielzielauswählers 
public func IsChooseable2() { return(1); }