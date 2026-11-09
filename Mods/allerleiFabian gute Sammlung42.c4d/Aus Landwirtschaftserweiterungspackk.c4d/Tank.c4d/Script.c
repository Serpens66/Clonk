/* --- Wassertank --- */

#strict 2

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
		dFillLevel = 0;
		dMaxFillLevel = MaxFillLevel();
		SelectMaterial(0, Material("Water"));
		pDirtyPipe = 0;
		pLimitPipe = 0;
		bInit = 1;
		SetEntrance(1);
	}
	return 1;
}

public func ShowContent()
{
	if (GetCon() < 100)
		return 0;
	FillLiquids();
	var dFilled = dFillLevel * 100 / dMaxFillLevel;
	var cR = 255 * Min(100, Max(0, 200 - dFilled * 2)) / 100, cG = 255 * Min(100, dFilled * 2) / 100;
	Message("@<c %x>%s:</c>|<c %x>%d%</c>", this, GetMaterialColorX(dMat, 1), MaterialName(dMat), RGBa(cR, cG, 0, 0), dFilled);
	//Grafik
	if (dMat == -1)
	{
		SetGraphics("", this, 0, 1);
		SetGraphics("", this, 0, 2);
	}
	else
	{
		var sFillState = "Empty";
		if (dFilled > 66)
			sFillState = "Full";
		else if (Inside(dFilled, 33, 66))
			sFillState = "Half";
		SetGraphics(sFillState, this, GetID(), 1, GFXOV_MODE_Base);
		SetGraphics(sFillState, this, GetID(), 2, GFXOV_MODE_Picture);
		SetClrModulation(RGB(GetMaterialColor(dMat, 0, 0), GetMaterialColor(dMat, 0, 1), GetMaterialColor(dMat, 0, 2)), this, 1);
		SetClrModulation(RGB(GetMaterialColor(dMat, 0, 0), GetMaterialColor(dMat, 0, 1), GetMaterialColor(dMat, 0, 2)), this, 2);
	}
	return 1;
}

private func FillLiquids()
{
	if (dFillLevel == 0)
		return 0;
	var idBarrel = GetBarrelType(dMat);
	if (!idBarrel)
		return 0;
	var pBarrel;
	//Fässer für Material
	while (pBarrel = FindObject(idBarrel, 0, 0, 0, 0, 0, 0, 0, this, pBarrel))
		if (!pBarrel->BarrelIsFull())
		{
			dFillLevel -= pBarrel->BarrelDoFill(dFillLevel, dMat);
			if (pBarrel->BarrelIsFull())
				Exit(pBarrel, 0, 25, 0, 0, 0, 0);
			else
				return 1;
		}
	if (dFillLevel == 0)
		return 1;
	//Leere Fässer
	var idEmptyBarrel = EmptyBarrelID();
	pBarrel = 0;
	if (idEmptyBarrel)
	{
		while (pBarrel = FindObject(idEmptyBarrel, 0, 0, 0, 0, 0, 0, 0, this, pBarrel))
			if (!pBarrel->BarrelIsFull())
			{
				ChangeDef(idBarrel, pBarrel);
				dFillLevel -= pBarrel->BarrelDoFill(dFillLevel, dMat);
				if (pBarrel->BarrelIsFull())
					Exit(pBarrel, 0, 25, 0, 0, 0, 0);
				else
					return 1;
			}
	}
	return 1;
}

public func AlternativeName()
{
	if ((dMat < 0) || (!bInit))
		return 0;
	return Format("%s-%s", MaterialName(dMat), GetName(this, 0, 1));
}

public func ContextSelectMaterial(object pClonk)
{
	[$TxtSelectMaterial$|Image=DTV1|Condition=IsEmpty]
	return MaterialMenu(pClonk);
}

public func IsEmpty()
{
	return dFillLevel == 0;
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
	return FindObject(DPIP, 0, 0, 0, 0, 0, "Connect", this);
}

public func ContextEjectContent(object pClonk)
{
	[$TxtEjectContent$|Image=DTFF|Condition=!IsEmpty]
	return EjectContentMenu(pClonk);
}

public func IsProducerOf(object pClonk, id idDef)
{
	if (!idDef)
		return 0;
	if (!this)
	{
		if (!idDef->~BarrelMaterialName())
			return 0;
		var dtMat;
		if (dtMat = idDef->~BarrelMaterial())
			return GetMaterialVal("Density", "Material", dtMat) == 25;
		return 0;
	}
	if (idDef == GetBarrelType(dMat))
		if (dFillLevel >= idDef->~BarrelMaxFill())
			return 1;
	return 0;
}

