/*-- Suizid --*/

#strict 3

protected func Activate(int iPlr)
{
  iPlr ??= 0;
  Log("$MsgSuicide$", GetPlayerName(iPlr));
  Kill(GetCursor(iPlr));
  return(1);
}


// Kann mittels des neuen Spielzielauswählers 
public func IsChooseable2() { return(1); }