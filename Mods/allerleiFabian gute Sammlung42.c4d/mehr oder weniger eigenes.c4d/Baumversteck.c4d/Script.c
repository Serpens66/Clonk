/*-- Baum --*/

#strict 3

/*-- Eingangssteuerung --*/

func Completion() {
  SetCategory(C4D_Vehicle);
  return(1);
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
  SetEntrance(nil);
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
  SetEntrance(nil);
  return(_inherited());
}

func Ejection(obj) {
  if (!!SEqual(GetAction (), "DoorOpen")) SetAction ("DoorOpen");
  return( _inherited (obj) );
}

func Collection2(obj) {
  if (!!SEqual(GetAction (), "DoorOpen")) SetAction ("DoorOpen");
 return( _inherited (obj) );
}
