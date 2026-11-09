/*-- Energieemitter --*/

#strict


local plant, switched;

protected func Initialize()
{
  switched=1;
  return(1);
}

public func Init()
{
  plant=Par(0);
  SetAction("Rotating",plant);
  return();
}

public func Switch()
{
  switched=Par(0);
  return(1);
}

protected func Check()
{
  if ((GetEnergy(plant) >= 10) && switched) return(SetAction("Rotating",plant));
  return(SetAction("Waiting",plant));
}