public func HowToProduce(object pClonk, id idDef)
{
	AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 10);
	if (idDef == GetBarrelType(dMat))
		return ProductionHelperBarrel(0, pClonk, GetDefinitionIndex(idDef) + 1);
	return 0;
}

public func ProductionHelperBarrel(Foo, object pClonk, int dIndexPlusEins)
{
	ScheduleCall(this, "DelayedProductionHelperBarrel", 1, 0, pClonk, dIndexPlusEins);
	return 1;
}

public func DelayedProductionHelperBarrel(object pClonk, int dIndexPlusEins)
{
	if (!dIndexPlusEins)
		return 0;
	var idDef = GetDefinition(dIndexPlusEins - 1);
	if (!GetName(0, idDef))
		return 0;
	if (!IsProducerOf(pClonk, idDef))
		return 0;
	var pObj;
	if (pObj = FindObject(EmptyBarrelID(), 0, 0, 0, 0, 0, 0, 0, pClonk))
	{
		AddCommand(pClonk, "Enter", this, 0, 0, 0, 0, 0);
		AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 50);
	}
	else
	{
		AddCommand(pClonk, "Call", this, pClonk, dIndexPlusEins, 0, 0, "ProductionHelperBarrel");
		AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 50);
		AddCommand(pClonk, "Acquire", 0, 0, 0, 0, 0, EmptyBarrelID());
		AddCommand(pClonk, "Wait", 0, 0, 0, 0, 0, 50);
	}
	return 1;
}

protected func MaterialMenu(object pClonk)
{
	CloseMenu(pClonk);
	CreateMenu(DTV1, pClonk, this, 0, "$TxtSelectMaterial$", 0, 1);
	AddMenuItem("$TxtNoMaterial$", "SelectMaterial", DPIP, pClonk, 0, -2, "$TxtNoMaterialDesc$");
	var i = -1;
	while (Material(MaterialName(++i)) != -1)
		if (IsContainerforMat(i))
			AddMenuItem(Format("$TxtMaterial$", MaterialName(i)), "SelectMaterial", DPIP, pClonk, 0, i, Format("$TxtMaterialDesc$", MaterialName(i)));
	return 1;
}

protected func SelectMaterial(Foo, int dnMat)
{
	if (!IsEmpty())
		return 0;
	dMat = dnMat;
	ShowContent();
	return 1;
}

protected func DirtyPipeMenu(object pClonk)
{
	CloseMenu(pClonk);
	CreateMenu(DPIP, pClonk, this, 0, "$TxtSelectDirtyPipe$", 0, 1, 1, DPIP);
	AddMenuItem("$TxtNoDirtyPipe$", "SelectPipe", DPIP, pClonk, 0, pClonk, "$TxtNoDirtyPipeDesc$", 128, 0, 0);
	var pLine = 0, pTarget;
	while (pLine = FindObject(DPIP, 0, 0, 0, 0, 0, "Connect", this, NoContainer(), pLine))
	{
		if (!pLine)
			continue;
		pTarget = GetActionTarget(1, pLine);
		if ((!pTarget) || (pTarget == this))
			continue;
		AddMenuItem(Format("$TxtDirtyPipe$", GetName(pTarget)), "SelectPipe", GetID(pTarget), pClonk, 0, pClonk, Format("$TxtDirtyPipeDesc$", GetName(pTarget)), 128, 0, ObjectNumber(pLine));
	}
	return 1;
}

protected func LimitPipeMenu(object pClonk)
{
	CloseMenu(pClonk);
	CreateMenu(DPIP, pClonk, this, 0, "$TxtSelectLimitPipe$", 0, 1, 1, SPIP);
	AddMenuItem("$TxtNoLimitPipe$", "SelectPipe", SPIP, pClonk, 0, pClonk, "$TxtNoLimitPipeDesc$", 128, 0, 0);
	var pLine = 0, pTarget;
	while (pLine = FindObject(DPIP, 0, 0, 0, 0, 0, "Connect", this, NoContainer(), pLine))
	{
		if (!pLine)
			continue;
		pTarget = GetActionTarget(1, pLine);
		if ((!pTarget) || (pTarget == this))
			continue;
		AddMenuItem(Format("$TxtLimitPipe$", GetName(pTarget)), "SelectPipe", GetID(pTarget), pClonk, 0, pClonk, Format("$TxtLimitPipeDesc$", GetName(pTarget)), 132, this, ObjectNumber(pLine));
	}
	return 1;
}

