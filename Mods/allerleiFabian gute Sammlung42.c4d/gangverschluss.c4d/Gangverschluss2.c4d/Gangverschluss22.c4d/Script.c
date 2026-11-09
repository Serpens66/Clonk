/*-- Gangverschluss (hilfsobjekt) --*/

#strict 3

local main;

func Initialize(...) {
  SetAction("NoBridge");
return(1);
}

func ControlRight(...) {
  SetAction("Bridge");
main->~ControlRight();
return(1);
}

func ControlLeft(...) {
  SetAction("NoBridge");
main->~ControlLeft();
return(1);
}

func ControlDigDouble(...) {
main->~ControlDigDouble();
return(1);
}

func NoBridge(...) {
  SetSolidMask(30, nil, 1, 1);
return(1);
}

//checkt, ob main brennt
func Timer()
{
if(Or(!Equal(AbsX(GetX(main)), AbsX(GetX(this()))), !Equal(AbsY(GetY(main)), AbsY(GetY(this())))) )
SetPosition(AbsX(GetX(main)),AbsY(GetY(main)),this());
if(OnFire(main))
{RemoveObject();}
}

func Set(ziel)
{
main=ziel;
}