/*--- Eisenerz ---*/

#strict 3

func Hit () {
  Sound("RockHit*");
}

/* Für Erzmine */

public func Sale(int plr) {
  plr ??= 0;
  var obj;
  if (obj = FindObject (OREM, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> OREM::PlayerHasSold(plr, this ());
  return(GetID());
}

public func Purchase(int plr, object buyobj) {
  plr ??= 0;
  var obj;
  if (obj = FindObject (OREM, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> OREM::PlayerHasBought(plr, this ());
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(200); }