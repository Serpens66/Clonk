/* --- Bergwerk --- */

#strict 3

#include DOOR

local dDepth,dSupport;
local pRule;

protected func Initialize()
{
	dSupport = 5;
	pRule = FindObject(DTBB);
	if (!pRule)
		pRule = CreateObject(DTBB, -(GetX() ?? 0), -(GetY() ?? 0), NO_OWNER);
	if ((GetY() ?? 0) > (LandscapeHeight() ?? 0) - 100)
		return 1;
	Message("$TxtWrongPlacement$", this, GetY(), (LandscapeHeight() ?? 0) - 100);
	Sound("Error");
	ChangeDef(CNKT);
	return 1;
}


/* Produktion*/

public func IsProducerOf(object pClonk, id idDef)
{
	return IsMineProduct(idDef);
}

public func HowToProduce(object pClonk, id idDef)
{
	AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 10);
	if (!IsMineProduct(idDef))
		return nil;
	return Waiting(nil, pClonk, GetDefinitionIndex(idDef));
}


public func RejectConstruction(int dX, int dY, object pClonk)
{
	dX ??= 0;
	dY ??= 0;
	if ((GetY(pClonk) ?? 0) > (LandscapeHeight() ?? 0) - 50)
		return nil;
	Message("$TxtWrongPlacement$", this, (GetY(pClonk) ?? 0) + (dY ?? 0) + 50, (LandscapeHeight() ?? 0) - 100);
	return 1;
}

protected func Mining(Foo, object pClonk, int dIndexPlusEins)
{
	dIndexPlusEins ??= 0;
	ScheduleCall(this, "DelayedMining", 1, nil, pClonk, dIndexPlusEins);
	return 1;
}

protected func DelayedMining(object pClonk, int dIndexPlusEins)
{
	dIndexPlusEins ??= 0;
	if (!pClonk)
		return nil;
	var idDef = nil;
	if (dIndexPlusEins)
		idDef = GetDefinition((dIndexPlusEins ?? 0) - 1);
	if (!IsMineProduct(idDef))
	{
		idDef = nil;
		dIndexPlusEins = nil;
	}
	if (!S42MigrationEqual(Contained(pClonk), this))
		return Waiting(nil, pClonk, dIndexPlusEins);
	//if (Random(RandomIdle()))
	//	return 1;
	CreateContents(GetMineProductID(nil, idDef));
	S42MigrationStep(dDepth, 1, true);
	Waiting(nil, pClonk, dIndexPlusEins);
	return 1;
}

protected func Waiting(Foo, object pClonk, int dIndexPlusEins)
{
	dIndexPlusEins ??= 0;
	if (!pClonk)
		return nil;
	CloseMenu(pClonk);
	var idDef = nil;
	if (dIndexPlusEins)
		idDef = GetDefinition((dIndexPlusEins ?? 0) - 1);
	if (!IsMineProduct(idDef))
	{
		idDef = nil;
		dIndexPlusEins = nil;
	}
	if (!S42MigrationEqual(Contained(pClonk), this))
	{
		AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, nil, nil, "Waiting");
		EasyCommandEnter(pClonk);
		return 1;
	}
	if (!BuildSupport())
	{
		//AcquireSupport siehe Hochofen
		AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, nil, nil, "Waiting");
		AddCommand(pClonk, "Call", this, nil, nil, nil, nil, "SupportWithWOOD", nil, 1);
		AddCommand(pClonk, "Put", this, nil, nil, nil, nil, WOOD);
		AddCommand(pClonk, "Acquire", nil, nil, nil, this, nil, WOOD);
		return 1;
	}
	AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, nil, nil, "Mining");
	AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 37 * 15);
	return 1;
}

public func SupportWithWOOD()
{
	return 1;
}

