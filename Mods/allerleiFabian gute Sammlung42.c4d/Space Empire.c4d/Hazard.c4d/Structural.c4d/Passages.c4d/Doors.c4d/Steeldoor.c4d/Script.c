/*-- Steeldoor --*/

#strict 3

#include GBDR

local Status;

public func CanBeLaserRecycled() { return(2); }

//Einstellungem

public func Initialize()
{
	SetAction("Close");
	_inherited();
	SetMaxDamage(80);
}

public func OnDmg(int iDmg, int iType) {

	// diese Tür ist nicht besonders dick
	iDmg ??= 0;
	iType ??= 0;
	if(Equal(iType, DMG_Projectile))	return(50);
	if(Equal(iType, DMG_Melee))	return(20);
	if(Equal(iType, DMG_Energy))	return(40);
	if(Equal(iType, DMG_Explosion))	return(nil);

	return(100);
}

public func OnOpen()
{
	if( Or(!!SEqual(GetAction(), "Open"), Status)) return(nil);

	SetAction("Open");
	SetSolidMask(nil);
	Status = true;
	//Sound
	Sound("SteeldoorOpen");
}

public func OnClose()
{
	if( Or(!!SEqual(GetAction(), "Close"), !Status)) return(nil);
	SetAction("Close");
	Status = nil;
	SetSolidMask(12, nil, 10, 30, 12, nil);
	Sound("SteeldoorClose");
}

public func OnDestroyed(iType)
{
	SetAction("Destroyed");
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (destroyed)
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"Destroyed\")";
}
