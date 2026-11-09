/*-- Verbrannte Pumpe --*/

#strict 3

#include BAS1

local Barrel;

protected func Destruction() { if(Barrel) RemoveObject(Barrel); return(_inherited() ); }
protected func Incineration() { if(Barrel) RemoveObject(Barrel); return(_inherited() ); }
protected func IncinerationEx() { if(Barrel) RemoveObject(Barrel); return(_inherited() ); }