protected func EjectContentMenu(object pClonk)
{
	SetAction("Idle", 0, 0);
	CloseMenu(pClonk);
	CreateMenu(DPIP, pClonk, this, 0, "$TxtEjectContent$", 0, 1, 1, DTFF);
	AddMenuItem("$TxtNoEjection$", "SelectPipe", DTFF, pClonk, 0, pClonk, "$TxtNoEjectionDesc$", 128, 0, -1);
	AddMenuItem("$TxtNoEjectPipe$", "SelectPipe", DTFF, pClonk, 0, pClonk, "$TxtNoEjectPipeDesc$", 132, this, ObjectNumber(this));
	var pLine = 0, pTarget;
	while (pLine = FindObject(DPIP, 0, 0, 0, 0, 0, "Connect", this, NoContainer(), pLine))
	{
		if (!pLine)
			continue;
		pTarget = GetActionTarget(1, pLine);
		if ((!pTarget) || (pTarget == this))
			continue;
		AddMenuItem(Format("$TxtEjectPipe$", GetName(pTarget)), "SelectPipe", GetID(pTarget), pClonk, 0, pClonk, Format("$TxtEjectPipeDesc$", GetName(pTarget)), 132, this, ObjectNumber(pLine));
	}
	return 1;
}

protected func SelectPipe(Foo, object pClonk, bool bSpezial, int dObject)
{
	if (!pClonk)
		return 0;
	var idDef = GetMenu(pClonk);
	if (!idDef)
		return 0;
	if ((dObject <= 0) || (!idDef))
	{
		if (idDef == SPIP)
			pLimitPipe = 0;
		if (idDef == DPIP)
			pDirtyPipe = 0;
		CloseMenu(pClonk);
		return 1;
	}
	var pLine = Object(dObject);
	if (!pLine)
		return 0;
	if (!bSpezial)
	{
		if (idDef == SPIP)
			pLimitPipe = pLine;
		if (idDef == DPIP)
			pDirtyPipe = pLine;
		if (idDef == DTFF)
			SetAction("Eject", pLine, pClonk);
		CloseMenu(pClonk);
	}
	else
	{
		SetPlrView(GetOwner(pClonk), GetActionTarget(1, pLine));
		CreateParticle("MSpark", GetX(GetActionTarget(1, pLine)), GetY(GetActionTarget(1, pLine)), 0, 15, 50, RGB(255, 255, 255));
	}
	return 1;
}

