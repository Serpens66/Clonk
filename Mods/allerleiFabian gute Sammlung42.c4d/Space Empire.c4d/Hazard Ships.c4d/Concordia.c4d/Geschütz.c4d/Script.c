/*-- Geschützkanzel --*/

#strict 3

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local waffensys, pilot, vessel, vertex;
  SetR(180);
  return(1);
}

func Bezug(_vessel)
{
  vessel=_vessel;
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
  if((GetR() ?? 0)<0) if((GetR() ?? 0)>-120)
  {
  SetRDir(nil);
  SetR(-120);
  }
  if((GetR() ?? 0)>0) if((GetR() ?? 0)<120)
  {
  SetRDir(nil);
  SetR(121);
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
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  return(SetRDir(nil));
}

func ContainedRight()
{
  return(SetRDir(-5));
}

func ContainedLeft()
{
  return(SetRDir(5));
}

func ContainedThrow() 
{
  if(!waffensys) Waffensystem();
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
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
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  if(waffensys=CreateContents(CHWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), CHWS))   ShiftContents(pilot);
  }
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}