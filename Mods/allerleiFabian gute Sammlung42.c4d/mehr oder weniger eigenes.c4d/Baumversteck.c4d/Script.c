/*-- Baum --*/

#strict

/*-- Eingangssteuerung --*/

func Completion() {
  SetCategory(C4D_Vehicle());
  return(1);
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


// DOOR
/* Dieses Objekt stellt die Grundfunktionalität für Gebäude
   mit Tür zur Verfügung. Die Aktionen OpenDoor, DoorOpen
   und CloseDoor müssen selbstverständlich in der ActMap der
   jeweiligen Gebäude vorhanden sein. */

protected func ActivateEntrance(pObj)
{
  if (ActIdle()) SetAction("OpenDoor");
  return(1);
}
  
private func OpenEntrance()
{
  SetEntrance(1);
}

private func CloseEntrance()
{
  SetEntrance(0);
}

private func SoundOpenDoor()
{
//  Sound("DoorOpen");
}

private func SoundCloseDoor()
{
//  Sound("DoorClose");
}
  
private func DoorClosed()
{
  return(1);
}

protected func Completion()
{
  SetEntrance(0);
  return(_inherited());
}

func Ejection(obj) {
  if (GetAction () eq "DoorOpen") SetAction ("DoorOpen");
  return( _inherited (obj) );
}

func Collection2(obj) {
  if (GetAction () eq "DoorOpen") SetAction ("DoorOpen");
 return( _inherited (obj) );
}
