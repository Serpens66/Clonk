/*-- Gemeinschaftskonto --*/

#strict 3


// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }

/* Initialisierung */

protected func Initialize()
{
  // Spieler ohne Konto?
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    CreateObject(ACNT,nil, nil, GetPlayerByIndex(i) );
  }
}

public func InitializePlayer(iPlr)
{
  // Neuer Spieler, neues Konto
  var old; if (old=FindObjectOwner(ACNT, iPlr)) RemoveObject(old);
  CreateObject(ACNT, nil,nil, iPlr);
}

protected func Activate(iPlr)
{
  var ms=ObjectCall(FindObjectOwner(ACNT, iPlr), "GetTeam");
  if (ms) ms=Format("Du hast ein Gemeinschaftskonto mit: %s",ms);
  if (!ms) ms="Du hast kein Gemeinschaftskonto.";
  MessageWindow(ms, iPlr);
}

protected func Destruction()
{
  // Keine Teamaccounts, keine Konten
  // fixme: Sollte Gemeinschaftskonten vorher auflösen...
  if ((ObjectCount(GetID()) ?? 0)<=1) RemoveAll(ACNT);
  return(1);
}
