/* --- Tiefbrunnen --- */

#strict 2

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
		pRule = CreateObject(DTBB, -GetX(), -GetY(), NO_OWNER);
	dMatPref = -1;
	if (GetY() > LandscapeHeight() - 100)
		return 1;
	Message("$TxtWrongPlacement$", this, GetY(), LandscapeHeight() - 100);
	Sound("Error");
	ChangeDef(CNKT);
	return 1;
}

public func RejectConstruction(int dX, int dY, object pClonk)
{
	if (GetY(pClonk) > LandscapeHeight() - 50)
		return 0;
	Message("$TxtWrongPlacement$", this, GetY(pClonk) + dY + 50, LandscapeHeight() - 100);
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
		return 0;
	CloseMenu(pClonk);
	CreateMenu(BARL, pClonk, this, 0, "$TxtSelectLiquid$", 0, 1, 0);
	var daLiquids = GetPumpProductList();
	AddMenuItem("$TxtConfigDefault$", Format("ChangeConfig(Object(%d),%d)", ObjectNumber(pClonk), -1), BARL, pClonk, 0, 0, "$TxtProductionDefaultDesc$");
	for (var dLiquid in daLiquids) 
		AddMenuItem(MaterialName(dLiquid), Format("ChangeConfig(Object(%d),%d)", ObjectNumber(pClonk), dLiquid), GetBarrelType(dLiquid), pClonk, 0, 0, Format("$TxtProductionLiquidDesc$", MaterialName(dLiquid), GetPumpValue(dLiquid) * 5, GetPumpTotalValue() + GetPumpValue(dLiquid) * 5));
	return 1;
}

protected func ChangeConfig(object pClonk, int dnMat)
{
	dMatPref = dnMat;
	return 1;
}

protected func IsPumpProduct(int dnMat)
{
	if (!pRule)
		return 0;
	if (dnMat == -1)
		return 0;
	return pRule->~IsPumpProduct(dnMat);
}

protected func GetPumpValue(int dnMat)
{
	if (!pRule)
		return 0;
	if (dnMat == -1)
		return 0;
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
		return 0;
	return pRule->~GetPumpTotalValue();
}

protected func GetPumpProductMaterial(int dnRandom)
{
	if (!pRule)
		return 0;
	if (!dnRandom)
		dnRandom = Random(GetPumpTotalValue() + GetPumpValue(dMatPref) * 4);
	if (dnRandom > GetPumpTotalValue())
		return dMatPref;
	var i = -1, datLiquids = GetPumpProductList();
	while (dnRandom >= 0)
	{
		i++;
		dnRandom -= GetPumpValue(datLiquids[i]);
	}
	return datLiquids[i];
}

protected func Check()
{
	if (GetCon() < 100)
		return 0;
	if (!EnergyCheck(100))
		return 0;
	DoEnergy(-1);
	if (dMatAmount)
	{
		Message("@<c %x>%s:|%dl</c>", this, GetMaterialColorX(dMat, 1), MaterialName(dMat), dMatAmount);
		return 1;
	}
	dDepth++;
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
	if (dnAmount == 0)
		return -2;
	if (dMat == -1)
		return -2;
	if ((dnMat != dMat) && (dnMat != -2))
		return -2;
	if (!EnergyCheck(100))
		return 0;
	if (dMatAmount - dnAmount < 0)
		return -2;
	if (pnLine)
	{
		if (GetID(pnLine) == DPIP)
		{
			if (GetActionTarget(0, pnLine) != this)
				return -2;
			if (!PUMP->~CheckCapacity(GetActionTarget(1, pnLine), dnAmount, dMat))
				return -2;
		}
		else
		{
			if (GetActionTarget(0, pnLine)->~IsLiquidConatiner())
				if (!GetActionTarget(0, pnLine)->~CheckOwnCapacity(dMat, dnAmount))
					return -2;
		}
	}
	dMatAmount -= dnAmount;
	return dMat;
}

public func GetDepth()
{
	return dDepth;
}