/*-- Konzentrierter Schuss (Ammo 3) --*/

#strict

local xdir;


/* Wird abgefeuert */

public func Launch(dx)
{
  if (dx == DIR_Left())  xdir = -100;
  if (dx == DIR_Right()) xdir = +100;
  SetAction("Travel");
  return(1);
}


/* Fliegt */

private func Travel()
{
  // Treffer-Überprüfung
  var obj;
  if (obj = FindObject(0,1,0,0,0, OCF_Alive(),0,0,NoContainer()))      // Lebewesen
    if (Hostile(GetOwner(obj), GetOwner()) || (GetOwner(obj)==-1))
      return(Hit());
  if (obj = FindObject(0,1,0,0,0, OCF_Entrance(),0,0,NoContainer()))   // Fahrzeuge
    if (GetCategory(obj) & C4D_Vehicle())
      if (Hostile(GetOwner(obj), GetOwner()))
        return(Hit());
  // Bewegung
  SetXDir(xdir);
  SetYDir(0);
  // zufällige Explosion
  if (GetActTime() < 50) return(1);
  if (!Random(80-GetActTime()))
    return(Hit());
  return(1);
}


/* Treffer */

protected func Hit()
{
  var frag, i;
  for (i = 1; i < 5; ++i)
  {
    frag = CreateObject(SMG0, xdir/15*i, 0);
    ObjectCall(frag,"SetCounter",i);
  }
  Explode(4);
  return(1);
}

