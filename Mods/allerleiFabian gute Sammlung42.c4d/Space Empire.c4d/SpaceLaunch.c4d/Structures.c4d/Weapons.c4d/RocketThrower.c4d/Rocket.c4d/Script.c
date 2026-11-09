/*-- Rakete --*/

#strict

local launcher, dirX, dirY;


/* Abschuss */

public func Launch(par_launcher, angle)
{
  SetAction("Travel");
  launcher = par_launcher;
  dirX = +Sin(angle, 200);
  dirY = -Cos(angle, 200);
  SetR(angle);
}


/* Fliegt */

protected func Travel()
{
  // Treffer-Überprüfung
  if (FindTarget() || (GetActTime() > 60+Random(10)))
    return(Hit());
  // Bewegung
  SetXDir(dirX);
  SetYDir(dirY);
  Smoke(0,0,10);
}

private func FindTarget()
{
  // Fahrzeuge
  var obj = FindObject(0, 1,1,0,0, OCF_Entrance(),0,0,NoContainer());
  if (GetCategory(obj) & C4D_Vehicle())
    if (Hostile(GetOwner(),GetOwner(obj)))
      return(obj);
  if(obj = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("IsBulletTarget"))) // alle bullettargets
     {if(!(GetOCF(obj) & OCF_Living && !(GetOCF(obj) & OCF_Alive))) {if(!FindObject(NOFF))  return(obj); if(FindObject(NOFF)) if(Hostile(GetOwner(obj), GetOwner())) return(obj);} }
  // Gebäude.. nicht mehr treffen
//  var obj = FindObject(0, 1,1,0,0, OCF_Exclusive(),0,0,NoContainer());
//  if (GetCategory(obj) & C4D_Structure())
//    if (Hostile(GetOwner(),GetOwner(obj)))
//      return(obj);
  return(0);
}


/* Zerstörung */

protected func Hit()
{
  Explode(20+Random(6));
}

public func Destroy()
{
  CastPXS("FlyAshes",25,25);
  Hit();
}

public IsProjectile: return(1);

