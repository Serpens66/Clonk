/* --- Bergwerkssteuerung --- */

#strict 3

local dController;
local bPublic;
local bLocked;
local idaRess;
local daRessValue;
local dRessValue;
local daLiquids;
local daLiquidsValue;
local dLiquidsValue;

//Per appento überladen, wenn Modifikation verhindert werden soll.
public func AutoLock()
{
	return nil;
}

protected func Initialize()
{
	SetPosition(-(GetX() ?? 0), -(GetY() ?? 0));
	MineRessSearch();
	PumpLiquidSearch();
	bLocked = AutoLock();
	if (!bLocked)
	{
		dController = GetPlayerByIndex(nil, C4PT_User);
		PlrMessage("$TxtRequestSetup", dController);
		ScheduleCall(this, "Publish", 37 * 30);
	}
	return 1;
}

protected func Publish()
{
	return bPublic = 1;
}

public func IsController(int dPlayer)
{
	dPlayer ??= 0;
	return (!bLocked) && ((bPublic) || (S42MigrationEqual(dPlayer, dController)));
}

protected func Activate(int iByPlayer)
{
	iByPlayer ??= 0;
	if (IsController(iByPlayer))
		return Menu(iByPlayer);
	InfoMenu(iByPlayer);
	return 1;
}

protected func Menu(int dPlayer)
{
	dPlayer ??= 0;
	if (!IsController(dPlayer))
		return Sound("Error");
	ClearScheduleCall(this, "Publish");
	if (bPublic)
		ScheduleCall(this, "Publish", 37 * 30);
	var pCursor = GetCursor(dPlayer);
	CreateMenu(GetID(), pCursor, this, nil, "$TxtMenu$", nil, 1);
	AddMenuItem("$TxtInfoMenu$", Format("InfoMenu(%d)", dPlayer), DTZ1, pCursor, nil, nil, "$TxtInfoMenuDesc$", 2, 25);
	AddMenuItem("$TxtRessConfigMenu$", Format("ConfigMenu(Object(%d),%d)", ObjectNumber(pCursor), 1), GOLD, pCursor, nil, nil, "$TxtRessConfigMenuDesc$");
	AddMenuItem("$TxtLiquidConfigMenu$", Format("ConfigMenu(Object(%d),%d)", ObjectNumber(pCursor), 2), WBRL, pCursor, nil, nil, "$TxtLiquidConfigMenuDesc$");
	AddMenuItem("$TxtLockMenu$", Format("LockMenu(%d)", dPlayer), DTZ1, pCursor, nil, nil, "$TxtLockMenuDesc$");
	return 1;
}

protected func ConfigMenu(object pClonk, int dType)
{
	dType ??= 0;
	if (!pClonk)
		return nil;
	if (!IsController(GetOwner(pClonk)))
		return Sound("Error");
	ClearScheduleCall(this, "Publish");
	if (bPublic)
		ScheduleCall(this, "Publish", 37 * 30);
	CreateMenu(GetID(), pClonk, this, 2, "$TxtConfigMenu$", nil, 1);
	if (S42MigrationEqual(dType, 1))
		for (var idDef in idaRess) 
			AddMenuItem(GetName(nil, idDef), Format("Config(Object(%d),%d,%d)", ObjectNumber(pClonk), dType, (GetDefinitionIndex(idDef) ?? 0) + 1), idDef, pClonk, GetMineValue(idDef), nil, Format("$TxtConfigDesc$", GetName(nil, idDef)));
	if (S42MigrationEqual(dType, 2))
		for (var dtMat in daLiquids) 
		{
			var idBarrel = GetBarrelType(dtMat);
			if (!idBarrel)
				idBarrel = GOLD;
			AddMenuItem(MaterialName(dtMat), Format("Config(Object(%d),%d,%d)", ObjectNumber(pClonk), dType, (dtMat ?? 0) + 1), idBarrel, pClonk, GetPumpValue(dtMat), nil, Format("$TxtConfigDesc$", MaterialName(dtMat)), 128, nil, GetValue(nil, idBarrel));
		}
	AddMenuItem("$TxtConfigFunctions$", Format("ConfigFunctions(Object(%d),%d)", ObjectNumber(pClonk), dType), CLNK, pClonk, nil, nil, "$TxtConfigFunctionsDesc$", 128);
	return 1;
}

