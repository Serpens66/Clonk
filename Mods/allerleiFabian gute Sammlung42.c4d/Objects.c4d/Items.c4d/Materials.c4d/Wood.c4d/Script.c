/*--- Holz ---*/

#strict 3

protected func Hit()
{
  Sound("WoodHit*");
  return(1);
}

func Incineration()
{
  SetClrModulation (RGB(48, 32, 32));
}
