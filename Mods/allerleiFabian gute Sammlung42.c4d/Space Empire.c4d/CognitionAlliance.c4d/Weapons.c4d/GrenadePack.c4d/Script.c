/*-- Granatenpack --*/

#strict 3


protected func Initialize()
{
  return(1);
}

protected func Hit()
{
  Sound("RockHit1");
  return(1);
}

protected func Damage()
{
  CastObjects(CWGR,6,25);
  RemoveObject();
  return(1);
}

protected func Activate()
{
  var i;
  for (i=nil; (i ?? 0)<6; S42MigrationStep(i, 1, true)) CreateContents(CWGR,Contained());
  
  Sound("Connect");
  RemoveObject();
  return(1);
}

// Eigenschaften

public func IsRefineryProduct() { return(1); }
