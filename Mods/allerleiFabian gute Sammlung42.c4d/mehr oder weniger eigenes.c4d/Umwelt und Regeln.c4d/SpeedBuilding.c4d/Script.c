/*-- Schnelle Produktion --*/

#strict

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }

// wird in clonkskript (building) geregelt
