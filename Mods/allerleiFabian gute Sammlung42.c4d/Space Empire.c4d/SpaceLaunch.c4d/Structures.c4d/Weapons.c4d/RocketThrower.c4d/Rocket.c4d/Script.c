/*-- Rakete --*/

#strict 3

local launcher, dirX, dirY;


/* Abschuss */

public func Launch(par_launcher, angle)
{
  SetAction("Travel");
  launcher = par_launcher;
  dirX = +(Sin(angle, 200) ?? 0);
  dirY = -(Cos(angle, 200) ?? 0);
  SetR(angle);
}


/* Fliegt */

protected func Travel()
{
  // Treffer-Überprüfung
  if (Or(FindTarget(), ((GetActTime() ?? 0) > 60+(Random(10) ?? 0))))
    return(Hit());
  // Bewegung
  SetXDir(dirX);
  SetYDir(dirY);
  Smoke(nil,nil,10);
}

private func FindTarget()
{
  // Fahrzeuge
  var obj = FindObject(nil, 1,1,nil,nil, OCF_Entrance,nil,nil,NoContainer());
  if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
    if (Hostile(GetOwner(),GetOwner(obj)))
      return(obj);
  if(obj = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("IsBulletTarget"))) // alle bullettargets
     {if(!(And((GetOCF(obj) ?? 0) & (OCF_Living ?? 0), !((GetOCF(obj) ?? 0) & (OCF_Alive ?? 0))))) {if(!FindObject(NOFF))  return(obj); if(FindObject(NOFF)) if(Hostile(GetOwner(obj), GetOwner())) return(obj);} }
  // Gebäude.. nicht mehr treffen
//  var obj = FindObject(0, 1,1,0,0, OCF_Exclusive(),0,0,NoContainer());
//  if (GetCategory(obj) & C4D_Structure())
//    if (Hostile(GetOwner(),GetOwner(obj)))
//      return(obj);
  return(nil);
}


/* Zerstörung */

protected func Hit()
{
  Explode(20+(Random(6) ?? 0));
}

public func Destroy()
{
  CastPXS("FlyAshes",25,25);
  Hit();
}

public func IsProjectile(...) { return(1);
}

