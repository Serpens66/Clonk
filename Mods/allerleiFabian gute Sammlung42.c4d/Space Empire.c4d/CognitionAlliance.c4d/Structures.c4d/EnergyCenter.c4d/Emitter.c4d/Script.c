/*-- Energieemitter --*/

#strict 3


local plant, switched;

protected func Initialize()
{
  switched=1;
  return(1);
}

public func Init()
{
  plant=Par(nil);
  SetAction("Rotating",plant);
  return;
}

public func Switch()
{
  switched=Par(nil);
  return(1);
}

protected func Check()
{
  if (And(((GetEnergy(plant) ?? 0) >= 10), switched)) return(SetAction("Rotating",plant));
  return(SetAction("Waiting",plant));
}
