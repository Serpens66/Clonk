/*-- Brücke --*/

#strict 3

func Initialize()
{
   local vessel, pilot;
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}


func Besetzt()
{
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this())) return(1);
  return(nil);
}


func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}


//Steuerung

func Ausstieg()
{
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}



func ContainedDig(pByObj) 
{
  pilot=pByObj;
  CreateMenu(TCN_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s","Schwebe",M27G,Par(nil));   //Schwebe
  AddMenuItem("%s","Ausstieg",M47G,Par(nil));    //Ausstieg
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
  SetComDir(COMD_Up,vessel);
  return(1);
}


func ContainedDown()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Down,vessel);
  return(1);
}


func ContainedLeft()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Left,vessel);
  return(1);
}


func ContainedRight()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  if(SEqual(GetAction(vessel),"Hangar")) return(1);
  SetComDir(COMD_Right,vessel);
  return(1);
}






















