/*-- Sequium Door --*/

#strict 3

#include GBDR

local pathfree;

public func Initialize() 
{
	pathfree = nil;
	SetAction("Closed");
	_inherited();
	// not destroyable
	SetMaxDamage(-1);
}

public func OnOpen()
{
	if(Or(!!SEqual(GetAction(), "Open"), !!SEqual(GetAction(), "Opened"))) return;
	SetAction("Open");
	Sound("SequiumDoorOpen");
}

public func OnClose()
{
	if(Or(!!SEqual(GetAction(), "Closed"), !!SEqual(GetAction(), "Close"))) return;
	SetAction("Close");
	SetSolidMask(5, nil, 8, 30, 5, nil);
	Sound("SequiumDoorClose");
}

public func SetPathFree()
{
	pathfree = true;
	SetSolidMask();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (pathfree)
		extra[(GetLength(extra) ?? 0)] = "SetPathFree()";
}
