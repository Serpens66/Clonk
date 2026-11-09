/*-- Feuerleitstand --*/

#strict

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local waffensys, pilot, sphere;
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
  sphere=FindObject(TSP_, 0,0,-1,-1, 0,0,0); 
  if(sphere->Besetzt()) 
  {
  Message("Sphäre besetzt.",this());
  Sound("Error");
  return(0);
  }
  Enter(sphere,pilot);
  DoAmmo(ENAM, GetAmmo(ENAM,waffensys), pilot); 
  DoAmmo(MIAM, GetAmmo(MIAM,waffensys), pilot); 
  if(waffensys) RemoveObject(waffensys);
  return(SetRDir(0));
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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
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
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  if(waffensys=CreateContents(TSWS, pilot))
  {
  while(GetID(Contents(0,pilot))!=TSWS)   ShiftContents(pilot);
  }
  return(1);
}

func Besetzt()
{
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}