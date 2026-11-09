/*-- Granate --*/

#strict


protected func Initialize()
{
  return(1);
}

public func Activate()
{
  SetAction("Counting");
  return(1);
}

protected func Hit()
{
  Sound("MetalHit1");
  return(1);
}

protected func Countdown()
{
  Sound("Bip");
  return(1);
}

protected func Detonation()
{
  Explode(20);
  return(1);
}
