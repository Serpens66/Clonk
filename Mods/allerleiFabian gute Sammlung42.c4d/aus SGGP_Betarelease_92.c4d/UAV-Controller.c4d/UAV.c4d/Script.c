/*-- UAV --*/

#strict

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
   Sound("UAVTriebwerk",0,0,0,0,-1);
   SetGraphics("2");
   Sound("Click");
   on = 0;
  }
  else
  {
   Sound("UAVTriebwerk",0,0,0,0,1);
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
  if(!on) { return(); }
  if(dir == 5)
  {
   dir = 360;
   return(1);
  }
  dir -= 5;
  return(1);
}

func Left()
{
  if(!on) { return(); }
  if(dir == 360)
  {
   dir = 5;
   return(1);
  }
  dir += 5;
  return(1);
}

func Damage()
{
  if(GetDamage() > 50)
  {
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))   Explode(15);
 else FakeExplode(15,0,0,15);
  }
  return(1);
}

func Check()
{
  if(on)
  {
   SetDirec(dir);
   SetYDir(-1*(Sin((dir),speed)));
   SetXDir(Cos((dir),speed));
  }
  else
  {
   SetDirec(dir);
   SetYDir(0);
   SetXDir(0);
  }
  return(1);
}

func SetDirec(int angle)
{
  SetAction("Spin");
  SetPhase((angle)/4);
  return(1);
}

func IsBulletTarget()  { return(true);}