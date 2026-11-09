/*-- Gold --*/

#strict 3

/* Aufschlag */

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

/* Für Goldmine */

public func Sale(plr) {
  var obj;
  if (obj = FindObject (GLDM, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> GLDM::PlayerHasSold(plr, this ());
}

public func Purchase(int plr, object buyobj) {
  plr ??= 0;
  var obj;
  if (obj = FindObject (GLDM, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> GLDM::PlayerHasBought(plr, this ());
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(180); }
