/* Pumpenappendto */

#strict 2
#appendto PUMP
#appendto DRCK

protected func CheckCapacity(pTarget, int iAmount, iMaterial)
{
	if (pTarget->~IsLiquidContainer())
		if (pTarget->~CheckOwnCapacity(iMaterial, iAmount))
			return 1;
	return _inherited(pTarget, iAmount, iMaterial);
}