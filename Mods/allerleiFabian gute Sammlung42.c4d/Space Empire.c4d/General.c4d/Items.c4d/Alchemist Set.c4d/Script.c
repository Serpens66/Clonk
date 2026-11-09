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

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(1); }
