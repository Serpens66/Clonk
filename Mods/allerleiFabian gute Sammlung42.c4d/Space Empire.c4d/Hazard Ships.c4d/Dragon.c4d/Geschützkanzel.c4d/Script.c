/*-- Geschützkanzel --*/

#strict

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
  return(0);
}

func Check()
{
  if(GetR()<rmin)
  {
  SetRDir(0);
  SetR(rmin+1);
  }
  if(GetR()>rmax)
  {
  SetRDir(0);
  SetR(rmax-1);
  }
}

//Steuerung

func ContainedUp()
{
  return(SetRDir(0));
}

func ContainedDown()
{
  return(SetRDir(0));
}

func ContainedRight()
{
  if(GetR()==rmax-1) return(1);
  return(SetRDir(5));
}

func ContainedLeft()
{
  if(GetR()==rmin+1) return(1);
  return(SetRDir(-5));
}