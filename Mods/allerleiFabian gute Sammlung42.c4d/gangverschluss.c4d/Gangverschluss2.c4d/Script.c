/*-- Gangverschluss (aufgebaut) --*/

#strict 3

local hilfe;

func Initialize(...) {
hilfe = CreateObject(G35Y,AbsX(GetX()),(AbsY(GetY()) ?? 0)+7,GetOwner());
hilfe->~Set(this());
  SetAction("NoBridge");
return(1);
}

func ControlLeft(...) {
  SetAction("NoBridge");
  SetSolidMask(30, nil, 1, 1);
  Sound("Connect");
return(1);
}

func ControlRight(...) {
  SetAction("Bridge");
  SetSolidMask(nil, 31, 73, 17);
  Sound("Connect");
return(1);
}

func ControlDigDouble(...) {
RemoveObject(hilfe);
  ChangeDef(GV5Y);
  Sound("Connect");
return(1);
}
  
func Bridge(...) {
  SetSolidMask(nil, 31, 73, 17);
return(1);
}

func NoBridge(...) {
  SetSolidMask(30, nil, 1, 1);
return(1);
}

func Destruction(a,b,c,d,e,f)
{
PreDestruction();
_inherited(a,b,c,d,e,f);
}

func PreDestruction()
{
if(hilfe)
RemoveObject(hilfe);
}