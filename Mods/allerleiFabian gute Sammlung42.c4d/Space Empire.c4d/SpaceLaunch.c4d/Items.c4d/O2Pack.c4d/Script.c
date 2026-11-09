/*-- Sauerstoff-Pack (voll) --*/

#strict 3

protected func Hit(...) {
  Sound("RockHit");
  return(1);
}

protected func CheckBreath(...) {
  var obj = Contained();
  if (!obj) return(1);
  if (!((GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))) return(1);
  if ((GetBreath(obj) ?? 0) > 30) return(1);
  DoBreath(500, Contained());
  Sound("Breath");
  ChangeDef(SO2E);
  return(1);
}

public func IsRefineryProduct() { return(1); }
