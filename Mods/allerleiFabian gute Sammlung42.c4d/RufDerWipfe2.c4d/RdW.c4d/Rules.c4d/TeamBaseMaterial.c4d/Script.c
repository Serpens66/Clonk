/*-- Gemeinschafts Basismaterial --*/

#strict 3

/* Initialisierung */

protected func Initialize()
{
  // Spieler ohne Konto?
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    CreateObject(BACN,nil, nil, GetPlayerByIndex(i) );
  }
}

public func InitializePlayer(iPlr)
{
  // Neuer Spieler, neues Konto
  var old; if (old=FindObjectOwner(BACN, iPlr)) RemoveObject(old);
  CreateObject(BACN, nil,nil, iPlr);
}

protected func Activate(iPlr)
{
  var ms=ObjectCall(FindObjectOwner(BACN, iPlr), "GetTeam");
  if (ms) ms=Format("Du teils dir dein kaufbares Material mit: %s",ms);
  if (!ms) ms="Du teils dir dein kaufbares Material nicht.";
  MessageWindow(ms, iPlr);
}

protected func Destruction()
{
  // Keine Teamaccounts, keine Konten
  // fixme: Sollte Gemeinschaftskonten vorher auflösen...
  if ((ObjectCount(GetID()) ?? 0)<=1) RemoveAll(BACN);
  return(1);
}

