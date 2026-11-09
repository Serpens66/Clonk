/*-- Gangverschluss (hilfsobjekt) --*/

#strict

local main;

Initialize:
  SetAction("NoBridge");
return(1);

ControlRight:
  SetAction("NoBridge");
main->~ControlRight();
return(1);

ControlLeft:
  SetAction("Bridge");
main->~ControlLeft();
return(1);

ControlDigDouble:
main->~ControlDigDouble();
return(1);



//checkt, ob main brennt
func Timer()
{
if(AbsX(GetX(main))!=AbsX(GetX(this())) || AbsY(GetY(main))!=AbsY(GetY(this())) )
SetPosition(AbsX(GetX(main)),AbsY(GetY(main)),this());
if(OnFire(main))
{RemoveObject();}
}


func Set(ziel)
{
main=ziel;
}