#strict 3

public func Activate()
{

  if (!CreateObject(3ATW, 0, 12, GetOwner())) return;
  Sound("Connect");
  RemoveObject();
  return(1);
}

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

