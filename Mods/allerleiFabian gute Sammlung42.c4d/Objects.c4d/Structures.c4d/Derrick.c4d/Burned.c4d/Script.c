/*-- Verbrannter Bohrturm --*/

#strict 3

#include DRBS

local pDrillHead;
local pBarrel;

protected func Incineration() { if (pBarrel) RemoveObject(pBarrel); pBarrel=nil; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=nil; }
protected func IncinerationEx() { if (pBarrel) RemoveObject(pBarrel); pBarrel=nil; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=nil; }
protected func Destruction() { if (pBarrel) RemoveObject(pBarrel); pBarrel=nil; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=nil; return(_inherited() ); }
