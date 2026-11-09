/*-- Geldbonus --*/

#strict 3

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
  S42MigrationStep(strength, 1);
}


/* Geldbonus geben */

protected func DoBonus()
{
  // für jeden Spieler
  var i = GetPlayerCount();
  while (i) 
  {
    S42MigrationStep(i, -1);
    if (!GetPlayerName(i)) continue;
    // Kontostand erhöhen
   if ((GetWealth(i) ?? 0)<9800)
{
    DoWealth(i, +(strength ?? 0));
    DoScore(i, -(strength ?? 0));
}
  }
}