protected func EjectContent(Foo, bool bVerify)
{
	var pTarget = GetActionTarget(0), pVerify = GetActionTarget(1);
	SetActionTargets(pTarget, 0);
	if ((pVerify) && (!bVerify))
	{
		CloseMenu(pVerify);
		CreateMenu(DTFF, pVerify, this, 0, "$TxtEjectPipe$", 0, 1, 0);
		AddMenuItem("$TxtNoEjection$", "SelectPipe", DTFF, pVerify, 0, pVerify, "$TxtNoEjectionDesc$");
		AddMenuItem("$TxtEjectContent$", "EjectContent", SPIP, pVerify, 0, 1, "$TxtEjectContentDesc$");
		return 1;
	}
	if (dFillLevel == 0)
	{
		SetAction("Idle", 0, 0);
		return 1;
	}
	if (pTarget == this)
	{
		var i = 0;
		while ((i++ < 10) && (LiquidOutput(-2, 1) >= 0))
			InsertMaterial(dMat);
	}
	else
	{
		var i = 0;
		while ((i++ < 10) && (LiquidOutput(-2, 1) >= 0))
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
	if (Material(MaterialName(dnMat)) == -1)
		return 0;
	if (GetMaterialVal("Density", "Material", dnMat) != 25)
		return 0;
	return 1;
}

public func IsLiquidContainer()
{
	return 1;
}

public func CheckOwnCapacity(int dnMat, int dnAmount, int dAntiStuck)
{
	if ((dnMat < 0) || (dnAmount == 0))
		return 0;
	if (GetAction() == "Eject")
		return 0;
	if (dnMat != dMat)
	{
		if (IsContainerforMat(dMat))
			return PipeCheck(pDirtyPipe, dnMat, dnAmount, dAntiStuck);
		return 0;
	}
	if (dFillLevel + dnAmount > dMaxFillLevel + 5)
		return PipeCheck(pLimitPipe, dnMat, dnAmount, dAntiStuck);
	return 1;
}

private func PipeCheck(object pPipe, int dnMat, int dnAmount, int dAntiStuck)
{
	if ((!pPipe) || (dAntiStuck > 100))
		return 0;
	if (this != GetActionTarget(0, pPipe))
		return 0;
	var pPumpTarget = GetActionTarget(1, pPipe);
	if ((!pPumpTarget) || (GetID(pPumpTarget) == LNKT))
		return 1;
	if (pPumpTarget->~IsLiquidContainer())
		return pPumpTarget->~CheckOwnCapacity(dnMat, dnAmount, dAntiStuck++);
	if (GetBarrelType(dnMat))
	{
		for (var pContent in FindObjects(Find_Container(pPumpTarget), Find_ID(GetBarrelType(dnMat)))) 
		{
			if (pContent->~BarrelIsFull())
				continue;
			return 1;
		}
		if (EmptyBarrelID())
			if (ObjectCount(EmptyBarrelID(), 0, 0, 0, 0, 0, 0, 0, pPumpTarget))
				return 1;
	}
	return 0;
}

public func LiquidOutput(int dnMat, int dnAmount, object pnLine)
{
	if (dnAmount == 0)
		return -2;
	if (dMat == -1)
		return -2;
	if ((dnMat != dMat) && (dnMat != -2))
		return -2;
	FillLiquids();
	
	if (dFillLevel - dnAmount < 0)
		return -2;
	if (pnLine)
	{
		if (GetID(pnLine) == DPIP)
		{
			if ((pnLine == pLimitPipe) || (pnLine == pDirtyPipe))
				return -2;
			if (GetActionTarget(0, pnLine) != this)
				return -2;
			if (!PUMP->~CheckCapacity(GetActionTarget(1, pnLine), dnAmount, dMat))
				return -2;
		}
		else
		{
			if ((pnLine == GetActionTarget(1, pLimitPipe)) || (pnLine == GetActionTarget(1, pDirtyPipe)))
				return -2;
			if ((GetID(pnLine) == DTFF) && (dFillLevel < dMaxFillLevel))
				return -2;
			if (pnLine->~IsLiquidConatiner())
				if (!pnLine->~CheckOwnCapacity(dMat, dnAmount))
					return -2;
		}
	}
	dFillLevel -= dnAmount;
	ShowContent();
	return dMat;
}

public func LiquidInput(int dnMat, int dnAmount, object pnLine)
{
	if ((dnMat < 0) || (dnAmount == 0))
		return 0;
	if (dnMat != dMat)
	{
		if (IsContainerforMat(dMat))
			return PipeTransfer(pDirtyPipe, dnMat, dnAmount);
		return 0;
	}
	if (dFillLevel + dnAmount > dMaxFillLevel + 5)
		return PipeTransfer(pLimitPipe, dnMat, dnAmount);
	dFillLevel += dnAmount;
	ShowContent();
	return 1;
}

protected func PipeTransfer(object pPipe, int dnMat, int dnAmount)
{
	if (!pPipe)
		return 0;
	// Angeschlossene Objekte ermitteln
	var pPumpSource = GetActionTarget(0, pPipe);
	// Kein Quellobjekt
	if ((!pPumpSource) || (pPumpSource != this))
		return 0;
	// Quellobjekt hat nicht genug Energie
	if (!EnergyCheck(1000, pPumpSource))
		return 0;
	// Zielobjekt vorhanden
	var i;
	var pPumpTarget = GetActionTarget(1, pPipe);
	if (pPumpTarget)
	{
		for (i = 0; i < dnAmount; i++)
			ObjectInsertMaterial(dnMat, pPumpTarget);
	}
	else
	{
		for (i = 0; i < dnAmount; i++)
			InsertMaterial(dnMat, GetVertex(50, 0), GetVertex(50, 1));
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
		Exit(pContent, 0, 25, 0, 0, 0, 0);
		Enter(this, pContent);
	}
	for (var pContent in FindObjects(Find_Container(pObj), Find_ID(GetBarrelType(dMat)))) 
	{
		if (pContent->~BarrelIsFull())
			continue;
		Exit(pContent, 0, 25, 0, 0, 0, 0);
		Enter(this, pContent);
	}
	if ((pObj->~BarrelMaxFill() <= 0) || (pObj->~BarrelIsFull()))
	{
		Exit(pObj, 0, 25, 0, 0, 0, 0);
		return 1;
	}
	for (var pContent in FindObjects(Find_Container(pObj))) 
	{
		Exit(pContent, 0, 25, 0, 0, 0, 0);
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