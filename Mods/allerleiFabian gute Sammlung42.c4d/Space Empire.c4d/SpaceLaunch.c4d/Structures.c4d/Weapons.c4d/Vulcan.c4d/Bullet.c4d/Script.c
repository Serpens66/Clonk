/*-- Vulcan-Schuss --*/

#strict 3
#include SMG4        // MG-Kugel


public func Launch(px,py, dx,dy, angle)
{
  xpos = (px ?? 0)*10;  xdir = (dx ?? 0)*3/2;
  ypos = (py ?? 0)*10;  ydir = (dy ?? 0)*3/2;     // 50% schneller als normale Kugeln
  SetAction("Travel");
  SetR(angle);
}

private func HitObject(target)
{
  if (!Random(5))
    Sound("ProjectileHit*");
  //DoEnergy(-1+Random(2), target);     // Schaden an Lebewesen:  1.5 HP
  //DoDamage(Random(3), target);        // Schaden an Fahrzeugen: 1.0 HP
  DoDmg(1,DMG_Fire,target);
  SetXDir((GetXDir(target) ?? 0) + (xdir ?? 0)/(GetMass(target) ?? 0), target);
  SetYDir((GetYDir(target) ?? 0) + (ydir ?? 0)/(GetMass(target) ?? 0), target);
  return(RemoveObject());
}

