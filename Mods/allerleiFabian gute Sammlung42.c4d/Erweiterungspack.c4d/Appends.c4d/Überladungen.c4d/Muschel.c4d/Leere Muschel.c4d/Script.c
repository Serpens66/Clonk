/*-- Leere Muschel --*/

#strict 3

protected func Hit()
{
  Sound("MetalHit*");
}

private func Perle()
{
  if(And(InLiquid(), !Random(5))) ChangeDef(SHEL);
}