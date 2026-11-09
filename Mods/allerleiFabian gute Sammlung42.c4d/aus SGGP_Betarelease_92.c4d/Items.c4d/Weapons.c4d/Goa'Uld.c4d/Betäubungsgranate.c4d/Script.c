/*-- Betäubungsgranate --*/
#strict 3
local Target;
local pStructure;

public func Activate()
{
  if(!!SEqual(GetAction(Contained()), "Lie"))
  {
   return(1);
  }
  
  if(!!SEqual(GetAction(), "Scharf"))
  {
   Contained(this())->SetAction("Throw");
   Exit(this(), (GetDir(Contained()) ?? 0)*12-6, nil, nil, (GetDir(Contained()) ?? 0)*6-3, -3, (GetDir(Contained()) ?? 0)*60-30);
   return(1);
  }

  SetAction("Scharf");
//  ScheduleCall(0,"Exploden",100);
  Sound("Connect");
  return(1);
}

global func KnockWave(int Level,object pTarget)
{
  Level ??= 0;
  if(!pTarget)
  {
   Message("KEIN ZIEL!");
   return(1);
  }
  CreateObject(WRSW,nil,nil,GetOwner(pTarget))->SetMaxSize(Level);
  return(1);
}

protected func Exploden()
{
  Sound("Connect");
  CreateObject(WRSW, nil, nil, GetOwner())->SetMaxSize(300);
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