/*-- Brücke --*/

#strict

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
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this())) return(1);
  return(0);
}


func Explosion(sphere)
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(sphere,pilot);
  RemoveObject();
  return(1);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
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
  CreateMenu(A56B,Par(0),0,0,0,0);
  AddMenuItem("%s","Schwebe",CB_2,Par(0));   //Schwebe	
  AddMenuItem("%s","Damage",CB_3,Par(0));    //Schaden	
  AddMenuItem("%s","Ausstieg",CB_1,Par(0));    //Ausstieg	
  return(1);
}

func Damage()
{
  Message("Schaden: %d%.",this(),GetDamage(vessel)/5);
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
  SetComDir(COMD_Up(),vessel);
  return(1);
}


func ContainedDown()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  SetComDir(COMD_Down(),vessel);
  return(1);
}


func ContainedLeft()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  SetComDir(COMD_Left(),vessel);
  return(1);
}


func ContainedRight()
{
  if(SEqual(GetAction(vessel),"Down")) return(1);
  SetComDir(COMD_Right(),vessel);
  return(1);
}






















