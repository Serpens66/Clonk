/*-- Basis --*/

#strict 3
#include BOB1
#include XFER

local basement, manager;


protected func Construction()
{
  basement = CreateObject(BS16,nil,8,GetOwner());
  return(1);
}

protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement); 
  return(1);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  return(inherited());
}

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-24,-24);
  return(1);
}

// Uplink-Steuerung

protected func ContainedUp()
{
  [Kauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(nil);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyUp(Par(nil));
  return(1);
}

protected func ContainedDig()
{
  [Verkauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(nil);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyDig(Par(nil));
  return(1);
}

private func GetUplink() 
{ 
  var uplink = FindObject(UPLB,nil,nil,nil,nil,nil,"Turn",this());
  if (!uplink)
      uplink = FindObject(UPLT,nil,nil,nil,nil,nil,"Turn",this());
  return(uplink);
}

// Funktion für Loren u.ä

private func DoorClosed()  
{
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
      GrabContents(obj);
  return(1);
}


// Eigenschaften

public func GetMaxDamage() { return(400); }
public func InfoBarPos() { return(12); }
public func InfoBarSize() { return(4); }