protected func Config(object pClonk, int dType, int dxChange, int dValuePlusEins)
{
	dType ??= 0;
	dxChange ??= 0;
	dValuePlusEins ??= 0;
	if (!pClonk)
		return nil;
	if (dValuePlusEins)
	{
		if (S42MigrationEqual(dType, 1))
		{
			var dtIndex = ArrayDataIndex(idaRess, GetDefinition((dxChange ?? 0) - 1));
			if (S42MigrationEqual(dtIndex, -1))
				return nil;
			var dTemp = daRessValue[(dtIndex ?? 0)];
			daRessValue[(dtIndex ?? 0)] = (dValuePlusEins ?? 0) - 1;
			S42MigrationAssign(dRessValue, "-=", (dTemp ?? 0) - ((dValuePlusEins ?? 0) - 1));
		}
		if (S42MigrationEqual(dType, 2))
		{
			var dtIndex = ArrayDataIndex(daLiquids, (dxChange ?? 0) - 1);
			if (S42MigrationEqual(dtIndex, -1))
				return nil;
			var dTemp = daLiquidsValue[(dtIndex ?? 0)];
			daLiquidsValue[(dtIndex ?? 0)] = (dValuePlusEins ?? 0) - 1;
			S42MigrationAssign(dLiquidsValue, "-=", (dTemp ?? 0) - ((dValuePlusEins ?? 0) - 1));
		}
	}
	if (!pClonk)
		return 1;
	CloseMenu(pClonk);
	var dtValue;
	if (S42MigrationEqual(dType, 1))
	{
		CreateMenu(GOLD, pClonk, this, nil, GetName(nil, GetDefinition((dxChange ?? 0) - 1)), nil, 1);
		dtValue = ArrayData(daRessValue, ArrayDataIndex(idaRess, GetDefinition((dxChange ?? 0) - 1)));
	}
	if (S42MigrationEqual(dType, 2))
	{
		CreateMenu(BARL, pClonk, this, nil, MaterialName((dxChange ?? 0) - 1), nil, 1);
		dtValue = ArrayData(daLiquidsValue, ArrayDataIndex(daLiquids, (dxChange ?? 0) - 1));
	}
	dtValue = BoundBy(dtValue, nil, 10000);
	var datValues = [nil, 1, 5, 10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000, (dtValue ?? 0) - 1000, (dtValue ?? 0) - 500, (dtValue ?? 0) - 250, (dtValue ?? 0) - 100, (dtValue ?? 0) - 50, (dtValue ?? 0) - 25, (dtValue ?? 0) - 10, (dtValue ?? 0) - 5, (dtValue ?? 0) - 1, dtValue, (dtValue ?? 0) + 1, (dtValue ?? 0) + 2, (dtValue ?? 0) + 5, (dtValue ?? 0) + 10, (dtValue ?? 0) + 25, (dtValue ?? 0) + 50, (dtValue ?? 0) + 100, (dtValue ?? 0) + 250, (dtValue ?? 0) + 500, (dtValue ?? 0) + 1000];
	ArrayBoundBy(datValues, nil, 10000, 1);
	ArraySort(datValues, "NumericOrder");
	for (var dnValue in datValues) 
		AddMenuItem(Format("%d", dnValue), Format("Config(Object(%d),%d,%d,%d)", ObjectNumber(pClonk), dType, dxChange, (dnValue ?? 0) + 1), FLAG, pClonk, S42MigrationEqual(dnValue, dtValue), nil, "$TxtConfigSetDesc$");
	AddMenuItem("$TxtBack$", Format("ConfigMenu(Object(%d),%d)", ObjectNumber(pClonk), dType), DTZ1, pClonk, nil, nil, "$TxtBackDesc$");
	SelectMenuItem(ArrayDataIndex(datValues, dtValue), pClonk);
	return 1;
}

