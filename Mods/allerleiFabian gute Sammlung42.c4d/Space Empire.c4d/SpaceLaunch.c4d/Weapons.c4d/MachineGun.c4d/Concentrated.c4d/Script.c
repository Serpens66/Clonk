/*-- Konzentrierter Schuss (Ammo 3) --*/

#strict 3

local xdir;


/* Wird abgefeuert */

public func Launch(dx)
{
  if (Equal(dx, DIR_Left))  xdir = -100;
  if (Equal(dx, DIR_Right)) xdir = +100;
  SetAction("Travel");
  return(1);
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Alive,nil,nil,NoContainer()))      // Lebewesen
    if (Or(Hostile(GetOwner(obj), GetOwner()), (Equal(GetOwner(obj), -1))))
      return(Hit());
  if (obj = FindObject(nil,1,nil,nil,nil, OCF_Entrance,nil,nil,NoContainer()))   // Fahrzeuge
    if ((GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0))
      if (Hostile(GetOwner(obj), GetOwner()))
        return(Hit());
  // Bewegung
  SetXDir(xdir);
  SetYDir(nil);
  // zufällige Explosion
  if ((GetActTime() ?? 0) < 50) return(1);
  if (!Random(80-(GetActTime() ?? 0)))
    return(Hit());
  return(1);
}


/* Treffer */

protected func Hit()
{
  var frag, i;
  for (i = 1; (i ?? 0) < 5; S42MigrationStep(i, 1))
  {
    frag = CreateObject(SMG0, (xdir ?? 0)/15*(i ?? 0), nil);
    ObjectCall(frag,"SetCounter",i);
  }
  Explode(4);
  return(1);
}

