/*-- RepairPack --*/

#strict 3

protected func Hit(...) {
  Sound("RockHit*");
  return(1);
}

public func Activate(...) {
  [Aktivieren]
  var num = 11;
  while(S42MigrationStep(num, -1)) CreateObject(SBOT,(Random(11) ?? 0)-5,(Random(11) ?? 0)-5,-1);
  RemoveObject();
  return(1);
}

public func IsRefineryProduct() { return(1); }