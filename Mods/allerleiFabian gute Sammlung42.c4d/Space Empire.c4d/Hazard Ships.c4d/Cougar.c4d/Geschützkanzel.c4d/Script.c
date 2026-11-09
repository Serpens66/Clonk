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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  if(waffensys) RemoveObject(waffensys);
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

func ContainedDownDouble()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Enter(vessel,pilot);
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  return(SetRDir(nil));
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
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  if(waffensys=CreateContents(GSWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), GSWS))   ShiftContents(pilot);
  }
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}