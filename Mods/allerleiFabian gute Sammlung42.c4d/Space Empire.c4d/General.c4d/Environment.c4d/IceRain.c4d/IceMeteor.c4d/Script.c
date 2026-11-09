/*-- Eisbrocken --*/

#strict 3

protected func Initialize()
{
  SetAction("Fall");
}

private func Effects()
{
  CastParticles("PxSpark", 3,40, nil,nil, 40,60, RGBa(nil,nil,255,128), RGBa(nil,255,255,nil));
  CreateParticle("IceTrace", nil,nil, nil,nil, 60, RGBa(255,255,255,nil));
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

public func IsProjectile(...) { return(1);
}

