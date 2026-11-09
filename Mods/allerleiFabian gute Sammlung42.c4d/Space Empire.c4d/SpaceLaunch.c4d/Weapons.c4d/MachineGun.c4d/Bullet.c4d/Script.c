/*-- Normaler Schuss (Ammo 1 & 2) --*/

#strict 3

local xdir, ydir;

/* Wird abgefeuert */

public func Launch(dx, dy)
{
  if (Equal(dx, DIR_Left))  xdir = -100;
  if (Equal(dx, DIR_Right)) xdir = +100;
  ydir = dy;
  SetAction("Travel");
  return(1);
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Alive,nil,nil,NoContainer()))      // Lebewesen
    if (Or(Hostile(GetOwner(obj), GetOwner()), (Equal(GetOwner(obj), -1))))
      return(HitObject(obj));
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Entrance,nil,nil,NoContainer()))   // Fahrzeuge
    if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
      if (Hostile(GetOwner(obj), GetOwner()))
        return(HitObject(obj));
  // Bewegung
  SetXDir(xdir);
  SetYDir(ydir);
  // zufälliges Verschwinden
  if ((GetActTime() ?? 0) < 30) return(1);
  if (!Random(50-(GetActTime() ?? 0)))
    return(RemoveObject());
  return(1);
}


/* Treffer */

private func HitObject(target)
{
  Sound("ProjectileHit*");
  DoEnergy(-(Random(4) ?? 0)-1, target);
  DoDamage((+(Random(2) ?? 0) ?? 0)+2, target);
  SetXDir((GetXDir(target) ?? 0) + (xdir ?? 0)*6/(GetMass(target) ?? 0), target);
  SetYDir((GetYDir(target) ?? 0) + (ydir ?? 0)*6/(GetMass(target) ?? 0), target);
  return(RemoveObject());
}

protected func Hit()
{
  CastParticles("BulletSpark", 2,40, nil,nil, 10,10, RGB(255,nil,nil), RGB(255,192,128));
  return(RemoveObject());
}

