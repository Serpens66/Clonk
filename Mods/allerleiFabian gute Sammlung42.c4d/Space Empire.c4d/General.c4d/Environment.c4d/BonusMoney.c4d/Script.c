/*-- Geldbonus --*/

#strict

local strength;


/* Initialisierung */

protected func Initialize()
{
  // andere Objekte dieses Typs finden
  strength = 1;
  var obj = FindObject(GetID());
  if (obj)
  {
    // Zähler bei anderem Objekt erhöhen
    obj->IncStrength();
    return(RemoveObject());
  }
}

public func IncStrength()
{
  ++strength;
}


/* Geldbonus geben */

protected func DoBonus()
{
  // für jeden Spieler
  var i = GetPlayerCount();
  while (i) 
  {
    --i;
    if (!GetPlayerName(i)) continue;
    // Kontostand erhöhen
   if (GetWealth(i)<9800)
{
    DoWealth(i, +strength);
    DoScore(i, -strength);
}
  }
}

