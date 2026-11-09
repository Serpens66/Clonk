/*-- Geschützkanzel --*/

#strict 3

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local waffensys, pilot, vessel, vertex, r0 , rmin, rmax;
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}

func Rotinit(_r0,_rmin,_rmax)
{
  SetR(_r0);
  rmin=_rmin;
  rmax=_rmax;
  return(1);
}

func Explosion()
{
  RemoveObject();
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

func Check()
{
  if((GetR() ?? 0)<(rmin ?? 0))
  {
  SetRDir(nil);
  SetR((rmin ?? 0)+1);
  }
  if((GetR() ?? 0)>(rmax ?? 0))
  {
  SetRDir(nil);
  SetR((rmax ?? 0)-1);
  }
}

//Steuerung

func ContainedUp()
{
  return(SetRDir(nil));
}

func ContainedDown()
{
  return(SetRDir(nil));
}

func ContainedRight()
{
  if(Equal(GetR(), (rmax ?? 0)-1)) return(1);
  return(SetRDir(5));
}

func ContainedLeft()
{
  if(Equal(GetR(), (rmin ?? 0)+1)) return(1);
  return(SetRDir(-5));
}