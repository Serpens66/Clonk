/* --- Tiefbrunnen --- */

#strict 3

#include BAS8
#include CXEC

local dDepth,dMatPref;
local pRule;
local dMat;
local dMatAmount;

public func MatChance()
{
	return 120;
}
public func MatMinAmount()
{
	return 1000;
}
public func MatMaxAmount()
{
	return 5000;
}

protected func Initialize()
{
	pRule = FindObject(DTBB);
	if (!pRule)
		pRule = CreateObject(DTBB, -(GetX() ?? 0), -(GetY() ?? 0), NO_OWNER);
	dMatPref = -1;
	if ((GetY() ?? 0) > (LandscapeHeight() ?? 0) - 100)
		return 1;
	Message("$TxtWrongPlacement$", this, GetY(), (LandscapeHeight() ?? 0) - 100);
	Sound("Error");
	ChangeDef(CNKT);
	return 1;
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

protected func ContextConfig(object pClonk)
{
	[$TxtConfig$|Image=GOLD]
	return Config(pClonk);
}

protected func ControlThrow(object pClonk)
{
	[$TxtConfig$|Image=GOLD]
	return Config(pClonk);
}

protected func Config(object pClonk)
{
	if (!pClonk)
		return nil;
	CloseMenu(pClonk);
	CreateMenu(BARL, pClonk, this, nil, "$TxtSelectLiquid$", nil, 1, nil);
	var daLiquids = GetPumpProductList();
	AddMenuItem("$TxtConfigDefault$", Format("ChangeConfig(Object(%d),%d)", ObjectNumber(pClonk), -1), BARL, pClonk, nil, nil, "$TxtProductionDefaultDesc$");
	for (var dLiquid in daLiquids) 
		AddMenuItem(MaterialName(dLiquid), Format("ChangeConfig(Object(%d),%d)", ObjectNumber(pClonk), dLiquid), GetBarrelType(dLiquid), pClonk, nil, nil, Format("$TxtProductionLiquidDesc$", MaterialName(dLiquid), (GetPumpValue(dLiquid) ?? 0) * 5, (GetPumpTotalValue() ?? 0) + (GetPumpValue(dLiquid) ?? 0) * 5));
	return 1;
}

protected func ChangeConfig(object pClonk, int dnMat)
{
	dnMat ??= 0;
	dMatPref = dnMat;
	return 1;
}

protected func IsPumpProduct(int dnMat)
{
	dnMat ??= 0;
	if (!pRule)
		return nil;
	if (S42MigrationEqual(dnMat, -1))
		return nil;
	return pRule->~IsPumpProduct(dnMat);
}

protected func GetPumpValue(int dnMat)
{
	dnMat ??= 0;
	if (!pRule)
		return nil;
	if (S42MigrationEqual(dnMat, -1))
		return nil;
	return pRule->~GetPumpValue(dnMat);
}

protected func GetPumpProductList()
{
	if (!pRule)
		return [];
	return pRule->~GetPumpProductList();
}

protected func GetPumpTotalValue()
{
	if (!pRule)
		return nil;
	return pRule->~GetPumpTotalValue();
}

protected func GetPumpProductMaterial(int dnRandom)
{
	dnRandom ??= 0;
	if (!pRule)
		return nil;
	if (!dnRandom)
		dnRandom = Random((GetPumpTotalValue() ?? 0) + (GetPumpValue(dMatPref) ?? 0) * 4);
	if ((dnRandom ?? 0) > (GetPumpTotalValue() ?? 0))
		return dMatPref;
	var i = -1, datLiquids = GetPumpProductList();
	while ((dnRandom ?? 0) >= 0)
	{
		S42MigrationStep(i, 1, true);
		S42MigrationAssign(dnRandom, "-=", (GetPumpValue(datLiquids[(i ?? 0)]) ?? 0));
	}
	return datLiquids[(i ?? 0)];
}

protected func Check()
{
	if ((GetCon() ?? 0) < 100)
		return nil;
	if (!EnergyCheck(100))
		return nil;
	DoEnergy(-1);
	if (dMatAmount)
	{
		Message("@<c %x>%s:|%dl</c>", this, GetMaterialColorX(dMat, 1), MaterialName(dMat), dMatAmount);
		return 1;
	}
	S42MigrationStep(dDepth, 1, true);
	Message("@$TxtDepth$", this, dDepth);
	if (Random(MatChance()))
		return 1;
	dMat = GetPumpProductMaterial();
	dMatAmount = RandomX(MatMinAmount(), MatMaxAmount());
	return 1;
}

public func IsLiquidContainer()
{
	return 1;
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
	if (!EnergyCheck(100))
		return nil;
	if ((dMatAmount ?? 0) - (dnAmount ?? 0) < 0)
		return -2;
	if (pnLine)
	{
		if (S42MigrationEqual(GetID(pnLine), DPIP))
		{
			if (!S42MigrationEqual(GetActionTarget(nil, pnLine), this))
				return -2;
			if (!PUMP->~CheckCapacity(GetActionTarget(1, pnLine), dnAmount, dMat))
				return -2;
		}
		else
		{
			if (GetActionTarget(nil, pnLine)->~IsLiquidConatiner())
				if (!GetActionTarget(nil, pnLine)->~CheckOwnCapacity(dMat, dnAmount))
					return -2;
		}
	}
	S42MigrationAssign(dMatAmount, "-=", (dnAmount ?? 0));
	return dMat;
}

public func GetDepth()
{
	return dDepth;
}