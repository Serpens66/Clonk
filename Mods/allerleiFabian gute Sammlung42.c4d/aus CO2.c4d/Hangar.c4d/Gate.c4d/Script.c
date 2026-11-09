/*-- Hangar-Tor --*/

#strict

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
    SetSolidMask(phase*6,0,6,118,0,0,this());
  else SetSolidMask((phase-13)*6,0,6,118,0,0,this());
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
  opened=false;
  return(1);
}
