/*-- UAV --*/

#strict 3

local on;
local dir;
local speed;

func Initialize()
{
  dir = 360;
  speed = 50;
  SetAction("Stand1");
  SetDirec(360);
  SetGraphics("2");
  return(1);
}

func ControlUpDouble(pCursor)
{
  if(FindContents(TAUV,pCursor))
  {
   Message("Ich trage bereits ein UAV!",pCursor);
   Sound("Error");
   return(1);
  }
  Enter(pCursor);
  Sound("Connect");
  return(1);
}

func Launch()
{
  if(on)
  {
   Sound("UAVTriebwerk",nil,nil,nil,nil,-1);
   SetGraphics("2");
   Sound("Click");
   on = nil;
  }
  else
  {
   Sound("UAVTriebwerk",nil,nil,nil,nil,1);
   SetAction("Spin");
   SetGraphics();
   SetDirec(dir);
   Sound("Rocket");
   on = 1;
  }
  return(1);
}

func Hit()
{
  if(!on)
  {
   dir = 90;
   SetDirec(90);
  }
  return(1);
}

func Hit3()
{
  DoDamage(200);
  return(1);
}

func Right()
{
  if(!on) { return; }
  if(Equal(dir, 5))
  {
   dir = 360;
   return(1);
  }
  S42MigrationAssign(dir, "-=", 5);
  return(1);
}

func Left()
{
  if(!on) { return; }
  if(Equal(dir, 360))
  {
   dir = 5;
   return(1);
  }
  S42MigrationAssign(dir, "+=", 5);
  return(1);
}

func Damage()
{
  if((GetDamage() ?? 0) > 50)
  {
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))   Explode(15);
 else FakeExplode(15,nil,nil,15);
  }
  return(1);
}

func Check()
{
  if(on)
  {
   SetDirec(dir);
   SetYDir(-1*((Sin((dir),speed)) ?? 0));
   SetXDir(Cos((dir),speed));
  }
  else
  {
   SetDirec(dir);
   SetYDir(nil);
   SetXDir(nil);
  }
  return(1);
}

func SetDirec(int angle)
{
  angle ??= 0;
  SetAction("Spin");
  SetPhase(((angle) ?? 0)/4);
  return(1);
}

func IsBulletTarget()  { return(true);}