/*-- Plasma-Schuss --*/

#strict 3

local xpos, ypos;
local xdir, ydir;


/* wird abgefeuert */

public func Launch(px,py, dx,dy, angle)
{
  xpos = (px ?? 0)*10;  xdir = dx;
  ypos = (py ?? 0)*10;  ydir = dy;
  SetAction("Travel");
  SetR(angle);
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Alive,nil,nil,NoContainer()))       // Lebewesen
      if (Or(Hostile(GetOwner(obj), GetOwner()), (Equal(GetOwner(obj), -1))))
   return(HitObject(obj));
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Entrance,nil,nil,NoContainer()))    // Fahrzeuge
    if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
      if (Hostile(GetOwner(obj), GetOwner()))
      return(HitObject(obj));
  if(obj = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("IsBulletTarget"))) // alle bullettargets
     {if(!(And((GetOCF(obj) ?? 0) & (OCF_Living ?? 0), !((GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))))) { if(!FindObject(NOFF))  return(HitObject(obj)); if(FindObject(NOFF)) if(Hostile(GetOwner(obj), GetOwner())) return(HitObject(obj)); }}
  // Bewegung
  S42MigrationAssign(xpos, "+=", (xdir ?? 0));
  S42MigrationAssign(ypos, "+=", (ydir ?? 0));
  SetPosition(((xpos ?? 0)+5)/10, ((ypos ?? 0)+5)/10);
  // Aufprall auf Landschaft
  if (GBackSolid())
  {
    CastParticles("BulletSpark", 2,40, nil,nil, 10,10, RGB(255,nil,nil), RGB(255,192,128));
    return(RemoveObject());
  }
  // zufälliges Verschwinden
  if ((GetActTime() ?? 0) < 50) return(1);
  if (!Random(80-(GetActTime() ?? 0)))
    return(RemoveObject());
  return(1);
}


/* Treffer */

private func HitObject(target)
{
  if (!Random(5))
    Sound("ProjectileHit*");
  DoDmg(2,DMG_Bio,target);
  //DoEnergy(-2+Random(5)/4, target);     // Schaden an Lebewesen:  20.8 HP
  //DoDamage(Random(2)+1, target);          // Schaden an Fahrzeugen: 20.5 HP
  SetXDir((GetXDir(target) ?? 0) + (xdir ?? 0)/(GetMass(target) ?? 0), target);
  SetYDir((GetYDir(target) ?? 0) + (ydir ?? 0)/(GetMass(target) ?? 0), target);
  return(RemoveObject());
}

