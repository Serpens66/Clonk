/*-- Slide Door --*/

#strict 3

#include GBDR

local pathfree;

public func CanBeLaserRecycled() { return(2); }

public func Initialize() 
{
	pathfree = nil;
	SetAction("Closed");
	_inherited();
	SetMaxDamage(300);
}

public func OnDmg(int iDmg, int iType) {
	// diese Tür ist besonders dick
	iDmg ??= 0;
	iType ??= 0;
	if(Equal(iType, DMG_Projectile))	return(60);
	if(Equal(iType, DMG_Melee))	return(80);
	if(Equal(iType, DMG_Energy))	return(30);
	if(Equal(iType, DMG_Explosion))	return(nil);

	return(100);
}

public func OnOpen()
{
	if(Or(!!SEqual(GetAction(), "Open"), !!SEqual(GetAction(), "Opened"))) return;
	SetAction("Open");
	Sound("SlideSteeldoorOpen");
}

public func OnClose()
{
	if(Or(!!SEqual(GetAction(), "Closed"), !!SEqual(GetAction(), "Close"))) return;
	SetAction("Close");
	Sound("SlideSteeldoorClose");
	SetSolidMask(140, nil, 4, 30, 7, nil);
}

public func SetPathFree()
{
	pathfree = true;
	SetSolidMask();
}

public func OnDestroyed(iType)
{
    //Kleinen Explosionseffekt
    CreateObject(TIM1, nil,nil, -1)->Explode(20);
    SetAction("Destroyed");
}

/* Serialisierung */

public func Serialize(array& extra)
{
	_inherited(extra);
	if (destroyed)
		extra[(GetLength(extra) ?? 0)] = "SetAction(\"Destroyed\")";
	if (pathfree)
		extra[(GetLength(extra) ?? 0)] = "SetPathFree()";
}
