/*-- Geschützkanzel --*/

#strict

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local waffensys, pilot, vessel, vertex, r0 , rmin, rmax, rocky;
}

func Bezug(_vessel,_rocky)
{
  vessel=_vessel;
  rocky=_rocky;
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
  SetRDir(0,rocky);
  return(SetRDir(0));
}

func ContainedDown()
{
  SetRDir(0,rocky);
  return(SetRDir(0));
}

func ContainedDownDouble()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(vessel,pilot);
  DoAmmo(STAM, GetAmmo(STAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  return(SetRDir(0));
}

func ContainedRight()
{
  SetRDir(5,rocky);
  return(SetRDir(5));
}

func ContainedLeft()
{
  SetRDir(-5,rocky);
  return(SetRDir(-5));
}

func ContainedThrow() 
{
  if(!waffensys) Waffensystem();
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func ContainedDigDouble()
{
  if(!waffensys)  Waffensystem();
  ObjectCall(waffensys,"ControlDigDouble",pilot);
  Sound("PRModus");
  return(1);
}

func Waffensystem()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  if(waffensys=CreateContents(CSWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=CSWS)   ShiftContents(pilot);
  waffensys->Bezug(rocky);
  }
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}