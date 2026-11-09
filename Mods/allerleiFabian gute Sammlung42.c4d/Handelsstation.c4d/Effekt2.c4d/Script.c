/*-- Effekt --*/

#strict 3

local Target, XDir, YDir, Modus, value, Player;

func SetTarget(...) {
  Target=Par(nil);
  Modus=Par(1);
  value=Par(2);
  Player=Par(3);
  SetAction("Fly");
  return(1);
}

func Pos(...) {
  CreateObject(EFK2,nil,5);
  SetR( Angle(GetX(),GetY() , GetX(Target),GetY(Target) ) );
//Bewegen:
  XDir = +(Sin(GetR(),10) ?? 0);
  YDir = -(Cos(GetR(),10) ?? 0);
  SetXDir(XDir);
  SetYDir(YDir);
  if((Distance(GetX(),GetY() , GetX(Target),GetY(Target)) ?? 0) < 40)
    Action();
  return(1);
}

func Action(...) {
  if(Equal(Modus, nil))
    return(ModeOne());
  if(Equal(Modus, 1))
    return(ModeTwo());
  if(Equal(Modus, 2))
    return(ModeThree());
  return(1);
}

func ModeOne(...) {
  var obj;
  while(obj=FindObject(nil,nil,nil,nil,nil,nil,nil,nil,this(),nil))
{
    Exit(obj,nil,nil);
    SetPosition(GetX(Target),GetY(Target),obj);
}
  Ready();
  return(1);
}

func ModeTwo(...) {
  var obj;
  while(obj=FindObject(nil,nil,nil,nil,nil,nil,nil,nil,this(),nil))
{
    Exit(obj,nil,nil);
    Enter(Target,obj);
    SetPosition(GetX(Target),GetY(Target),obj);
}
  Ready();
  return(1);
}

func ModeThree(...) {
  SetWealth(Player,(GetWealth(Player) ?? 0)+(value ?? 0));
  Ready();
  return(1);
}

func Ready(...) {
  ObjectCall(Target,"Effekt");
  Sound("Effekt");
  RemoveObject();
  return(1);
}