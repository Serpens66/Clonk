/*-- Gangverschluss (aufgebaut) --*/

#strict

local hilfe;

Initialize:  
hilfe = CreateObject(G35Y,AbsX(GetX()),AbsY(GetY())+7,GetOwner());
hilfe->~Set(this());
  SetAction("NoBridge");
return(1);

ControlLeft:
  SetAction("NoBridge");
  SetSolidMask(30, 0, 1, 1);
  Sound("Connect");
return(1);

ControlRight:
  SetAction("Bridge");
  SetSolidMask(0, 31, 73, 17);
  Sound("Connect");
return(1);

ControlDigDouble:
RemoveObject(hilfe);
  ChangeDef(GV5Y);
  Sound("Connect");
return(1);
  
Bridge:
  SetSolidMask(0, 31, 73, 17);
return(1);

NoBridge:
  SetSolidMask(30, 0, 1, 1);
return(1);

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