/*-- EJD --*/

#strict

local range;
local timer;

func Initialize() 
{
  timer = 300;
  SetGraphics(0);
  range = 500;
  return(1);
}

func Check()
{
  if(timer)
  {
   timer --;
   SetGraphics(0);
  }
  else
  {
   SetGraphics("Green");
  }
  return(1);
}

func ControlDigDouble(pCaller)
{
  if(timer > 0)
  {
   Sound("Ding");
   return(1);
  }
  Sound("Tac");
  timer = 300;
  var desY;
  var desX;
  if(GetDir(Contained()) == 1)
  {
   desX = GetX()+(range/2);
  }
  else
  {
   desX = GetX()-(range/2);
  }
  desY = GetYLaunch(GetY(),desX);
  pCaller->SetPosition(desX,desY-10);
  return(1);
}
  
func GetYLaunch(int i,int j)
{
  var a;
  a = i + (range/2);
  while(GBackSolid(j-GetX(),a-GetY())||GBackLiquid(j-GetX(),a-GetY())) a-=2;
  return(a);
}