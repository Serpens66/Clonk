/* --- Bergwerkssteuerung --- */

#strict 2

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
	return 0;
}

protected func Initialize()
{
	SetPosition(-GetX(), -GetY());
	MineRessSearch();
	PumpLiquidSearch();
	bLocked = AutoLock();
	if (!bLocked)
	{
		dController = GetPlayerByIndex(0, C4PT_User);
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
	return (!bLocked) && ((bPublic) || (dPlayer == dController));
}

protected func Activate(int iByPlayer)
{
	if (IsController(iByPlayer))
		return Menu(iByPlayer);
	InfoMenu(iByPlayer);
	return 1;
}

protected func Menu(int dPlayer)
{
	if (!IsController(dPlayer))
		return Sound("Error");
	ClearScheduleCall(this, "Publish");
	if (bPublic)
		ScheduleCall(this, "Publish", 37 * 30);
	var pCursor = GetCursor(dPlayer);
	CreateMenu(GetID(), pCursor, this, 0, "$TxtMenu$", 0, 1);
	AddMenuItem("$TxtInfoMenu$", Format("InfoMenu(%d)", dPlayer), DTZ1, pCursor, 0, 0, "$TxtInfoMenuDesc$", 2, 25);
	AddMenuItem("$TxtRessConfigMenu$", Format("ConfigMenu(Object(%d),%d)", ObjectNumber(pCursor), 1), GOLD, pCursor, 0, 0, "$TxtRessConfigMenuDesc$");
	AddMenuItem("$TxtLiquidConfigMenu$", Format("ConfigMenu(Object(%d),%d)", ObjectNumber(pCursor), 2), WBRL, pCursor, 0, 0, "$TxtLiquidConfigMenuDesc$");
	AddMenuItem("$TxtLockMenu$", Format("LockMenu(%d)", dPlayer), DTZ1, pCursor, 0, 0, "$TxtLockMenuDesc$");
	return 1;
}

protected func ConfigMenu(object pClonk, int dType)
{
	if (!pClonk)
		return 0;
	if (!IsController(GetOwner(pClonk)))
		return Sound("Error");
	ClearScheduleCall(this, "Publish");
	if (bPublic)
		ScheduleCall(this, "Publish", 37 * 30);
	CreateMenu(GetID(), pClonk, this, 2, "$TxtConfigMenu$", 0, 1);
	if (dType == 1)
		for (var idDef in idaRess) 
			AddMenuItem(GetName(0, idDef), Format("Config(Object(%d),%d,%d)", ObjectNumber(pClonk), dType, GetDefinitionIndex(idDef) + 1), idDef, pClonk, GetMineValue(idDef), 0, Format("$TxtConfigDesc$", GetName(0, idDef)));
	if (dType == 2)
		for (var dtMat in daLiquids) 
		{
			var idBarrel = GetBarrelType(dtMat);
			if (!idBarrel)
				idBarrel = GOLD;
			AddMenuItem(MaterialName(dtMat), Format("Config(Object(%d),%d,%d)", ObjectNumber(pClonk), dType, dtMat + 1), idBarrel, pClonk, GetPumpValue(dtMat), 0, Format("$TxtConfigDesc$", MaterialName(dtMat)), 128, 0, GetValue(0, idBarrel));
		}
	AddMenuItem("$TxtConfigFunctions$", Format("ConfigFunctions(Object(%d),%d)", ObjectNumber(pClonk), dType), CLNK, pClonk, 0, 0, "$TxtConfigFunctionsDesc$", 128);
	return 1;
}

protected func Config(object pClonk, int dType, int dxChange, int dValuePlusEins)
{
	if (!pClonk)
		return 0;
	if (dValuePlusEins)
	{
		if (dType == 1)
		{
			var dtIndex = ArrayDataIndex(idaRess, GetDefinition(dxChange - 1));
			if (dtIndex == -1)
				return 0;
			var dTemp = daRessValue[dtIndex];
			daRessValue[dtIndex] = dValuePlusEins - 1;
			dRessValue -= dTemp - (dValuePlusEins - 1);
		}
		if (dType == 2)
		{
			var dtIndex = ArrayDataIndex(daLiquids, dxChange - 1);
			if (dtIndex == -1)
				return 0;
			var dTemp = daLiquidsValue[dtIndex];
			daLiquidsValue[dtIndex] = dValuePlusEins - 1;
			dLiquidsValue -= dTemp - (dValuePlusEins - 1);
		}
	}
	if (!pClonk)
		return 1;
	CloseMenu(pClonk);
	var dtValue;
	if (dType == 1)
	{
		CreateMenu(GOLD, pClonk, this, 0, GetName(0, GetDefinition(dxChange - 1)), 0, 1);
		dtValue = ArrayData(daRessValue, ArrayDataIndex(idaRess, GetDefinition(dxChange - 1)));
	}
	if (dType == 2)
	{
		CreateMenu(BARL, pClonk, this, 0, MaterialName(dxChange - 1), 0, 1);
		dtValue = ArrayData(daLiquidsValue, ArrayDataIndex(daLiquids, dxChange - 1));
	}
	dtValue = BoundBy(dtValue, 0, 10000);
	var datValues = [0, 1, 5, 10, 25, 50, 75, 100, 250, 500, 750, 1000, 2500, 5000, 7500, 10000, dtValue - 1000, dtValue - 500, dtValue - 250, dtValue - 100, dtValue - 50, dtValue - 25, dtValue - 10, dtValue - 5, dtValue - 1, dtValue, dtValue + 1, dtValue + 2, dtValue + 5, dtValue + 10, dtValue + 25, dtValue + 50, dtValue + 100, dtValue + 250, dtValue + 500, dtValue + 1000];
	ArrayBoundBy(datValues, 0, 10000, 1);
	ArraySort(datValues, "NumericOrder");
	for (var dnValue in datValues) 
		AddMenuItem(Format("%d", dnValue), Format("Config(Object(%d),%d,%d,%d)", ObjectNumber(pClonk), dType, dxChange, dnValue + 1), FLAG, pClonk, dnValue == dtValue, 0, "$TxtConfigSetDesc$");
	AddMenuItem("$TxtBack$", Format("ConfigMenu(Object(%d),%d)", ObjectNumber(pClonk), dType), DTZ1, pClonk, 0, 0, "$TxtBackDesc$");
	SelectMenuItem(ArrayDataIndex(datValues, dtValue), pClonk);
	return 1;
}

protected func ConfigFunctions(object pClonk, int dType)
{
	if (!pClonk)
		return 0;
	CloseMenu(pClonk);
	CreateMenu(CLNK, pClonk, this, 0, "$TxtSelectConfigFunctions$", 0, 1);
	AddMenuItem("$TxtFunctionEqual$", Format("ConfigFunction_Equal(Object(%d),%d)", ObjectNumber(pClonk), dType), ERTH, pClonk, 0, 0, "$TxtFunctionEqualDesc$");
	AddMenuItem("$TxtFunctionValue$", Format("ConfigFunction_Value(Object(%d),%d)", ObjectNumber(pClonk), dType), ROCK, pClonk, 0, 0, "$TxtFunctionValueDesc$");
	AddMenuItem("$TxtFunctionValue2$", Format("ConfigFunction_Value(Object(%d),%d,1)", ObjectNumber(pClonk), dType), GOLD, pClonk, 0, 0, "$TxtFunctionValue2Desc$");
	AddMenuItem("$TxtFunctionAmount$", Format("ConfigFunction_Amount(Object(%d),%d)", ObjectNumber(pClonk), dType), WBRL, pClonk, 0, 0, "$TxtFunctionAmountDesc$");
	AddMenuItem("$TxtFunctionRatio$", Format("ConfigFunction_Ratio(Object(%d),%d)", ObjectNumber(pClonk), dType), ICE1, pClonk, 0, 0, "$TxtFunctionRatioDesc$");
	AddMenuItem("$TxtFunctionRandom10$", Format("ConfigFunction_Random(Object(%d),%d,10)", ObjectNumber(pClonk), dType), ICE1, pClonk, 0, 0, "$TxtFunctionRandom10Desc$");
	AddMenuItem("$TxtFunctionRandom100$", Format("ConfigFunction_Random(Object(%d),%d,100)", ObjectNumber(pClonk), dType), ICE1, pClonk, 0, 0, "$TxtFunctionRandom100Desc$");
	AddMenuItem("$TxtFunctionRandom1000$", Format("ConfigFunction_Random(Object(%d),%d,1000)", ObjectNumber(pClonk), dType), ICE1, pClonk, 0, 0, "$TxtFunctionRandom1000Desc$");
	AddMenuItem("$TxtFunctionRandom10000$", Format("ConfigFunction_Random(Object(%d),%d,10000)", ObjectNumber(pClonk), dType), ICE1, pClonk, 0, 0, "$TxtFunctionRandom10000Desc$");
	return 1;
}

protected func ConfigFunction_Equal(object pClonk, int dType)
{
	if (dType == 1)
		for (var i = 0; i < GetLength(daRessValue); i++)
			daRessValue[i] = 1;
	if (dType == 2)
		for (var i = 0; i < GetLength(daLiquidsValue); i++)
			daLiquidsValue[i] = 1;
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Value(object pClonk, int dType, bool bSquare)
{
	var dBasic = 100 + 900 * bSquare;
	if (dType == 1)
		for (var i = 0; i < GetLength(daRessValue); i++)
		{
			var idDef = idaRess[i];
			var dTemp = Max(1, dBasic / Max(1, GetValue(0, idDef)));
			if (bSquare)
				dTemp = Max(1, dTemp / Max(1, GetValue(0, idDef)));
			daRessValue[i] = dTemp;
		}
	if (dType == 2)
		for (var i = 0; i < GetLength(daLiquidsValue); i++)
		{
			var idDef = GetBarrelType(daLiquids[i]);
			var dTemp = Max(1, dBasic / Max(1, GetValue(0, idDef)));
			if (bSquare)
				dTemp = Max(1, dTemp / Max(1, GetValue(0, idDef)));
			daLiquidsValue[i] = dTemp * (!!idDef);
		}
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Amount(object pClonk, int dType)
{
	if (dType == 1)
		for (var i = 0; i < GetLength(daRessValue); i++)
		{
			var j = -1, dTemp = 0;
			while (Material(MaterialName(++j)) != -1)
			{
				if (GetMaterialVal("Dig2Object", "Material", j) == idaRess[i])
					dTemp += GetMaterialCount(j);
				else if (GetMaterialVal("Blast2Object", "Material", j) == idaRess[i])
					dTemp += GetMaterialCount(j);
			}
			daRessValue[i] = dTemp / 100;
		}
	if (dType == 2)
		for (var i = 0; i < GetLength(daLiquidsValue); i++)
			daLiquidsValue[i] = GetMaterialCount(daLiquids[i]) / 100;
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Ratio(object pClonk, int dType)
{
	if (dType == 1)
		for (var i = 0; i < GetLength(daRessValue); i++)
		{
			var j = -1, dTemp = 0;
			while (Material(MaterialName(++j)) != -1)
			{
				if (GetMaterialVal("Dig2Object", "Material", j) == idaRess[i])
					dTemp += GetMaterialCount(j) / Max(1, GetMaterialVal("Dig2ObjectRatio", "Material", j));
				else if (GetMaterialVal("Blast2Object", "Material", j) == idaRess[i])
					dTemp += GetMaterialCount(j) / Max(1, GetMaterialVal("Blast2ObjectRatio", "Material", j));
			}
			daRessValue[i] = dTemp;
		}
	if (dType == 2)
		for (var i = 0; i < GetLength(daLiquidsValue); i++)
		{
			var j = -1, dTemp = 0;
			while (Material(MaterialName(++j)) != -1)
			{
				if (MaterialName(daLiquids[i]) == MaterialName(j))
					dTemp += GetMaterialCount(j);
				else if (GetMaterialVal("AboveTempConvertTo", "Material", j) == MaterialName(daLiquids[i]))
					dTemp += GetMaterialCount(j);
				else if (GetMaterialVal("BelowTempConvertTo", "Material", j) == MaterialName(daLiquids[i]))
					dTemp += GetMaterialCount(j);
			}
			daLiquidsValue[i] = dTemp / 100;
		}
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}

protected func ConfigFunction_Random(object pClonk, int dType, int dSize)
{
	if (dType == 1)
		for (var i = 0; i < GetLength(daRessValue); i++)
			daRessValue[i] = Random(dSize + 1);
	if (dType == 2)
		for (var i = 0; i < GetLength(daLiquidsValue); i++)
			daLiquidsValue[i] = Random(dSize + 1);
	if (pClonk)
		ConfigMenu(pClonk, dType);
	return 1;
}
	
protected func InfoMenu(int dPlayer)
{
	var stInfo = Format("$TxtInfoHead$");
	for (var idtRess in idaRess) 
		stInfo = Format("%s|%d=%s (%d)", stInfo, GetMineValue(idtRess), GetName(0, idtRess), GetValue(0, idtRess));
	stInfo = Format("%s|%s", stInfo, "--------------------");
	for (var dtLiquid in daLiquids) 
		if (GetBarrelType(dtLiquid))
			stInfo = Format("%s|%d=%s (%d)", stInfo, GetPumpValue(dtLiquid), MaterialName(dtLiquid), GetValue(0, GetBarrelType(dtLiquid)));
		else
			stInfo = Format("%s|%d=%s", stInfo, GetPumpValue(dtLiquid), MaterialName(dtLiquid));
	MessageWindow(Format("$TxtInfoMessage$", GetDesc(), stInfo), dPlayer, GOLD, GetName());
	return 1;
}
	
protected func LockMenu(int dPlayer, bool bLock)
{
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
	CreateMenu(GetID(), pCursor, this, 0, "$TxtMenu$", 0, 1);
	AddMenuItem("$TxtLock$", Format("LockMenu(%d,1)", dPlayer), DTZ1, pCursor, 0, 0, "$TxtLockDesc$");
	AddMenuItem("$TxtBack$", Format("Menu(%d)", dPlayer), DTZ1, pCursor, 0, 0, "$TxtBackDesc$", 2, 8);
	SelectMenuItem(1, pCursor);
	return 1;
}
	
protected func MineRessSearch()
{
	idaRess = [];
	daRessValue = [];
	dRessValue = 0;
	var i = -1, idDef;
	while (idDef = GetDefinition(++i, C4D_Object))
		if (IsMineProduct(idDef))
			idaRess[GetLength(idaRess)] = idDef;
	for (var i = 0; i < GetLength(idaRess); i++)
	{
		daRessValue[i] = GetMineValue(idaRess[i], 1);
		dRessValue += daRessValue[i];
	}
	bLocked = 0;
	ScheduleCall(this, "Publish", 37 * 30);
	return 1;
}
	
public func IsMineProduct(id idDef)
{
	if (!idDef)
		return 0;
	return idDef->~IsMiningProduct();
}

public func GetMineValue(id idDef, bool bDefault)
{
	if (!idDef)
		return 0;
	if (!bDefault)
		return ArrayData(daRessValue, ArrayDataIndex(idaRess, idDef));
	var dTemp = idDef->~GetMiningRate();
	if (!dTemp)
		dTemp = Max(1, 1000 / Max(1, GetValue(0, idDef)) / Max(1, GetValue(0, idDef)));
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
	dLiquidsValue = 0;
	var i = -1;
	while (Material(MaterialName(++i)) != -1)
		if (IsPumpProduct(i))
			ArrayAdd(daLiquids, i);
	for (var i = 0; i < GetLength(daLiquids); i++)
	{
		daLiquidsValue[i] = GetPumpValue(daLiquids[i], 1);
		dLiquidsValue += daLiquidsValue[i];
	}
	bLocked = 0;
	ScheduleCall(this, "Publish", 37 * 30);
	return 1;
}
	
public func IsPumpProduct(int dnMat)
{
	if (Material(MaterialName(dnMat)) == -1)
		return 0;
	if (GetMaterialVal("Density", "Material", dnMat) != 25)
		return 0;
	return 1;
}

public func GetPumpValue(int dnMat, bool bDefault)
{
	if (!IsPumpProduct(dnMat))
		return 0;
	if (!bDefault)
		return ArrayData(daLiquidsValue, ArrayDataIndex(daLiquids, dnMat));
	;
	var dTemp = _inherited(dnMat);
	if (dTemp)
		return dTemp;
	var idBarrel = GetBarrelType(dnMat);
	if (!idBarrel)
		return 0;
	dTemp = GetValue(0, idBarrel);
	//dTemp=GetValue(0,idBarrel)-GetValue(0,idBarrel->~EmptyBarrelID());
	return Max(1, 100 / dTemp);
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