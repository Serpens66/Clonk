/*--- Der Stein ---*/

#strict 3

protected func Hit()
{
  Sound("RockHit*");
  return(1);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(160); }