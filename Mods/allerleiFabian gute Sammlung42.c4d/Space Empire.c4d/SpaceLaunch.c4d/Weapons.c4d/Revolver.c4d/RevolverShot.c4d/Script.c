/*-- Revolverschuss --*/

#strict

local speed, ypos, alpha;


/* Wird abgefeuert */

public func Launch(direction, bigshot)
{
  if (bigshot) SetAction("BigShot");
          else SetAction("SmallShot");
  SetPhase(Random(7));
  alpha = 0;
  ypos = GetY();
  // Richtung setzen
  SetDir(direction);
  if (direction == DIR_Left()) speed = -120;
                          else speed = +120;
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var target;
  if (target = FindObject(0,-5,-5,11,11, OCF_Alive(),0,0,NoContainer()))
    return(HitObject(target));
  if (target = FindObject(0,1,0,0,0, OCF_Alive(),0,0,NoContainer()))
    return(HitObject(target));
  // Bewegung
  SetXDir(speed);
  SetYDir(0);
  SetPosition(GetX(),ypos);
  // ausblenden
  if (GetActTime() > 20)
  {
    alpha += 15;
    if (alpha >= 255)
      return(RemoveObject());
    SetClrModulation(RGBa(255,255,255,alpha));
  }
}


/* Treffer */

protected func HitObject(target)
{
  Sound("ProjectileHit*");
  var dmg = 3 + Random(3);
  if (GetAction() eq "BigShot")
    dmg = 10 + Random(6);
  DoEnergy(-dmg, target);
  DoDamage(+dmg, target);
  SetXDir(GetXDir(target) + speed/4, target);
  RemoveObject();
}

protected func Hit()
{
  if (GetAction() eq "BigShot") Explode(5);
                           else RemoveObject();
}

