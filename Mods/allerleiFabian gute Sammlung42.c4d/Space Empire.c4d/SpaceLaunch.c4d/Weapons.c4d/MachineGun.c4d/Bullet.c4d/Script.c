/*-- Normaler Schuss (Ammo 1 & 2) --*/

#strict

local xdir, ydir;

/* Wird abgefeuert */

public func Launch(dx, dy)
{
  if (dx == DIR_Left())  xdir = -100;
  if (dx == DIR_Right()) xdir = +100;
  ydir = dy;
  SetAction("Travel");
  return(1);
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(0,1,0,0,0, OCF_Alive(),0,0,NoContainer()))      // Lebewesen
    if (Hostile(GetOwner(obj), GetOwner()) || (GetOwner(obj)==-1))
      return(HitObject(obj));
  if (obj = FindObject(0,1,0,0,0, OCF_Entrance(),0,0,NoContainer()))   // Fahrzeuge
    if (GetCategory(obj) & C4D_Vehicle())
      if (Hostile(GetOwner(obj), GetOwner()))
        return(HitObject(obj));
  // Bewegung
  SetXDir(xdir);
  SetYDir(ydir);
  // zufälliges Verschwinden
  if (GetActTime() < 30) return(1);
  if (!Random(50-GetActTime()))
    return(RemoveObject());
  return(1);
}


/* Treffer */

private func HitObject(target)
{
  Sound("ProjectileHit*");
  DoEnergy(-Random(4)-1, target);
  DoDamage(+Random(2)+2, target);
  SetXDir(GetXDir(target) + xdir*6/GetMass(target), target);
  SetYDir(GetYDir(target) + ydir*6/GetMass(target), target);
  return(RemoveObject());
}

protected func Hit()
{
  CastParticles("BulletSpark", 2,40, 0,0, 10,10, RGB(255,0,0), RGB(255,192,128));
  return(RemoveObject());
}

