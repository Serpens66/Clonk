/* --- Bergwerk --- */

#strict 2

#include DOOR

local dDepth,dSupport;
local pRule;

protected func Initialize()
{
	dSupport = 5;
	pRule = FindObject(DTBB);
	if (!pRule)
		pRule = CreateObject(DTBB, -GetX(), -GetY(), NO_OWNER);
	if (GetY() > LandscapeHeight() - 100)
		return 1;
	Message("$TxtWrongPlacement$", this, GetY(), LandscapeHeight() - 100);
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
	AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 10);
	if (!IsMineProduct(idDef))
		return 0;
	return Waiting(0, pClonk, GetDefinitionIndex(idDef));
}


public func RejectConstruction(int dX, int dY, object pClonk)
{
	if (GetY(pClonk) > LandscapeHeight() - 50)
		return 0;
	Message("$TxtWrongPlacement$", this, GetY(pClonk) + dY + 50, LandscapeHeight() - 100);
	return 1;
}

protected func Mining(Foo, object pClonk, int dIndexPlusEins)
{
	ScheduleCall(this, "DelayedMining", 1, 0, pClonk, dIndexPlusEins);
	return 1;
}

protected func DelayedMining(object pClonk, int dIndexPlusEins)
{
	if (!pClonk)
		return 0;
	var idDef = 0;
	if (dIndexPlusEins)
		idDef = GetDefinition(dIndexPlusEins - 1);
	if (!IsMineProduct(idDef))
	{
		idDef = 0;
		dIndexPlusEins = 0;
	}
	if (Contained(pClonk) != this)
		return Waiting(0, pClonk, dIndexPlusEins);
	//if (Random(RandomIdle()))
	//	return 1;
	CreateContents(GetMineProductID(0, idDef));
	dDepth++;
	Waiting(0, pClonk, dIndexPlusEins);
	return 1;
}

protected func Waiting(Foo, object pClonk, int dIndexPlusEins)
{
	if (!pClonk)
		return 0;
	CloseMenu(pClonk);
	var idDef = 0;
	if (dIndexPlusEins)
		idDef = GetDefinition(dIndexPlusEins - 1);
	if (!IsMineProduct(idDef))
	{
		idDef = 0;
		dIndexPlusEins = 0;
	}
	if (Contained(pClonk) != this)
	{
		AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, 0, 0, "Waiting");
		EasyCommandEnter(pClonk);
		return 1;
	}
	if (!BuildSupport())
	{
		//AcquireSupport siehe Hochofen
		AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, 0, 0, "Waiting");
		AddCommand(pClonk, "Call", this, 0, 0, 0, 0, "SupportWithWOOD", 0, 1);
		AddCommand(pClonk, "Put", this, 0, 0, 0, 0, WOOD);
		AddCommand(pClonk, "Acquire", 0, 0, 0, this, 0, WOOD);
		return 1;
	}
	AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, 0, 0, "Mining");
	AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 37 * 15);
	return 1;
}

public func SupportWithWOOD()
{
	return 1;
}

public func SupportWithWOODFailed(object pClonk)
{
	// Hat nicht geklappt, also mit Beton versuchen
	AddCommand(pClonk, "Call", this, 0, 0, 0, 0, "SupportWithCNCR", 0, 1);
	AddCommand(pClonk, "Put", this, 0, 0, 0, 0, CNCR);
	AddCommand(pClonk, "Acquire", 0, 0, 0, this, 0, CNCR);
	return 1;
}

public func SupportWithCNCR()
{
	return 1;
}

public func SupportWithCNCRFailed(object pClonk)
{
	// Hat nicht geklappt, nur warten.
	AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 37 * 15);
	return 1;
}

protected func BuildSupport()
{
	if (!dSupport)
	{
		var pSupport;
		if (pSupport = FindObject(CNCR, 0, 0, 0, 0, 0, 0, 0, this))
		{
			dSupport += 3;
			RemoveObject(pSupport);
		}
		else if (pSupport = FindObject(WOOD, 0, 0, 0, 0, 0, 0, 0, this))
		{
			dSupport += 1;
			RemoveObject(pSupport);
		}
	}
	if (!dSupport)
		return 0;
	dSupport--;
	return dSupport + 1;
}

protected func ContextProduce(object pClonk)
{
	[$TxtProduction$|Image=GOLD]
	return Production(pClonk);
}

protected func Production(object pClonk)
{
	if (!pClonk)
		return 0;
	CloseMenu(pClonk);
	CreateMenu(GOLD, pClonk, this, 0, "$TxtSelectRess$", 0, 1, 0);
	var idaRess = GetMineProductList();
	AddMenuItem("$TxtProductionDefault$", Format("Waiting(0,Object(%d),0)", ObjectNumber(pClonk)), CLNK, pClonk, 0, 0, "$TxtProductionDefaultDesc$");
	for (var idDef in idaRess) 
		AddMenuItem(GetName(0, idDef), Format("Waiting(0,Object(%d),%d)", ObjectNumber(pClonk), GetDefinitionIndex(idDef) + 1), idDef, pClonk, 0, 0, Format("$TxtProductionRessDesc$", GetName(0, idDef), GetMineValue(idDef) * 5, GetMineTotalValue() + GetMineValue(idDef) * 5));
	return 1;
}

protected func IsMineProduct(id idDef)
{
	if (!pRule)
		return 0;
	if (!idDef)
		return 0;
	return pRule->~IsMineProduct(idDef);
}

protected func GetMineValue(id idDef)
{
	if (!pRule)
		return 0;
	if (!idDef)
		return 0;
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
		return 0;
	return pRule->~GetMineTotalValue();
}

protected func GetMineProductID(int dnRandom, id idnPref)
{
	if (!pRule)
		return 0;
	if (!dnRandom)
		dnRandom = Random(GetMineTotalValue() + GetMineValue(idnPref) * 4);
	if (dnRandom > GetMineTotalValue())
		return idnPref;
	var i = -1, idtRess = GetMineProductList();
	while (dnRandom >= 0)
	{
		i++;
		dnRandom -= GetMineValue(idtRess[i]);
	}
	return idtRess[i];
}

private func EasyCommandEnter(object pClonk)
{
	AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 50);
	AddCommand(pClonk, "Put", this, 100);
	AddCommand(pClonk, "Enter", this);
	return 1;
}

public func GetDepth()
{
	return dDepth;
}