/*-- Hangar-Tor --*/

#strict 3

local phase;
local opened;

func Initialize()
{
  SetAction("Closed");
  return(1);
}

func CheckSolidMask()
{
  phase = GetPhase();
  if (opened)
    SetSolidMask((phase ?? 0)*6,nil,6,118,nil,nil,this());
  else SetSolidMask(((phase ?? 0)-13)*6,nil,6,118,nil,nil,this());
  return(1);
}

func End()
{
  Sound("DrawbridgeHit");
  return(1);
}

func Open()
{
  opened=true;
  return(1);
}

func Close()
{
  opened=nil;
  return(1);
}
