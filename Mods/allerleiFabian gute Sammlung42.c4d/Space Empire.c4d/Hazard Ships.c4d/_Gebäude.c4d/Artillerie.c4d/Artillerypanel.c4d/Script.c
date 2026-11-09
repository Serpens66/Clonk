/*-- Artilleriesteuerung --*/

#strict 3

local bunker,gun, cost;


func Initialize() {
  cost=10;
  bunker = CreateObject(ART_,nil,20);
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
  Message("Schaden: %d%.",this(),(GetDamage(bunker) ?? 0)/3);
  return(1);
}

func ControlThrow(user)
{
  if(!bunker)  return(Sound("Error"));
  if(bunker->Wrecked()) return(Sound("Error"));
  var Plr=GetOwner(user);
  if(!Equal(GetOwner(), Plr)) ControlDigDouble(user);
  if(!SEqual(GetAction(gun),"Attaching")) return(1);
  //unendlich Muni?
  if(!ObjectCount(NOAM))
  {
  //genug Geld?
  if((GetWealth(Plr) ?? 0) < (cost ?? 0))
    return(Sound("Error"));
  //Geld abziehen
  DoWealth(Plr,-(cost ?? 0));
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



