public func SupportWithWOODFailed(object pClonk)
{
	// Hat nicht geklappt, also mit Beton versuchen
	AddCommand(pClonk, "Call", this, nil, nil, nil, nil, "SupportWithCNCR", nil, 1);
	AddCommand(pClonk, "Put", this, nil, nil, nil, nil, CNCR);
	AddCommand(pClonk, "Acquire", nil, nil, nil, this, nil, CNCR);
	return 1;
}

public func SupportWithCNCR()
{
	return 1;
}

public func SupportWithCNCRFailed(object pClonk)
{
	// Hat nicht geklappt, nur warten.
	AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 37 * 15);
	return 1;
}

protected func BuildSupport()
{
	if (!dSupport)
	{
		var pSupport;
		if (pSupport = FindObject(CNCR, nil, nil, nil, nil, nil, nil, nil, this))
		{
			S42MigrationAssign(dSupport, "+=", 3);
			RemoveObject(pSupport);
		}
		else if (pSupport = FindObject(WOOD, nil, nil, nil, nil, nil, nil, nil, this))
		{
			S42MigrationAssign(dSupport, "+=", 1);
			RemoveObject(pSupport);
		}
	}
	if (!dSupport)
		return nil;
	S42MigrationStep(dSupport, -1, true);
	return (dSupport ?? 0) + 1;
}

protected func ContextProduce(object pClonk)
{
	[$TxtProduction$|Image=GOLD]
	return Production(pClonk);
}

protected func Production(object pClonk)
{
	if (!pClonk)
		return nil;
	CloseMenu(pClonk);
	CreateMenu(GOLD, pClonk, this, nil, "$TxtSelectRess$", nil, 1, nil);
	var idaRess = GetMineProductList();
	AddMenuItem("$TxtProductionDefault$", Format("Waiting(0,Object(%d),0)", ObjectNumber(pClonk)), CLNK, pClonk, nil, nil, "$TxtProductionDefaultDesc$");
	for (var idDef in idaRess) 
		AddMenuItem(GetName(nil, idDef), Format("Waiting(0,Object(%d),%d)", ObjectNumber(pClonk), (GetDefinitionIndex(idDef) ?? 0) + 1), idDef, pClonk, nil, nil, Format("$TxtProductionRessDesc$", GetName(nil, idDef), (GetMineValue(idDef) ?? 0) * 5, (GetMineTotalValue() ?? 0) + (GetMineValue(idDef) ?? 0) * 5));
	return 1;
}

protected func IsMineProduct(id idDef)
{
	if (!pRule)
		return nil;
	if (!idDef)
		return nil;
	return pRule->~IsMineProduct(idDef);
}

protected func GetMineValue(id idDef)
{
	if (!pRule)
		return nil;
	if (!idDef)
		return nil;
	return pRule->~GetMineValue(idDef);
}

protected func GetMineProductList()
{
	if (!pRule)
		return [];
	return pRule->~GetMineProductList();
}

protected func GetMineTotalValue()
{
	if (!pRule)
		return nil;
	return pRule->~GetMineTotalValue();
}

protected func GetMineProductID(int dnRandom, id idnPref)
{
	dnRandom ??= 0;
	if (!pRule)
		return nil;
	if (!dnRandom)
		dnRandom = Random((GetMineTotalValue() ?? 0) + (GetMineValue(idnPref) ?? 0) * 4);
	if ((dnRandom ?? 0) > (GetMineTotalValue() ?? 0))
		return idnPref;
	var i = -1, idtRess = GetMineProductList();
	while ((dnRandom ?? 0) >= 0)
	{
		S42MigrationStep(i, 1, true);
		S42MigrationAssign(dnRandom, "-=", (GetMineValue(idtRess[(i ?? 0)]) ?? 0));
	}
	return idtRess[(i ?? 0)];
}

private func EasyCommandEnter(object pClonk)
{
	AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 50);
	AddCommand(pClonk, "Put", this, 100);
	AddCommand(pClonk, "Enter", this);
	return 1;
}

public func GetDepth()
{
	return dDepth;
}