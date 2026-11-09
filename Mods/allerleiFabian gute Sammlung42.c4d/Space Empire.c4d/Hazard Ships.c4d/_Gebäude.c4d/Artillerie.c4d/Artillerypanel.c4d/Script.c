/*-- Artilleriesteuerung --*/

#strict

local bunker,gun, cost;


func Initialize() {
  cost=10;
  bunker = CreateObject(ART_,0,20);
  gun=bunker->GetGun();
}

func ControlDigDouble(user)
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  bunker->SetUser(user);
  SetOwner(GetOwner(user),this());
  Sound("BipBipBip");
  return(1);
}

func Cost(c)  {cost=c;}

func ControlUpDouble()
{
  if(!bunker)  return(Sound("Error"));
  if (!GetDamage(bunker))
  {
  Message("Kein Schaden.",this());
  return(1);
  }
  Message("Schaden: %d%.",this(),GetDamage(bunker)/3);
  return(1);
}

func ControlThrow(user)
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  var Plr=GetOwner(user);
  if(GetOwner()!=Plr) ControlDigDouble(user);
  if(!SEqual(GetAction(gun),"Attaching")) return(1);
  //unendlich Muni?
  if(!ObjectCount(NOAM))
  {
  //genug Geld?
  if(GetWealth(Plr) < cost)
    return(Sound("Error"));
  //Geld abziehen
  DoWealth(Plr,-cost);
  }
  gun->Fire();
  return(1);
}

func ControlUp()
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  gun->Up();
  return(1);
}

func ControlDown()
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  gun->Down();
  return(1);
}

func ControlLeft()
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  gun->Left();
  return(1);
}

func ControlRight()
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  gun->Right();
  return(1);
}



















