/* --- Wassertank --- */

#strict 3

#include BAS1

local dFillLevel;
local dMaxFillLevel;
local dMat;
local pDirtyPipe;
local pLimitPipe;
local bInit;

public func Initialize()
{
	if (!bInit)
	{
		dFillLevel = nil;
		dMaxFillLevel = MaxFillLevel();
		SelectMaterial(nil, Material("Water"));
		pDirtyPipe = nil;
		pLimitPipe = nil;
		bInit = 1;
		SetEntrance(1);
	}
	return 1;
}

public func ShowContent()
{
	if ((GetCon() ?? 0) < 100)
		return nil;
	FillLiquids();
	var dFilled = (dFillLevel ?? 0) * 100 / (dMaxFillLevel ?? 0);
	var cR = 255 * (Min(100, Max(nil, 200 - (dFilled ?? 0) * 2)) ?? 0) / 100, cG = 255 * (Min(100, (dFilled ?? 0) * 2) ?? 0) / 100;
	Message("@<c %x>%s:</c>|<c %x>%d%</c>", this, GetMaterialColorX(dMat, 1), MaterialName(dMat), RGBa(cR, cG, nil, nil), dFilled);
	//Grafik
	if (S42MigrationEqual(dMat, -1))
	{
		SetGraphics("", this, nil, 1);
		SetGraphics("", this, nil, 2);
	}
	else
	{
		var sFillState = "Empty";
		if ((dFilled ?? 0) > 66)
			sFillState = "Full";
		else if (Inside(dFilled, 33, 66))
			sFillState = "Half";
		SetGraphics(sFillState, this, GetID(), 1, GFXOV_MODE_Base);
		SetGraphics(sFillState, this, GetID(), 2, GFXOV_MODE_Picture);
		SetClrModulation(RGB(GetMaterialColor(dMat, nil, nil), GetMaterialColor(dMat, nil, 1), GetMaterialColor(dMat, nil, 2)), this, 1);
		SetClrModulation(RGB(GetMaterialColor(dMat, nil, nil), GetMaterialColor(dMat, nil, 1), GetMaterialColor(dMat, nil, 2)), this, 2);
	}
	return 1;
}

private func FillLiquids()
{
	if (S42MigrationEqual(dFillLevel, nil))
		return nil;
	var idBarrel = GetBarrelType(dMat);
	if (!idBarrel)
		return nil;
	var pBarrel;
	//Fässer für Material
	while (pBarrel = FindObject(idBarrel, nil, nil, nil, nil, nil, nil, nil, this, pBarrel))
		if (!pBarrel->BarrelIsFull())
		{
			S42MigrationAssign(dFillLevel, "-=", (pBarrel->BarrelDoFill(dFillLevel, dMat) ?? 0));
			if (pBarrel->BarrelIsFull())
				Exit(pBarrel, nil, 25, nil, nil, nil, nil);
			else
				return 1;
		}
	if (S42MigrationEqual(dFillLevel, nil))
		return 1;
	//Leere Fässer
	var idEmptyBarrel = EmptyBarrelID();
	pBarrel = nil;
	if (idEmptyBarrel)
	{
		while (pBarrel = FindObject(idEmptyBarrel, nil, nil, nil, nil, nil, nil, nil, this, pBarrel))
			if (!pBarrel->BarrelIsFull())
			{
				ChangeDef(idBarrel, pBarrel);
				S42MigrationAssign(dFillLevel, "-=", (pBarrel->BarrelDoFill(dFillLevel, dMat) ?? 0));
				if (pBarrel->BarrelIsFull())
					Exit(pBarrel, nil, 25, nil, nil, nil, nil);
				else
					return 1;
			}
	}
	return 1;
}

public func AlternativeName()
{
	if (((dMat ?? 0) < 0) || (!bInit))
		return nil;
	return Format("%s-%s", MaterialName(dMat), GetName(this, nil, 1));
}

public func ContextSelectMaterial(object pClonk)
{
	[$TxtSelectMaterial$|Image=DTV1|Condition=IsEmpty]
	return MaterialMenu(pClonk);
}

public func IsEmpty()
{
	return S42MigrationEqual(dFillLevel, nil);
}

public func ContextSelectDirtyPipe(object pClonk)
{
	[$TxtSelectDirtyPipe$|Image=DPIP|Condition=HasDrainPipe]
	return DirtyPipeMenu(pClonk);
}

