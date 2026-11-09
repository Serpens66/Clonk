/*-- Suizid --*/

#strict

protected func Activate(int iPlr)
{
  Log("$MsgSuicide$", GetPlayerName(iPlr));
  Kill(GetCursor(iPlr));
  return(1);
}


// Kann mittels des neuen Spielzielauswählers 
public func IsChooseable2() { return(1); }