protected func ConfigFunctions(object pClonk, int dType)
{
	dType ??= 0;
	if (!pClonk)
		return nil;
	CloseMenu(pClonk);
	CreateMenu(CLNK, pClonk, this, nil, "$TxtSelectConfigFunctions$", nil, 1);
	AddMenuItem("$TxtFunctionEqual$", Format("ConfigFunction_Equal(Object(%d),%d)", ObjectNumber(pClonk), dType), ERTH, pClonk, nil, nil, "$TxtFunctionEqualDesc$");
	AddMenuItem("$TxtFunctionValue$", Format("ConfigFunction_Value(Object(%d),%d)", ObjectNumber(pClonk), dType), ROCK, pClonk, nil, nil, "$TxtFunctionValueDesc$");
	AddMenuItem("$TxtFunctionValue2$", Format("ConfigFunction_Value(Object(%d),%d,1)", ObjectNumber(pClonk), dType), GOLD, pClonk, nil, nil, "$TxtFunctionValue2Desc$");
	AddMenuItem("$TxtFunctionAmount$", Format("ConfigFunction_Amount(Object(%d),%d)", ObjectNumber(pClonk), dType), WBRL, pClonk, nil, nil, "$TxtFunctionAmountDesc$");
	AddMenuItem("$TxtFunctionRatio$", Format("ConfigFunction_Ratio(Object(%d),%d)", ObjectNumber(pClonk), dType), ICE1, pClonk, nil, nil, "$TxtFunctionRatioDesc$");
	AddMenuItem("$TxtFunctionRandom10$", Format("ConfigFunction_Random(Object(%d),%d,10)", ObjectNumber(pClonk), dType), ICE1, pClonk, nil, nil, "$TxtFunctionRandom10Desc$");
	AddMenuItem("$TxtFunctionRandom100$", Format("ConfigFunction_Random(Object(%d),%d,100)", ObjectNumber(pClonk), dType), ICE1, pClonk, nil, nil, "$TxtFunctionRandom100Desc$");
	AddMenuItem("$TxtFunctionRandom1000$", Format("ConfigFunction_Random(Object(%d),%d,1000)", ObjectNumber(pClonk), dType), ICE1, pClonk, nil, nil, "$TxtFunctionRandom1000Desc$");
	AddMenuItem("$TxtFunctionRandom10000$", Format("ConfigFunction_Random(Object(%d),%d,10000)", ObjectNumber(pClonk), dType), ICE1, pClonk, nil, nil, "$TxtFunctionRandom10000Desc$");
	return 1;
}

