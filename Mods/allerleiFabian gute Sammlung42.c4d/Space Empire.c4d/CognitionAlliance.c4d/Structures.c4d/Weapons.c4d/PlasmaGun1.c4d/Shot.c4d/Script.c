/*-- Plasma-Schuss --*/

#strict

local xpos, ypos;
local xdir, ydir;


/* wird abgefeuert */

public func Launch(px,py, dx,dy, angle)
{
  xpos = px*10;  xdir = dx;
  ypos = py*10;  ydir = dy;
  SetAction("Travel");
  SetR(angle);
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(0,1,0,0,0, OCF_Alive(),0,0,NoContainer()))       // Lebewesen
      if (Hostile(GetOwner(obj), GetOwner()) || (GetOwner(obj)==-1)) 
   return(HitObject(obj));
  if (obj = FindObject(0,1,0,0,0, OCF_Entrance(),0,0,NoContainer()))    // Fahrzeuge
    if (GetCategory(obj) & C4D_Vehicle())
      if (Hostile(GetOwner(obj), GetOwner()))
      return(HitObject(obj));
  if(obj = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("IsBulletTarget"))) // alle bullettargets
     {if(!(GetOCF(obj) & OCF_Living && !(GetOCF(obj) & OCF_Alive))) { if(!FindObject(NOFF))  return(HitObject(obj)); if(FindObject(NOFF)) if(Hostile(GetOwner(obj), GetOwner())) return(HitObject(obj)); }}
  // Bewegung
  xpos += xdir;
  ypos += ydir;
  SetPosition((xpos+5)/10, (ypos+5)/10);
  // Aufprall auf Landschaft
  if (GBackSolid())
  {
    CastParticles("BulletSpark", 2,40, 0,0, 10,10, RGB(255,0,0), RGB(255,192,128));
    return(RemoveObject());
  }
  // zufälliges Verschwinden
  if (GetActTime() < 50) return(1);
  if (!Random(80-GetActTime()))
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
  SetXDir(GetXDir(target) + xdir/GetMass(target), target);
  SetYDir(GetYDir(target) + ydir/GetMass(target), target);
  return(RemoveObject());
}

