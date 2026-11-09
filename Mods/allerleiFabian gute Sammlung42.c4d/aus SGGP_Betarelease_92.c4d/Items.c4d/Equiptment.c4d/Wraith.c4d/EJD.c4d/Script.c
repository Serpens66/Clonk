/*-- EJD --*/

#strict 3

local range;
local timer;

func Initialize() 
{
  timer = 300;
  SetGraphics(nil);
  range = 500;
  return(1);
}

func Check()
{
  if(timer)
  {
   S42MigrationStep(timer , -1, true);
   SetGraphics(nil);
  }
  else
  {
   SetGraphics("Green");
  }
  return(1);
}

func ControlDigDouble(pCaller)
{
  if((timer ?? 0) > 0)
  {
   Sound("Ding");
   return(1);
  }
  Sound("Tac");
  timer = 300;
  var desY;
  var desX;
  if(Equal(GetDir(Contained()), 1))
  {
   desX = (GetX() ?? 0)+((range ?? 0)/2);
  }
  else
  {
   desX = (GetX() ?? 0)-((range ?? 0)/2);
  }
  desY = GetYLaunch(GetY(),desX);
  pCaller->SetPosition(desX,(desY ?? 0)-10);
  return(1);
}
  
func GetYLaunch(int i,int j)
{
  i ??= 0;
  j ??= 0;
  var a;
  a = (i ?? 0) + ((range ?? 0)/2);
  while(Or(GBackSolid((j ?? 0)-(GetX() ?? 0),(a ?? 0)-(GetY() ?? 0)), GBackLiquid((j ?? 0)-(GetX() ?? 0),(a ?? 0)-(GetY() ?? 0)))) S42MigrationAssign(a, "-=", 2);
  return(a);
}