/*-- Schnelle Produktion --*/

#strict 3

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}

// Kann mittels des neuen Spielzielauswählers variabel eingestellt werden
public func IsVariabelChooseable2() { return(1); }

// wird in den Fabrik skripten direkt geregelt (space empire general structures factory object) und anvil selbst
