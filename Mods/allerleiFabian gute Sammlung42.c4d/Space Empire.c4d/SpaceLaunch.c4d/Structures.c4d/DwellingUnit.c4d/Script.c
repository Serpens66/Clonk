/*-- Wohneinheit --*/

#strict 3
#include BOB1
#include XFER

local basement, manager;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(17, -10);
}

protected func Construction()
{
  basement = CreateObject(BS16,nil,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  inherited();
}


/* Uplink-Steuerung */

protected func ContainedUp(caller)
{
  [Kauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(nil);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyUp(caller);
}

protected func ContainedDig(caller)
{
  [Verkauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(nil);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyDig(caller);
}

private func GetUplink() 
{ 
  var uplink = FindObject(UPLB,nil,nil,nil,nil,nil,"Turn",this());
  if (!uplink)
      uplink = FindObject(UPLT,nil,nil,nil,nil,nil,"Turn",this());
  return(uplink);
}


/* Inhalt von Loren übernehmen */

private func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
      GrabContents(obj);
}


/* Eigenschaften */

public func InfoBarSize(...) { return(3);
}
public func GetMaxDamage(...) { return(240);
}

