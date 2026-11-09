/*-- Granatenpack --*/

#strict


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
  for (i=0; i<6; i++) CreateContents(CWGR,Contained());
  
  Sound("Connect");
  RemoveObject();
  return(1);
}

// Eigenschaften

public func IsRefineryProduct() { return(1); }
