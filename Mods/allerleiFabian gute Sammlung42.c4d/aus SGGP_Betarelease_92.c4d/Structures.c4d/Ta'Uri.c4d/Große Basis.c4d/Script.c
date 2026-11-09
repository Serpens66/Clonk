/*-- Basis --*/

#strict 3

#include DOOR
#include BA15
#include XFER

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

protected func SoundOpenDoor() { return(1); }
protected func SoundCloseDoor() { return(1); }

protected func ContainedLeft(pCursor)
{
  pCursor->Exit();
  pCursor->SetPosition(GetX(),(GetY() ?? 0)+10);
  return(1);
}

protected func ContextAufstieg(object pClonk)
{
  [Tower betreten|Image=MEPU]
  pClonk->ContainedCall("ContainedLeft", this());
}

func Damage()
{
  if((GetDamage() ?? 0) >= 200)
  {
   Explode(90);
  }
  return(1);
}