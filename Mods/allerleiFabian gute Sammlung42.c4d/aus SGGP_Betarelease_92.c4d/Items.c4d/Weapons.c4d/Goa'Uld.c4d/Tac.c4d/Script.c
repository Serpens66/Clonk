/*-- Tac --*/
#strict

local on;
local target;
local pX;
local pY;
local time;
local shots;

public func Activate(pCursor)
{
  pX = GetX();
  pY = GetY();
  SetOwner(GetOwner(pCursor));
  if(on)
  {
   Exit();
   return(1);
  }
  Exit();
  on = 1;
  time = 5;
}

public func Damage()
{
  Explode(3);
  return(1);
}

public func EMPShock()
{
  Explode(3);
  return(1);
}

public func Aktiv()
{
  on = 1;
  if(Contained())
  {
   Exit();
  }
  Sound("Connect");
  return(1);
}

public func ControlThrow(pCursor)
{
  SetOwner(GetOwner(pCursor));
  if(on)
  {
   return(1);
  }
  Enter(pCursor);
  Sound("Connect");
  return(1);
}

public func Check()
{
  if(shots == 100)
  {
   Explode(3);
  }
  if(time > 0)
  {
   time --;
  }
  
  if(!on)
  {
   return();
  }
  SetPosition(pX,pY);
  SetXDir();
  SetYDir();

  if(time == 0)
  {
   time = 5;

  if(FindObject2(Find_Or(Find_OCF(OCF_Alive()),Find_Category(C4D_Vehicle())),Find_Distance(400),Find_Not(Find_Action("Lie")),Find_Not(Find_Action("Zatfall")),Find_NoContainer(),Find_PathFree(),Find_Hostile(GetOwner())))
  {
   target = FindObject2(Find_Or(Find_OCF(OCF_Alive()),Find_Category(C4D_Vehicle())),Find_Distance(400),Find_Not(Find_Action("Lie")),Find_Not(Find_Action("Zatfall")),Find_NoContainer(),Find_PathFree(),Find_Hostile(GetOwner()));
   var angle;
   var ammo;
   angle = Angle(Abs(GetX()),Abs(GetY()),Abs(GetX(target)),Abs(GetY(target)));
   SetR(angle);
   var ammo = CreateObject(TACS,0,-3,GetOwner(this()));
   ammo->Launch(RandomX(angle-3,angle+3),250,500,2,30,2);
   Sound("tac");
   shots++;
   return(1);
  }
  
  if(FindObject2(Find_Distance(400),Find_PathFree(),Find_ID(REPL)))
  {
   target = FindObject2(Find_Distance(400),Find_PathFree(),Find_ID(REPL));
   var angle;
   var ammo;
   angle = Angle(Abs(GetX()),Abs(GetY()),Abs(GetX(target)),Abs(GetY(target)));
   SetR(angle);
   var ammo = CreateObject(TACS,0,-3,GetOwner(this()));
   ammo->Launch(RandomX(angle-3,angle+3),250,500,2,30,2);
   Sound("tac");
   shots++;
   return(1);
  }
  
  
  }
  return(1);
}

func IsGoauldWeapon() 	{ return(true); }
func IsMachine() 		{ return(true); }