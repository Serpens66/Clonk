/*-- Eingangssteuerung --*/

#strict 3

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
  Sound("DoorOpen");
}

private func SoundCloseDoor()
{
  Sound("DoorClose");
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
