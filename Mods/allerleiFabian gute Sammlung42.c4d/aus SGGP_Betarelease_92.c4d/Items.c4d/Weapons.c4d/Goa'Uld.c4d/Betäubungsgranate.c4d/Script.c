/*-- Betäubungsgranate --*/
#strict
local Target;
local pStructure;

public func Activate()
{
  if(GetAction(Contained()) eq "Lie")
  {
   return(1);
  }
  
  if(GetAction() eq "Scharf")
  {
   Contained(this())->SetAction("Throw");
   Exit(this(), GetDir(Contained())*12-6, 0, 0, GetDir(Contained())*6-3, -3, GetDir(Contained())*60-30);
   return(1);
  }

  SetAction("Scharf");
//  ScheduleCall(0,"Exploden",100);
  Sound("Connect");
  return(1);
}

global func KnockWave(int Level,object pTarget)
{
  if(!pTarget)
  {
   Message("KEIN ZIEL!");
   return(1);
  }
  CreateObject(WRSW,0,0,GetOwner(pTarget))->SetMaxSize(Level);
  return(1);
}

protected func Exploden()
{
  Sound("Connect");
  CreateObject(WRSW, 0, 0, GetOwner())->SetMaxSize(300);
  RemoveObject();
  return(1);
}

protected func Hit()
{
  Sound("GRHit*");
}



func IsGoauldWeapon() { return(true); }
func AlchemProcessTime() { return(500); }
func IsGrenade() { return(true); }