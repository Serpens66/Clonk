/*-- Arsenal --*/

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

func ContainedDownDouble()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Enter(vessel,pilot);
  Sound("Airlock1");
  return(1);
}



func ContainedDig(pByObj) 
{
  pilot=pByObj;
  CreateMenu(TCN_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s","Bullets",STAM,Par(nil));
  AddMenuItem("%s","Missles",MIAM,Par(nil));
  return(1);
}


func Bullets()
{
  if((GetAmmo(STAM,pilot) ?? 0)>=200)
  {
  Message("Munitionsgürtel voll.",this());
  return(nil);
  }
  if((GetWealth(GetController(pilot)) ?? 0) < 10)
  {
  Message("Nicht genug Geld",this());
  return(nil);
  }
  SetWealth( GetController(pilot), Sub(GetWealth(GetController(pilot)),10) );
  DoAmmo(STAM,100,pilot);
  Message("100 Patronen gekauft.",this());
  Sound("Cash"); 
  return(1);
}


func Missles()
{
  if((GetAmmo(MIAM,pilot) ?? 0)>=12)
  {
  Message("Munitionsgürtel voll.",this());
  return(nil);
  }
  if((GetWealth(GetController(pilot)) ?? 0) < 10)
  {
  Message("Nicht genug Geld",this());
  return(nil);
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






















