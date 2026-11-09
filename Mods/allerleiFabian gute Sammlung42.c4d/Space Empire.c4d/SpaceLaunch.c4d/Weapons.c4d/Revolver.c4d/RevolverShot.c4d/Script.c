/*-- Revolverschuss --*/

#strict 3

local speed, ypos, alpha;


/* Wird abgefeuert */

public func Launch(direction, bigshot)
{
  if (bigshot) SetAction("BigShot");
          else SetAction("SmallShot");
  SetPhase(Random(7));
  alpha = nil;
  ypos = GetY();
  // Richtung setzen
  SetDir(direction);
  if (Equal(direction, DIR_Left)) speed = -120;
                          else speed = +120;
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var target;
  if (target = FindObject(nil,-5,-5,11,11, OCF_Alive,nil,nil,NoContainer()))
    return(HitObject(target));
  if (target = FindObject(nil,1,nil,nil,nil, OCF_Alive,nil,nil,NoContainer()))
    return(HitObject(target));
  // Bewegung
  SetXDir(speed);
  SetYDir(nil);
  SetPosition(GetX(),ypos);
  // ausblenden
  if ((GetActTime() ?? 0) > 20)
  {
    S42MigrationAssign(alpha, "+=", 15);
    if ((alpha ?? 0) >= 255)
      return(RemoveObject());
    SetClrModulation(RGBa(255,255,255,alpha));
  }
}


/* Treffer */

protected func HitObject(target)
{
  Sound("ProjectileHit*");
  var dmg = 3 + (Random(3) ?? 0);
  if (!!SEqual(GetAction(), "BigShot"))
    dmg = 10 + (Random(6) ?? 0);
  DoEnergy(-(dmg ?? 0), target);
  DoDamage(+(dmg ?? 0), target);
  SetXDir((GetXDir(target) ?? 0) + (speed ?? 0)/4, target);
  RemoveObject();
}

protected func Hit()
{
  if (!!SEqual(GetAction(), "BigShot")) Explode(5);
                           else RemoveObject();
}

