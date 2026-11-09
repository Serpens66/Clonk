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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  if(waffensys) RemoveObject(waffensys);
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

func ContainedDownDouble()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(vessel,pilot);
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  return(SetRDir(0));
}

func ContainedRight()
{
  return(SetRDir(5));
}

func ContainedLeft()
{
  return(SetRDir(-5));
}

func ContainedThrow() 
{
  if(!waffensys) Waffensystem();
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  if(waffensys=CreateContents(GSWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=GSWS)   ShiftContents(pilot);
  }
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}