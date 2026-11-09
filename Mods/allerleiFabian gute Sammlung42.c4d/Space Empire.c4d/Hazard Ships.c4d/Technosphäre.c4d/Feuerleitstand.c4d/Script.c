/*-- Feuerleitstand --*/

#strict 3

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local waffensys, pilot, sphere;
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
  sphere=FindObject(TSP_, nil,nil,-1,-1, nil,nil,nil);
  if(sphere->Besetzt()) 
  {
  Message("Sphäre besetzt.",this());
  Sound("Error");
  return(nil);
  }
  Enter(sphere,pilot);
  DoAmmo(ENAM, GetAmmo(ENAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  return(SetRDir(nil));
}

func ContainedRight()
{
  if (SEqual(GetAction(),"Laden")) return(1);
  return(SetRDir(5));
}

func ContainedLeft()
{
  if (SEqual(GetAction(),"Laden")) return(1);
  return(SetRDir(-5));
}

func Laden()
{
  SetAction("Laden");
}

func ContainedThrow() 
{

  if(!waffensys) Waffensystem();
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  ObjectCall(waffensys,"ControlThrow",pilot);
  return(1);
}

func ContainedDig()
{
  Waffenmodi();
  return(1);
}

func Waffenmodi()
{
  if(!waffensys)  Waffensystem();
  ObjectCall(waffensys,"ControlDigDouble",pilot);
  Sound("PRModus");
  return(1);
}


func Waffensystem()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  if(waffensys=CreateContents(TSWS, pilot))
  {
  while(!Equal(GetID(Contents(nil,pilot)), TSWS))   ShiftContents(pilot);
  }
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}