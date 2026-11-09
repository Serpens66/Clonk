/* --- Sense --- */

#strict 3

local CoolDown;

public func Initialize()
{
	return 1;
}

protected func Hit()
{
	return Sound("WoodHit*");
}

protected func Activate(object pByObj)
{
	if (CoolDown)
		return nil;
	S42MigrationAssign(CoolDown, "+=", 10);
	ObjectSetAction(pByObj, "Throw");
	var pGras = FindObject2(Find_Exclude(this), Find_Distance(20), Find_Or(Find_ID(GRA1),Find_ID(FLWR)));
	if (pGras)
	{
 for(var cnt=nil;(cnt ?? 0)<15+(Random(10) ?? 0);S42MigrationStep(cnt, 1, true))
		CreateParticle("GrassBlade", RandomX((AbsX(GetX(pGras)) ?? 0)-5, (AbsX(GetX(pGras)) ?? 0)+5), RandomX((AbsY(GetY(pGras)) ?? 0)-5,(AbsY(GetY(pGras)) ?? 0)+5),RandomX(-20, 20), RandomX(-40, -20), 30 + (Random(30) ?? 0), RGB(45, 162, 48), nil, nil);
RemoveObject(pGras);
CreateObject(DTFH);
		Sound("SenseHit");
	}

	var pLiv = FindObject2(Find_Exclude(pByObj), Find_NoContainer(),Find_Distance(12), Find_OCF(OCF_Alive));
	if (pLiv)
	{
		if (!Contained(pLiv))
			DoEnergy(-(RandomX(5, 15) ?? 0), pLiv);
		CreateParticle("PSpark", (GetX(pLiv) ?? 0) - (GetX() ?? 0), (GetY(pLiv) ?? 0) - (GetY() ?? 0), nil, nil, 180, RGB(255, 50, nil));
		CreateParticle("PSpark", (GetX(pLiv) ?? 0) - (GetX() ?? 0), (GetY(pLiv) ?? 0) - (GetY() ?? 0), nil, nil, 100, RGB(255, nil, nil));
	}
	return 1;
}


public func CheckCooldown()
{
	if ((CoolDown ?? 0) > 0)
		S42MigrationStep(CoolDown, -1, true);
	return CoolDown;
}
/* Kann geschmiedet werden */
protected func IsAnvilProduct()
{
	return 1;
}