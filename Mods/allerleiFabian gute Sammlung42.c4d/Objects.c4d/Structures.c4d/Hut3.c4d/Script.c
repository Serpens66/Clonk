/*-- Hütte --*/

#strict 3

#include BAS4
#include DOOR
#include XFER  // für Uplink Transfer

public func Completion()
{
SetPlrViewRange(25);
}

  
/* Forschung */

public func GetResearchBase() { return(HUT2); }