protected func ConfigFunction_Equal(object pClonk, int dType)
{
	dType ??= 0;
	if (S42MigrationEqual(dType, 1))
		for (var i = nil; (i ?? 0) < (GetLength(daRessValue) ?? 0); S42MigrationStep(i, 1, true))
			daRessValue[(i ?? 0)] = 1;
	if (S42MigrationEqual(dType, 2))
		for (var i = nil; (i ?? 0) < (GetLength(daLiquidsValue) ?? 0); S42MigrationStep(i, 1, true))
			daLiquidsValue[(i ?? 0)] = 1;
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Value(object pClonk, int dType, bool bSquare)
{
	dType ??= 0;
	bSquare ??= false;
	var dBasic = 100 + 900 * (bSquare ?? 0);
	if (S42MigrationEqual(dType, 1))
		for (var i = nil; (i ?? 0) < (GetLength(daRessValue) ?? 0); S42MigrationStep(i, 1, true))
		{
			var idDef = idaRess[(i ?? 0)];
			var dTemp = Max(1, (dBasic ?? 0) / (Max(1, GetValue(nil, idDef)) ?? 0));
			if (bSquare)
				dTemp = Max(1, (dTemp ?? 0) / (Max(1, GetValue(nil, idDef)) ?? 0));
			daRessValue[(i ?? 0)] = dTemp;
		}
	if (S42MigrationEqual(dType, 2))
		for (var i = nil; (i ?? 0) < (GetLength(daLiquidsValue) ?? 0); S42MigrationStep(i, 1, true))
		{
			var idDef = GetBarrelType(daLiquids[(i ?? 0)]);
			var dTemp = Max(1, (dBasic ?? 0) / (Max(1, GetValue(nil, idDef)) ?? 0));
			if (bSquare)
				dTemp = Max(1, (dTemp ?? 0) / (Max(1, GetValue(nil, idDef)) ?? 0));
			daLiquidsValue[(i ?? 0)] = (dTemp ?? 0) * ((!!idDef) ?? 0);
		}
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Amount(object pClonk, int dType)
{
	dType ??= 0;
	if (S42MigrationEqual(dType, 1))
		for (var i = nil; (i ?? 0) < (GetLength(daRessValue) ?? 0); S42MigrationStep(i, 1, true))
		{
			var j = -1, dTemp = nil;
			while (!S42MigrationEqual(Material(MaterialName(S42MigrationStep(j, 1))), -1))
			{
				if (S42MigrationEqual(GetMaterialVal("Dig2Object", "Material", j), idaRess[(i ?? 0)]))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0));
				else if (S42MigrationEqual(GetMaterialVal("Blast2Object", "Material", j), idaRess[(i ?? 0)]))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0));
			}
			daRessValue[(i ?? 0)] = (dTemp ?? 0) / 100;
		}
	if (S42MigrationEqual(dType, 2))
		for (var i = nil; (i ?? 0) < (GetLength(daLiquidsValue) ?? 0); S42MigrationStep(i, 1, true))
			daLiquidsValue[(i ?? 0)] = (GetMaterialCount(daLiquids[(i ?? 0)]) ?? 0) / 100;
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Ratio(object pClonk, int dType)
{
	dType ??= 0;
	if (S42MigrationEqual(dType, 1))
		for (var i = nil; (i ?? 0) < (GetLength(daRessValue) ?? 0); S42MigrationStep(i, 1, true))
		{
			var j = -1, dTemp = nil;
			while (!S42MigrationEqual(Material(MaterialName(S42MigrationStep(j, 1))), -1))
			{
				if (S42MigrationEqual(GetMaterialVal("Dig2Object", "Material", j), idaRess[(i ?? 0)]))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0) / (Max(1, GetMaterialVal("Dig2ObjectRatio", "Material", j)) ?? 0));
				else if (S42MigrationEqual(GetMaterialVal("Blast2Object", "Material", j), idaRess[(i ?? 0)]))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0) / (Max(1, GetMaterialVal("Blast2ObjectRatio", "Material", j)) ?? 0));
			}
			daRessValue[(i ?? 0)] = dTemp;
		}
	if (S42MigrationEqual(dType, 2))
		for (var i = nil; (i ?? 0) < (GetLength(daLiquidsValue) ?? 0); S42MigrationStep(i, 1, true))
		{
			var j = -1, dTemp = nil;
			while (!S42MigrationEqual(Material(MaterialName(S42MigrationStep(j, 1))), -1))
			{
				if (S42MigrationEqual(MaterialName(daLiquids[(i ?? 0)]), MaterialName(j)))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0));
				else if (S42MigrationEqual(GetMaterialVal("AboveTempConvertTo", "Material", j), MaterialName(daLiquids[(i ?? 0)])))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0));
				else if (S42MigrationEqual(GetMaterialVal("BelowTempConvertTo", "Material", j), MaterialName(daLiquids[(i ?? 0)])))
					S42MigrationAssign(dTemp, "+=", (GetMaterialCount(j) ?? 0));
			}
			daLiquidsValue[(i ?? 0)] = (dTemp ?? 0) / 100;
		}
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Random(object pClonk, int dType, int dSize)
{
	dType ??= 0;
	dSize ??= 0;
	if (S42MigrationEqual(dType, 1))
		for (var i = nil; (i ?? 0) < (GetLength(daRessValue) ?? 0); S42MigrationStep(i, 1, true))
			daRessValue[(i ?? 0)] = Random((dSize ?? 0) + 1);
	if (S42MigrationEqual(dType, 2))
		for (var i = nil; (i ?? 0) < (GetLength(daLiquidsValue) ?? 0); S42MigrationStep(i, 1, true))
			daLiquidsValue[(i ?? 0)] = Random((dSize ?? 0) + 1);
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}
	
protected func InfoMenu(int dPlayer)
{
	dPlayer ??= 0;
	var stInfo = Format("$TxtInfoHead$");
	for (var idtRess in idaRess) 
		stInfo = Format("%s|%d=%s (%d)", stInfo, GetMineValue(idtRess), GetName(nil, idtRess), GetValue(nil, idtRess));
	stInfo = Format("%s|%s", stInfo, "--------------------");
	for (var dtLiquid in daLiquids) 
		if (GetBarrelType(dtLiquid))
			stInfo = Format("%s|%d=%s (%d)", stInfo, GetPumpValue(dtLiquid), MaterialName(dtLiquid), GetValue(nil, GetBarrelType(dtLiquid)));
		else
			stInfo = Format("%s|%d=%s", stInfo, GetPumpValue(dtLiquid), MaterialName(dtLiquid));
	MessageWindow(Format("$TxtInfoMessage$", GetDesc(), stInfo), dPlayer, GOLD, GetName());
	return 1;
}
	