public func ContextSelectLimitPipe(object pClonk)
{
	[$TxtSelectLimitPipe$|Image=SPIP|Condition=HasDrainPipe]
	return LimitPipeMenu(pClonk);
}

public func HasDrainPipe()
{
	return FindObject(DPIP, nil, nil, nil, nil, nil, "Connect", this);
}

public func ContextEjectContent(object pClonk)
{
	[$TxtEjectContent$|Image=DTFF|Condition=!IsEmpty]
	return EjectContentMenu(pClonk);
}

public func IsProducerOf(object pClonk, id idDef)
{
	if (!idDef)
		return nil;
	if (!this)
	{
		if (!idDef->~BarrelMaterialName())
			return nil;
		var dtMat;
		if (dtMat = idDef->~BarrelMaterial())
			return S42MigrationEqual(GetMaterialVal("Density", "Material", dtMat), 25);
		return nil;
	}
	if (S42MigrationEqual(idDef, GetBarrelType(dMat)))
		if ((dFillLevel ?? 0) >= (idDef->~BarrelMaxFill() ?? 0))
			return 1;
	return nil;
}

public func HowToProduce(object pClonk, id idDef)
{
	AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 10);
	if (S42MigrationEqual(idDef, GetBarrelType(dMat)))
		return ProductionHelperBarrel(nil, pClonk, (GetDefinitionIndex(idDef) ?? 0) + 1);
	return nil;
}

public func ProductionHelperBarrel(Foo, object pClonk, int dIndexPlusEins)
{
	dIndexPlusEins ??= 0;
	ScheduleCall(this, "DelayedProductionHelperBarrel", 1, nil, pClonk, dIndexPlusEins);
	return 1;
}

public func DelayedProductionHelperBarrel(object pClonk, int dIndexPlusEins)
{
	dIndexPlusEins ??= 0;
	if (!dIndexPlusEins)
		return nil;
	var idDef = GetDefinition((dIndexPlusEins ?? 0) - 1);
	if (!GetName(nil, idDef))
		return nil;
	if (!IsProducerOf(pClonk, idDef))
		return nil;
	var pObj;
	if (pObj = FindObject(EmptyBarrelID(), nil, nil, nil, nil, nil, nil, nil, pClonk))
	{
		AddCommand(pClonk, "Enter", this, nil, nil, nil, nil, nil);
		AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 50);
	}
	else
	{
		AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, nil, nil, "ProductionHelperBarrel");
		AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 50);
		AddCommand(pClonk, "Acquire", nil, nil, nil, nil, nil, EmptyBarrelID());
		AddCommand(pClonk, "Wait", nil, nil, nil, nil, nil, 50);
	}
	return 1;
}

protected func MaterialMenu(object pClonk)
{
	CloseMenu(pClonk);
	CreateMenu(DTV1, pClonk, this, nil, "$TxtSelectMaterial$", nil, 1);
	AddMenuItem("$TxtNoMaterial$", "SelectMaterial", DPIP, pClonk, nil, -2, "$TxtNoMaterialDesc$");
	var i = -1;
	while (!S42MigrationEqual(Material(MaterialName(S42MigrationStep(i, 1))), -1))
		if (IsContainerforMat(i))
			AddMenuItem(Format("$TxtMaterial$", MaterialName(i)), "SelectMaterial", DPIP, pClonk, nil, i, Format("$TxtMaterialDesc$", MaterialName(i)));
	return 1;
}

protected func SelectMaterial(Foo, int dnMat)
{
	dnMat ??= 0;
	if (!IsEmpty())
		return nil;
	dMat = dnMat;
	ShowContent();
	return 1;
}

protected func DirtyPipeMenu(object pClonk)
{
	CloseMenu(pClonk);
	CreateMenu(DPIP, pClonk, this, nil, "$TxtSelectDirtyPipe$", nil, 1, 1, DPIP);
	AddMenuItem("$TxtNoDirtyPipe$", "SelectPipe", DPIP, pClonk, nil, pClonk, "$TxtNoDirtyPipeDesc$", 128, nil, nil);
	var pLine = nil, pTarget;
	while (pLine = FindObject(DPIP, nil, nil, nil, nil, nil, "Connect", this, NoContainer(), pLine))
	{
		if (!pLine)
			continue;
		pTarget = GetActionTarget(1, pLine);
		if ((!pTarget) || (S42MigrationEqual(pTarget, this)))
			continue;
		AddMenuItem(Format("$TxtDirtyPipe$", GetName(pTarget)), "SelectPipe", GetID(pTarget), pClonk, nil, pClonk, Format("$TxtDirtyPipeDesc$", GetName(pTarget)), 128, nil, ObjectNumber(pLine));
	}
	return 1;
}

