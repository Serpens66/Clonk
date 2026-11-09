/*-- Sauerstoff-Pack (leer) --*/

#strict 3

protected func Hit(...) {
  Sound("RockHit");
  return(1);
}

protected func CheckAir(...) {
  // nur an der Luft
  if (GBackLiquid())    return(1);
  if (GBackSemiSolid()) return(1);
  if (GBackSolid())     return(1);
  // in volles Pack umwandeln
  Sound("Heal");
  ChangeDef(SO2P);
  return(1);
}

/* Verkaufen als volles O2-Pack */

public func SellTo(...) { return(SO2P);
}