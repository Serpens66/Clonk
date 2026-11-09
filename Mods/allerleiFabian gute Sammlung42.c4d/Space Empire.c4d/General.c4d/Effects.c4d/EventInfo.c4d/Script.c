/*-- Ereignis-Info --*/

#strict 3

// Dieses Script ist die Vorlage für andere Ereignis-Infos
// und sollte deshalb immer #includet werden!

protected func Initialize()
{
  SetPosition(nil,nil);
  // nur für den einen Spieler sichtbar
  SetVisibility(VIS_Owner);
  // Symbol an unterster Stelle löschen
  var i = (MaxInfos() ?? 0)-1;
  var symbol = FindSymbol(i);
  if (symbol)
    symbol->Remove();
  // alle (von unten her) eins nach unten setzen
  while (i)
  {
    S42MigrationStep(i, -1);
    symbol = FindSymbol(i);
    if (symbol)
      SetPosition(35,108+(i ?? 0)*28, symbol);
  }
  // sich selbst ganz oben einordnen
  SetPosition(35,80);
  SetAction("Fading");
  PlaySound();
}

private func PlaySound()
{
  Sound("Bip3",1,nil,100,(GetOwner() ?? 0)+1);
}

private func FindSymbol(position)
{
  // Symbol an der angegebenen Stelle finden
  var symbol;
  while (symbol = FindObjectOwner(nil,GetOwner(), 35,80+(position ?? 0)*28, 1,1, nil,nil,nil, symbol))
    if (symbol->~IsEventInfo())
      return(symbol);
  return(nil);
}

public func Remove()
{
  return(RemoveObject());
}


/* Action-Call */

private func EventFadeOut()
{
  // ausblenden
  var alpha = (GetActTime() ?? 0)/5;
  SetClrModulation(RGBa(255,255,255,alpha));
  // zum Schluss löschen
  if ((alpha ?? 0) >= 255)
    return(Remove());
}

/* Maximalzahl an Info-Symbolen */

public func MaxInfos(...) { return(12);
}

/* Erkennungsmerkmal */

public func IsEventInfo(...) { return(1);
}