protected func LockMenu(int dPlayer, bool bLock)
{
	dPlayer ??= 0;
	bLock ??= false;
	if (!IsController(dPlayer))
		return Sound("Error");
	ClearScheduleCall(this, "Publish");
	if (bLock)
	{
		bLocked = 1;
		CloseMenu(GetCursor(dPlayer));
		Sound("Connect");
		return 1;
	}
	ScheduleCall(this, "Publish", 37 * 30);
	dController = dPlayer;
	var pCursor = GetCursor(dPlayer);
	CreateMenu(GetID(), pCursor, this, nil, "$TxtMenu$", nil, 1);
	AddMenuItem("$TxtLock$", Format("LockMenu(%d,1)", dPlayer), DTZ1, pCursor, nil, nil, "$TxtLockDesc$");
	AddMenuItem("$TxtBack$", Format("Menu(%d)", dPlayer), DTZ1, pCursor, nil, nil, "$TxtBackDesc$", 2, 8);
	SelectMenuItem(1, pCursor);
	return 1;
}
	
protected func MineRessSearch()
{
	idaRess = [];
	daRessValue = [];
	dRessValue = nil;
	var i = -1, idDef;
	while (idDef = GetDefinition(S42MigrationStep(i, 1), C4D_Object))
		if (IsMineProduct(idDef))
			idaRess[(GetLength(idaRess) ?? 0)] = idDef;
	for (var i = nil; (i ?? 0) < (GetLength(idaRess) ?? 0); S42MigrationStep(i, 1, true))
	{
		daRessValue[(i ?? 0)] = GetMineValue(idaRess[(i ?? 0)], 1);
		S42MigrationAssign(dRessValue, "+=", (daRessValue[(i ?? 0)] ?? 0));
	}
	bLocked = nil;
	ScheduleCall(this, "Publish", 37 * 30);
	return 1;
}
	
public func IsMineProduct(id idDef)
{
	if (!idDef)
		return nil;
	return idDef->~IsMiningProduct();
}

public func GetMineValue(id idDef, bool bDefault)
{
	bDefault ??= false;
	if (!idDef)
		return nil;
	if (!bDefault)
		return ArrayData(daRessValue, ArrayDataIndex(idaRess, idDef));
	var dTemp = idDef->~GetMiningRate();
	if (!dTemp)
		dTemp = Max(1, 1000 / (Max(1, GetValue(nil, idDef)) ?? 0) / (Max(1, GetValue(nil, idDef)) ?? 0));
	return dTemp;
}

public func GetMineProductList()
{
	return idaRess;
}

public func GetMineTotalValue()
{
	return dRessValue;
}

protected func PumpLiquidSearch()
{
	daLiquids = [];
	daLiquidsValue = [];
	dLiquidsValue = nil;
	var i = -1;
	while (!S42MigrationEqual(Material(MaterialName(S42MigrationStep(i, 1))), -1))
		if (IsPumpProduct(i))
			ArrayAdd(daLiquids, i);
	for (var i = nil; (i ?? 0) < (GetLength(daLiquids) ?? 0); S42MigrationStep(i, 1, true))
	{
		daLiquidsValue[(i ?? 0)] = GetPumpValue(daLiquids[(i ?? 0)], 1);
		S42MigrationAssign(dLiquidsValue, "+=", (daLiquidsValue[(i ?? 0)] ?? 0));
	}
	bLocked = nil;
	ScheduleCall(this, "Publish", 37 * 30);
	return 1;
}
	
public func IsPumpProduct(int dnMat)
{
	dnMat ??= 0;
	if (S42MigrationEqual(Material(MaterialName(dnMat)), -1))
		return nil;
	if (!S42MigrationEqual(GetMaterialVal("Density", "Material", dnMat), 25))
		return nil;
	return 1;
}

public func GetPumpValue(int dnMat, bool bDefault)
{
	dnMat ??= 0;
	bDefault ??= false;
	if (!IsPumpProduct(dnMat))
		return nil;
	if (!bDefault)
		return ArrayData(daLiquidsValue, ArrayDataIndex(daLiquids, dnMat));
	;
	var dTemp = _inherited(dnMat);
	if (dTemp)
		return dTemp;
	var idBarrel = GetBarrelType(dnMat);
	if (!idBarrel)
		return nil;
	dTemp = GetValue(nil, idBarrel);
	//dTemp=GetValue(0,idBarrel)-GetValue(0,idBarrel->~EmptyBarrelID());
	return Max(1, 100 / (dTemp ?? 0));
}

public func GetPumpProductList()
{
	return daLiquids;
}

public func GetPumpTotalValue()
{
	return dLiquidsValue;
}

public func LineConnectType()
{
	return DPIP;
}