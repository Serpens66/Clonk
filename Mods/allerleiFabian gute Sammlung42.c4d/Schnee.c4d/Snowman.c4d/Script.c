/*-- Schneemann --*/

#strict


protected func Initialize() {
  SetCategory(C4D_Vehicle());
  return(1);
}

func CheckTemperatur()
{
if(GetTemperature()>0)
{  DoCon(-10);}
}

func Destruction()
{
CreateObject(CNKT);
_inherited();
}