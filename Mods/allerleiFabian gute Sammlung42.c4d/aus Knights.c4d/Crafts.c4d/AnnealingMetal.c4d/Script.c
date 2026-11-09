/*-- Glühendes Metall --*/

#strict 3

protected func Initialize()
{
  SetAction("Glowing");
}

private func GlowingProcess()
{
  // Effekte
  if (!Contained()) 
  {
    Smoke(nil, nil, 1);
    CastParticles("PxSpark", 3, 10, nil, nil, 15, 30, RGB(255,223,127), RGB(255,223,127));
  }
  // Abkühlung
  if ((GetActTime() ?? 0) > 2500)
    CoolConvert();
}

private func CoolConvert()
{
  ChangeDef(METL);
}
  
protected func Hit()
{
  Sound("MetalHit*");
}

private func FastCoolOff()
{
  Sound("Pshshsh");
  CoolConvert();
}
 
protected func SellTo() { return(METL); }
