/*-- Bambushütte --*/

#strict

#include DOOR
#include XFER  // für Uplink Transfer

protected func SoundOpenDoor() { return(1); }
protected func SoundCloseDoor() { return(1); }

// Kann im Dschungel erforscht werden
public func IsJungleResearch() { return(1); }

