/*-- Effekt --*/

#strict

local Target, XDir, YDir, Modus, value, Player;

SetTarget:
  Target=Par(0);
  Modus=Par(1);
  value=Par(2);
  Player=Par(3);
  SetAction("Fly");
  return(1);

Pos:
  CreateObject(EFK2,0,5);
  SetR( Angle(GetX(),GetY() , GetX(Target),GetY(Target) ) );
//Bewegen:
  XDir = +Sin(GetR(),10);
  YDir = -Cos(GetR(),10);
  SetXDir(XDir);
  SetYDir(YDir);
  if(Distance(GetX(),GetY() , GetX(Target),GetY(Target)) < 40)
    Action();
  return(1);

Action:
  if(Modus==0)
    return(ModeOne());
  if(Modus==1)
    return(ModeTwo());
  if(Modus==2)
    return(ModeThree());
  return(1);

ModeOne:
  var obj;
  while(obj=FindObject(0,0,0,0,0,0,0,0,this(),0))
{
    Exit(obj,0,0);
    SetPosition(GetX(Target),GetY(Target),obj);
}
  Ready();
  return(1);

ModeTwo:
  var obj;
  while(obj=FindObject(0,0,0,0,0,0,0,0,this(),0))
{
    Exit(obj,0,0);
    Enter(Target,obj);
    SetPosition(GetX(Target),GetY(Target),obj);
}
  Ready();
  return(1);

ModeThree:
  SetWealth(Player,GetWealth(Player)+value);
  Ready();
  return(1);

Ready:
  ObjectCall(Target,"Effekt");
  Sound("Effekt");
  RemoveObject();
  return(1);