protected func LimitPipeMenu(object pClonk)
{
	CloseMenu(pClonk);
	CreateMenu(DPIP, pClonk, this, nil, "$TxtSelectLimitPipe$", nil, 1, 1, SPIP);
	AddMenuItem("$TxtNoLimitPipe$", "SelectPipe", SPIP, pClonk, nil, pClonk, "$TxtNoLimitPipeDesc$", 128, nil, nil);
	var pLine = nil, pTarget;
	while (pLine = FindObject(DPIP, nil, nil, nil, nil, nil, "Connect", this, NoContainer(), pLine))
	{
		if (!pLine)
			continue;
		pTarget = GetActionTarget(1, pLine);
		if ((!pTarget) || (S42MigrationEqual(pTarget, this)))
			continue;
		AddMenuItem(Format("$TxtLimitPipe$", GetName(pTarget)), "SelectPipe", GetID(pTarget), pClonk, nil, pClonk, Format("$TxtLimitPipeDesc$", GetName(pTarget)), 132, this, ObjectNumber(pLine));
	}
	return 1;
}

protected func EjectContentMenu(object pClonk)
{
	SetAction("Idle", nil, nil);
	CloseMenu(pClonk);
	CreateMenu(DPIP, pClonk, this, nil, "$TxtEjectContent$", nil, 1, 1, DTFF);
	AddMenuItem("$TxtNoEjection$", "SelectPipe", DTFF, pClonk, nil, pClonk, "$TxtNoEjectionDesc$", 128, nil, -1);
	AddMenuItem("$TxtNoEjectPipe$", "SelectPipe", DTFF, pClonk, nil, pClonk, "$TxtNoEjectPipeDesc$", 132, this, ObjectNumber(this));
	var pLine = nil, pTarget;
	while (pLine = FindObject(DPIP, nil, nil, nil, nil, nil, "Connect", this, NoContainer(), pLine))
	{
		if (!pLine)
			continue;
		pTarget = GetActionTarget(1, pLine);
		if ((!pTarget) || (S42MigrationEqual(pTarget, this)))
			continue;
		AddMenuItem(Format("$TxtEjectPipe$", GetName(pTarget)), "SelectPipe", GetID(pTarget), pClonk, nil, pClonk, Format("$TxtEjectPipeDesc$", GetName(pTarget)), 132, this, ObjectNumber(pLine));
	}
	return 1;
}

protected func SelectPipe(Foo, object pClonk, bool bSpezial, int dObject)
{
	bSpezial ??= false;
	dObject ??= 0;
	if (!pClonk)
		return nil;
	var idDef = GetMenu(pClonk);
	if (!idDef)
		return nil;
	if (((dObject ?? 0) <= 0) || (!idDef))
	{
		if (S42MigrationEqual(idDef, SPIP))
			pLimitPipe = nil;
		if (S42MigrationEqual(idDef, DPIP))
			pDirtyPipe = nil;
		CloseMenu(pClonk);
		return 1;
	}
	var pLine = Object(dObject);
	if (!pLine)
		return nil;
	if (!bSpezial)
	{
		if (S42MigrationEqual(idDef, SPIP))
			pLimitPipe = pLine;
		if (S42MigrationEqual(idDef, DPIP))
			pDirtyPipe = pLine;
		if (S42MigrationEqual(idDef, DTFF))
			SetAction("Eject", pLine, pClonk);
		CloseMenu(pClonk);
	}
	else
	{
		SetPlrView(GetOwner(pClonk), GetActionTarget(1, pLine));
		CreateParticle("MSpark", GetX(GetActionTarget(1, pLine)), GetY(GetActionTarget(1, pLine)), nil, 15, 50, RGB(255, 255, 255));
	}
	return 1;
}

