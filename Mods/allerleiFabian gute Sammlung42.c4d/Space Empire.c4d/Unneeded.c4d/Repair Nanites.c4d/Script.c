/*-- RepairPack --*/

#strict

protected Hit:
  Sound("RockHit*");
  return(1);

public Activate:
  [Aktivieren]
  var num = 11;
  while(--num) CreateObject(SBOT,Random(11)-5,Random(11)-5,-1);
  RemoveObject();
  return(1);

public func IsRefineryProduct() { return(1); }