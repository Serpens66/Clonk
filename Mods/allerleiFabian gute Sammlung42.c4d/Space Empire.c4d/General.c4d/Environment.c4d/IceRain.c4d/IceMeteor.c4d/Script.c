/*-- Eisbrocken --*/

#strict

protected func Initialize()
{
  SetAction("Fall");
}

private func Effects()
{
  CastParticles("PxSpark", 3,40, 0,0, 40,60, RGBa(0,0,255,128), RGBa(0,255,255,0));
  CreateParticle("IceTrace", 0,0, 0,0, 60, RGBa(255,255,255,0));
}

protected func Hit()
{
  CastPXS("Ice",50,50);
  Sound("MeteorHit");
  RemoveObject();
}

public func Destroy()
{
  CastPXS("Snow",40,50);
  Sound("MeteorHit");
  RemoveObject();
}

public IsProjectile: return(1);

