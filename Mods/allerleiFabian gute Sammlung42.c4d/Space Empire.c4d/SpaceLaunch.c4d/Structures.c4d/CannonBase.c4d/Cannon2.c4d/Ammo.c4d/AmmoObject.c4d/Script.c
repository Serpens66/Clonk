/*-- Ammo-Objekt --*/

#strict 3

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

private func SetDirection(...) {
  SetR(Angle(GetXDir(),GetYDir()));
  return(1);
}

/* Ziel finden */

private func FindTarget(...) {
  // Lebewesen finden
  var target = FindObject(nil,-5,-5,11,11, OCF_Alive,nil,nil,NoContainer());
  if (!target)
    target = FindObject(nil,1,nil,nil,nil, OCF_Alive,nil,nil,NoContainer());
  // der Bazooka-Clonk
  if (Equal(target, launcher)) return(nil);
  return(target);
}

private func FindVehicle(...) {
  // Fahrzeuge sind auch gute Ziele
  var target;
  if (target = FindObject(SHTL, nil,1,nil,nil, nil,nil,nil,NoContainer()))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  if (target = FindObject(CVLA, nil,1,nil,nil, nil,nil,nil,NoContainer()))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  if (target = FindObject(TANK, nil,1,nil,nil, nil,nil,nil,NoContainer()))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  if (target = FindObject2(Find_Func("IsHazardShip"),Find_Distance(10)))
    if (Hostile(GetOwner(target), GetOwner()))
      return(target);
  return(nil);
}

