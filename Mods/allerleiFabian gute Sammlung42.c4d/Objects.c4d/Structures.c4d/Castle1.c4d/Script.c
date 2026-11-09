/*-- Kleine Burg --*/

#strict

#include BAS5
#include DOOR
#include XFER  // für Uplink Transfer

private func SoundOpenDoor()
{
  Sound("GateOpen");
}
  
private func SoundCloseDoor()
{
  Sound("GateClose");
}

