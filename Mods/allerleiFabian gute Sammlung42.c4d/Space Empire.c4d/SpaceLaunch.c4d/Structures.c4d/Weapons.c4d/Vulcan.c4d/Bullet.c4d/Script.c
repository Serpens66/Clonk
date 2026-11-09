/*-- Vulcan-Schuss --*/

#strict
#include SMG4        // MG-Kugel


public func Launch(px,py, dx,dy, angle)
{
  xpos = px*10;  xdir = dx*3/2;
  ypos = py*10;  ydir = dy*3/2;     // 50% schneller als normale Kugeln
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
  SetXDir(GetXDir(target) + xdir/GetMass(target), target);
  SetYDir(GetYDir(target) + ydir/GetMass(target), target);
  return(RemoveObject());
}