protected func EjectContent(Foo, bool bVerify)
{
	bVerify ??= false;
	var pTarget = GetActionTarget(nil), pVerify = GetActionTarget(1);
	SetActionTargets(pTarget, nil);
	if ((pVerify) && (!bVerify))
	{
		CloseMenu(pVerify);
		CreateMenu(DTFF, pVerify, this, nil, "$TxtEjectPipe$", nil, 1, nil);
		AddMenuItem("$TxtNoEjection$", "SelectPipe", DTFF, pVerify, nil, pVerify, "$TxtNoEjectionDesc$");
		AddMenuItem("$TxtEjectContent$", "EjectContent", SPIP, pVerify, nil, 1, "$TxtEjectContentDesc$");
		return 1;
	}
	if (S42MigrationEqual(dFillLevel, nil))
	{
		SetAction("Idle", nil, nil);
		return 1;
	}
	if (S42MigrationEqual(pTarget, this))
	{
		var i = nil;
		while ((S42MigrationStep(i, 1, true) < 10) && ((LiquidOutput(-2, 1) ?? 0) >= 0))
			InsertMaterial(dMat);
	}
	else
	{
		var i = nil;
		while ((S42MigrationStep(i, 1, true) < 10) && ((LiquidOutput(-2, 1) ?? 0) >= 0))
			PipeTransfer(pTarget, dMat, 1);
	}
	return 1;
}

public func MaxFillLevel()
{
	return 5000;
}

public func IsContainerforMat(int dnMat)
{
	dnMat ??= 0;
	if (S42MigrationEqual(Material(MaterialName(dnMat)), -1))
		return nil;
	if (!S42MigrationEqual(GetMaterialVal("Density", "Material", dnMat), 25))
		return nil;
	return 1;
}

public func IsLiquidContainer()
{
	return 1;
}

public func CheckOwnCapacity(int dnMat, int dnAmount, int dAntiStuck)
{
	dnMat ??= 0;
	dnAmount ??= 0;
	dAntiStuck ??= 0;
	if (((dnMat ?? 0) < 0) || (S42MigrationEqual(dnAmount, nil)))
		return nil;
	if (S42MigrationEqual(GetAction(), "Eject"))
		return nil;
	if (!S42MigrationEqual(dnMat, dMat))
	{
		if (IsContainerforMat(dMat))
			return PipeCheck(pDirtyPipe, dnMat, dnAmount, dAntiStuck);
		return nil;
	}
	if ((dFillLevel ?? 0) + (dnAmount ?? 0) > (dMaxFillLevel ?? 0) + 5)
		return PipeCheck(pLimitPipe, dnMat, dnAmount, dAntiStuck);
	return 1;
}

private func PipeCheck(object pPipe, int dnMat, int dnAmount, int dAntiStuck)
{
	dnMat ??= 0;
	dnAmount ??= 0;
	dAntiStuck ??= 0;
	if ((!pPipe) || ((dAntiStuck ?? 0) > 100))
		return nil;
	if (!S42MigrationEqual(this, GetActionTarget(nil, pPipe)))
		return nil;
	var pPumpTarget = GetActionTarget(1, pPipe);
	if ((!pPumpTarget) || (S42MigrationEqual(GetID(pPumpTarget), LNKT)))
		return 1;
	if (pPumpTarget->~IsLiquidContainer())
		return pPumpTarget->~CheckOwnCapacity(dnMat, dnAmount, S42MigrationStep(dAntiStuck, 1, true));
	if (GetBarrelType(dnMat))
	{
		for (var pContent in FindObjects(Find_Container(pPumpTarget), Find_ID(GetBarrelType(dnMat)))) 
		{
			if (pContent->~BarrelIsFull())
				continue;
			return 1;
		}
		if (EmptyBarrelID())
			if (ObjectCount(EmptyBarrelID(), nil, nil, nil, nil, nil, nil, nil, pPumpTarget))
				return 1;
	}
	return nil;
}

