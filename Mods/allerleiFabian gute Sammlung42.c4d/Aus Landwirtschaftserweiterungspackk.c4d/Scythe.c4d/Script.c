/* --- Sense --- */

#strict 2

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
		return 0;
	CoolDown += 10;
	ObjectSetAction(pByObj, "Throw");
	var pGras = FindObject2(Find_Exclude(this), Find_Distance(20), Find_Or(Find_ID(GRA1),Find_ID(FLWR)));
	if (pGras)
	{
 for(var cnt=0;cnt<15+Random(10);cnt++)
		CreateParticle("GrassBlade", RandomX(AbsX(GetX(pGras))-5, AbsX(GetX(pGras))+5), RandomX(AbsY(GetY(pGras))-5,AbsY(GetY(pGras))+5),RandomX(-20, 20), RandomX(-40, -20), 30 + Random(30), RGB(45, 162, 48), 0, 0);
RemoveObject(pGras);
CreateObject(DTFH);
		Sound("SenseHit");
	}

	var pLiv = FindObject2(Find_Exclude(pByObj), Find_NoContainer(),Find_Distance(12), Find_OCF(OCF_Alive));
	if (pLiv)
	{
		if (!Contained(pLiv))
			DoEnergy(-RandomX(5, 15), pLiv);
		CreateParticle("PSpark", GetX(pLiv) - GetX(), GetY(pLiv) - GetY(), 0, 0, 180, RGB(255, 50, 0));
		CreateParticle("PSpark", GetX(pLiv) - GetX(), GetY(pLiv) - GetY(), 0, 0, 100, RGB(255, 0, 0));
	}
	return 1;
}


public func CheckCooldown()
{
	if (CoolDown > 0)
		CoolDown--;
	return CoolDown;
}
/* Kann geschmiedet werden */
protected func IsAnvilProduct()
{
	return 1;
}