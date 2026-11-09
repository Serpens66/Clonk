/*-- Brücke --*/

#strict

func Initialize()
{
   local vessel, pilot;
   SetEntrance(1);
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}


//Steuerung
func ContainedDig(pByObj) 
{
  pilot=pByObj;
  CreateMenu(CCD_,Par(0),0,0,0,0);
  AddMenuItem("%s","Schwebe",M27G,Par(0));     //Schwebe	
  AddMenuItem("%s","Hauptdeck",C11Q,Par(0));   //Hauptdeck
  AddMenuItem("%s","Ausstieg",M47G,Par(0));    //Ausstieg	
  return(1);
}


func Hauptdeck()
{
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}


func Ausstieg()
{
  Exit(pilot,0,5);
  Sound("Airlock1");
  return(1);
}


func Schwebe()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  ObjectCall(vessel,"Schwebe");
  return(1);
}


func ContainedThrow()
{
  return(1);
}


func ContainedUp()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Up(),vessel);
  return(1);
}


func ContainedDown()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Down(),vessel);
  return(1);
}


func ContainedLeft()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Left(),vessel);
  return(1);
}


func ContainedRight()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Right(),vessel);
  return(1);
}






















