/*-- Ammo-Objekt --*/

#strict

local combi, launcher;


/* wird abgefeuert */

public func Launch(par_combi, clonk)
{
  SetAction("Travel");
  SetPlrView(GetOwner(),this());
  if (par_combi)
    combi = par_combi;
  launcher = clonk;
  return(1);
}

/* Drehung je nach Bewegung neu setzen */

private SetDirection:
  SetR(Angle(GetXDir(),GetYDir()));
  return(1);

/* Ziel finden */

private FindTarget:
  // Lebewesen finden
  var target = FindObject(0,-5,-5,11,11, OCF_Alive(),0,0,NoContainer());
  if (!target)
    target = FindObject(0,1,0,0,0, OCF_Alive(),0,0,NoContainer());
  // der Bazooka-Clonk
  if (target == launcher) return(0);
  return(target);

private FindVehicle:
  // Fahrzeuge sind auch gute Ziele
  var target;
  if (target = FindObject(SHTL, 0,1,0,0, 0,0,0,NoContainer()))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  if (target = FindObject(CVLA, 0,1,0,0, 0,0,0,NoContainer()))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  if (target = FindObject(TANK, 0,1,0,0, 0,0,0,NoContainer()))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  if (target = FindObject2(Find_Func("IsHazardShip"),Find_Distance(10)))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  return(0);

