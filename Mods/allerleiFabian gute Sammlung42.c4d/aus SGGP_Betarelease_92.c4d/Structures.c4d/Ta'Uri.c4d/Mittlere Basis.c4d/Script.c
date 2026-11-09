/*-- Basis --*/

#strict

#include DOOR
#include BA15
#include XFER

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

protected func SoundOpenDoor() { return(1); }
protected func SoundCloseDoor() { return(1); }

func Damage()
{
  if(GetDamage() >= 200)
  {
   Explode(70);
  }
  return(1);
}