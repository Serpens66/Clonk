/*--- Schwefel ---*/

#strict 3

protected func Hit()
{
  Sound("WoodHit*");
  return(1);
}

func Incineration()
{
  SetClrModulation (RGB(96,64,64));
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(60); }
