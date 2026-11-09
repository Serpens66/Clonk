#strict

public func Activate()
{

  CreateObject(3ATW,0,12,GetOwner()); Sound("Connect"); RemoveObject();
  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

