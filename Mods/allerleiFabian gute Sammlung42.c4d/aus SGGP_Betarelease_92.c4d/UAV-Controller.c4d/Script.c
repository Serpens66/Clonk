/*-- UAV-Kontrollstation --*/

#strict 3
#include BAS3

local target;
local pUser;
local hold;
local condir;

func Initialize()
{
  target = nil;
  SetAction("Spin");
  return(1);
}

func ControlDigDouble(pCaller)
{
  pUser = pCaller;
  CreateMenu(TAUC, pUser, nil, nil, "UAV-Kontrolle", nil, 1);
  if(!target)
  {
   AddMenuItem("UAV kaufen: 50c","UAVBuy",MEPU,pUser);
  }
  else
  {
   AddMenuItem("UAV selbstzerstören","UAVDestroy",MEPU,pUser);
  }
  Sound("Ding");
  return(1);
}

func Damage()
{
  if((GetDamage() ?? 0) > 100)
  {
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))   Explode(30);
 else FakeExplode(30,nil,nil,30);
  }
  return(1);
}

func UAVDestroy()
{
  target->DoDamage(200);
  Message("<c ff0000>UAV wurde zerstört!</c>",this());
  Sound("BipBipBip");
  return(1);
}

func UAVBuy()
{
  if((GetWealth(GetOwner(pUser)) ?? 0) < 50)
  {
   Message("<c ff0000>Zu wenig geld!|Es fehlen:%v</c>",this(),50-(GetWealth(GetOwner(pUser)) ?? 0));
   Sound("Error");
   return(1);
  }
  if(target)
  {
   Message("Es ist bereits ein UAV vorhanden!",this());
   Sound("Error");
   return(1);
  }
  SetWealth(GetOwner(pUser),(GetWealth(GetOwner(pUser)) ?? 0)-50);
  target = CreateObject(TAUV);
  LocalN("on",target) = 1;
  hold = 1;
  Sound("Uncash");
  return(1);
}

func Check()
{
  if(!hold)
  {
   if(target)
   {
    if((ObjectDistance(target) ?? 0) < 30)
    {
     if(Equal(LocalN("on",target), nil))
     {
      LocalN("dir",target) = 360;
      target->SetDirec(360);
      target->SetAction("Spin");
      LocalN("on",target) = 1;
      hold = 1;
      Sound("Connect");
      return(1);
     }
    }
   }
  }

  if(hold)
  {
   if(target)
   {
    target->SetPosition((GetX() ?? 0)-5,(GetY() ?? 0)-15);
    target->SetYDir();
    LocalN("dir",target) = 360;
    target->SetDirec(360);
    return(1);
   }
  }
  else
  {
   if(target)
   {
    if(Equal(condir, -1))
    {
     target->Left();
    }
    if(Equal(condir, 1))
    {
     target->Right();
    }
   }
  }
  return(1);
}

func ControlThrow(pCursor)
{
  pUser = pCursor;
  if(!target)  { return; }
  SetPlrView(GetOwner(pUser),target);
  if(hold)
  {
   hold = nil;
   LocalN("on",target) = nil;
  }
  target->Launch();
  ControlUp(pCursor);
  return(1);
}


func ControlLeft(pCursor)
{
  pUser = pCursor;
  if(!target)  { return(1); }
  if(hold)  { return(1); }
  SetPlrView(GetOwner(pUser),target);
  condir = -1;
  return(1);
}

func ControlRight(pCursor)
{
  pUser = pCursor;
  if(!target)  { return(1); }
  if(hold)  { return(1); }
  SetPlrView(GetOwner(pUser),target);
  condir = 1;
  return(1);
}

func ControlUp(pCursor)
{
  pUser = pCursor;
  if(!target)  { return(1); }
  if(hold)  { return(1); }
  SetPlrView(GetOwner(pUser),target);
  condir = nil;
  return(1);
}