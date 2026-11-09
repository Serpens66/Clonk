/* --- Erweiterter Pumpenscript --- */

#strict 2

// Flüssigkeit aus Fässern im Objekt extrahieren
global func ObjectExtractLiquid(object pFrom)
{
	if (pFrom->~IsLiquidContainer())
	{
		var dMat = pFrom->~LiquidOutput(-2, 1, this);
		if (dMat != -1)
			return dMat;
	}
	return _inherited(pFrom);
}

