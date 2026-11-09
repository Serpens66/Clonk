/*-- Leere Muschel --*/

#strict

protected func Hit()
{
  Sound("MetalHit*");
}

private func Perle()
{
  if(InLiquid() && !Random(5)) ChangeDef(SHEL);
}