/*-- Ereignis-Info --*/

#strict

// Dieses Script ist die Vorlage für andere Ereignis-Infos
// und sollte deshalb immer #includet werden!

protected func Initialize()
{
  SetPosition(0,0);
  // nur für den einen Spieler sichtbar
  SetVisibility(VIS_Owner());
  // Symbol an unterster Stelle löschen
  var i = MaxInfos()-1;
  var symbol = FindSymbol(i);
  if (symbol)
    symbol->Remove();
  // alle (von unten her) eins nach unten setzen
  while (i)
  {
    --i;
    symbol = FindSymbol(i);
    if (symbol)
      SetPosition(35,108+i*28, symbol);
  }
  // sich selbst ganz oben einordnen
  SetPosition(35,80);
  SetAction("Fading");
  PlaySound();
}

private func PlaySound()
{
  Sound("Bip3",1,0,100,GetOwner()+1);
}

private func FindSymbol(position)
{
  // Symbol an der angegebenen Stelle finden
  var symbol;
  while (symbol = FindObjectOwner(0,GetOwner(), 35,80+position*28, 1,1, 0,0,0, symbol))
    if (symbol->~IsEventInfo())
      return(symbol);
  return(0);
}

public func Remove()
{
  return(RemoveObject());
}


/* Action-Call */

private func EventFadeOut()
{
  // ausblenden
  var alpha = GetActTime()/5;
  SetClrModulation(RGBa(255,255,255,alpha));
  // zum Schluss löschen
  if (alpha >= 255)
    return(Remove());
}

/* Maximalzahl an Info-Symbolen */

public MaxInfos: return(12);

/* Erkennungsmerkmal */

public IsEventInfo: return(1);

