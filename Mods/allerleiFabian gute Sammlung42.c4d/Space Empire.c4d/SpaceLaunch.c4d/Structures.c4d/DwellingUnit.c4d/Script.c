/*-- Wohneinheit --*/

#strict
#include BOB1
#include XFER

local basement, manager;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(17, -10);
}

protected func Construction()
{
  basement = CreateObject(BS16,0,+8,GetOwner());
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
  if (!uplink)  return(0);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyUp(caller);
}

protected func ContainedDig(caller)
{
  [Verkauf|Image=UPLI|Condition=GetUplink]
  var uplink = GetUplink();
  if (!uplink)  return(0);
  if (HasEMP()) return(Sound("Error"));
  uplink->KeyDig(caller);
}

private func GetUplink() 
{ 
  var uplink = FindObject(UPLB,0,0,0,0,0,"Turn",this());
  if (!uplink)
      uplink = FindObject(UPLT,0,0,0,0,0,"Turn",this());
  return(uplink);
}


/* Inhalt von Loren übernehmen */

private func DoorClosed()
{
  var num = -1;
  var obj;
  while (obj = Contents(++num))
    if (obj->~IsLorry())
      GrabContents(obj);
}


/* Eigenschaften */

public InfoBarSize: return(3);
public GetMaxDamage: return(240);