public func LiquidOutput(int dnMat, int dnAmount, object pnLine)
{
	dnMat ??= 0;
	dnAmount ??= 0;
	if (S42MigrationEqual(dnAmount, nil))
		return -2;
	if (S42MigrationEqual(dMat, -1))
		return -2;
	if ((!S42MigrationEqual(dnMat, dMat)) && (!S42MigrationEqual(dnMat, -2)))
		return -2;
	FillLiquids();
	
	if ((dFillLevel ?? 0) - (dnAmount ?? 0) < 0)
		return -2;
	if (pnLine)
	{
		if (S42MigrationEqual(GetID(pnLine), DPIP))
		{
			if ((S42MigrationEqual(pnLine, pLimitPipe)) || (S42MigrationEqual(pnLine, pDirtyPipe)))
				return -2;
			if (!S42MigrationEqual(GetActionTarget(nil, pnLine), this))
				return -2;
			if (!PUMP->~CheckCapacity(GetActionTarget(1, pnLine), dnAmount, dMat))
				return -2;
		}
		else
		{
			if ((S42MigrationEqual(pnLine, GetActionTarget(1, pLimitPipe))) || (S42MigrationEqual(pnLine, GetActionTarget(1, pDirtyPipe))))
				return -2;
			if ((S42MigrationEqual(GetID(pnLine), DTFF)) && ((dFillLevel ?? 0) < (dMaxFillLevel ?? 0)))
				return -2;
			if (pnLine->~IsLiquidConatiner())
				if (!pnLine->~CheckOwnCapacity(dMat, dnAmount))
					return -2;
		}
	}
	S42MigrationAssign(dFillLevel, "-=", (dnAmount ?? 0));
	ShowContent();
	return dMat;
}

public func LiquidInput(int dnMat, int dnAmount, object pnLine)
{
	dnMat ??= 0;
	dnAmount ??= 0;
	if (((dnMat ?? 0) < 0) || (S42MigrationEqual(dnAmount, nil)))
		return nil;
	if (!S42MigrationEqual(dnMat, dMat))
	{
		if (IsContainerforMat(dMat))
			return PipeTransfer(pDirtyPipe, dnMat, dnAmount);
		return nil;
	}
	if ((dFillLevel ?? 0) + (dnAmount ?? 0) > (dMaxFillLevel ?? 0) + 5)
		return PipeTransfer(pLimitPipe, dnMat, dnAmount);
	S42MigrationAssign(dFillLevel, "+=", (dnAmount ?? 0));
	ShowContent();
	return 1;
}

protected func PipeTransfer(object pPipe, int dnMat, int dnAmount)
{
	dnMat ??= 0;
	dnAmount ??= 0;
	if (!pPipe)
		return nil;
	// Angeschlossene Objekte ermitteln
	var pPumpSource = GetActionTarget(nil, pPipe);
	// Kein Quellobjekt
	if ((!pPumpSource) || (!S42MigrationEqual(pPumpSource, this)))
		return nil;
	// Quellobjekt hat nicht genug Energie
	if (!EnergyCheck(1000, pPumpSource))
		return nil;
	// Zielobjekt vorhanden
	var i;
	var pPumpTarget = GetActionTarget(1, pPipe);
	if (pPumpTarget)
	{
		for (i = nil; (i ?? 0) < (dnAmount ?? 0); S42MigrationStep(i, 1, true))
			ObjectInsertMaterial(dnMat, pPumpTarget);
	}
	else
	{
		for (i = nil; (i ?? 0) < (dnAmount ?? 0); S42MigrationStep(i, 1, true))
			InsertMaterial(dnMat, GetVertex(50, nil), GetVertex(50, 1));
	}
	return 1;
}

public func LineConnectType()
{
	return DPIP;
}

public func Collection2(object pObj)
{
	for (var pContent in FindObjects(Find_Container(pObj), Find_ID(EmptyBarrelID()))) 
	{
		Exit(pContent, nil, 25, nil, nil, nil, nil);
		Enter(this, pContent);
	}
	for (var pContent in FindObjects(Find_Container(pObj), Find_ID(GetBarrelType(dMat)))) 
	{
		if (pContent->~BarrelIsFull())
			continue;
		Exit(pContent, nil, 25, nil, nil, nil, nil);
		Enter(this, pContent);
	}
	if (((pObj->~BarrelMaxFill() ?? 0) <= 0) || (pObj->~BarrelIsFull()))
	{
		Exit(pObj, nil, 25, nil, nil, nil, nil);
		return 1;
	}
	for (var pContent in FindObjects(Find_Container(pObj))) 
	{
		Exit(pContent, nil, 25, nil, nil, nil, nil);
	}
	ShowContent();
	return 1;
}

public func NoLorryEjection(object pLorry)
{
	return 1;
}

//public func GetResearchBase()
//{
//	return PUMP;
//}