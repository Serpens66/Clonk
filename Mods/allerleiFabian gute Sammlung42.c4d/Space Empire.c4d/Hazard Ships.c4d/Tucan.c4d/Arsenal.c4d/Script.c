/*-- Arsenal --*/

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

func ContainedDownDouble()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this());
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}



func ContainedDig(pByObj) 
{
  pilot=pByObj;
  CreateMenu(TCN_,Par(0),0,0,0,0);
  AddMenuItem("%s","Bullets",STAM,Par(0)); 
  AddMenuItem("%s","Missles",MIAM,Par(0));   		
  return(1);
}


func Bullets()
{
  if(GetAmmo(STAM,pilot)>=200)
  {
  Message("Munitionsgürtel voll.",this());
  return(0);
  }
  if(GetWealth(GetController(pilot)) < 10)
  {
  Message("Nicht genug Geld",this());
  return(0);
  }
  SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),10) );
  DoAmmo(STAM,100,pilot);
  Message("100 Patronen gekauft.",this());
  Sound("Cash"); 
  return(1);
}


func Missles()
{
  if(GetAmmo(MIAM,pilot)>=12)
  {
  Message("Munitionsgürtel voll.",this());
  return(0);
  }
  if(GetWealth(GetController(pilot)) < 10)
  {
  Message("Nicht genug Geld",this());
  return(0);
  }
  SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),10) );
  DoAmmo(MIAM,3,pilot);
  Message("3 Raketen gekauft.",this());
  Sound("Cash"); 
  return(1);
}


func ContainedUp()
{
    return(1);
}

func ContainedDown()
{
    return(1);
}


func ContainedThrow()
{
  return(1);
}


func ContainedLeft()
{
    return(1);
}


func ContainedRight()
{
    return(1);
}






















