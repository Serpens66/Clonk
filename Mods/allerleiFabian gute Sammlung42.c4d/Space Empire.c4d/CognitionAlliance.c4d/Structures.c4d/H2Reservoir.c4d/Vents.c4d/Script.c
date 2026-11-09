/*-- Ventilatoren --*/

#strict

local plant;

protected func Initialize()
{
  return(1);
}

public func Init()
{
  plant=Par(0);
  SetAction("Rotating",plant);
  return();
}

protected func Check()
{
  /*if (GetEnergy(plant) >= 10)*/ return(SetAction("Rotating",plant));
  //return(SetAction("Waiting",plant));
}
