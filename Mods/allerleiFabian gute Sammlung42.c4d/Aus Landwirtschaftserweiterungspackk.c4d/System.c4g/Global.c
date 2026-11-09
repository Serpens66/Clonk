/* --- Globale Scripte --- */

#strict 2

global func GetDefinitionIndex(id idDef, int dCategory)
{
	var idtDef, i = -1;
	while (idtDef = GetDefinition(++i, dCategory))
		if (idtDef == idDef)
			return i;
	return -1;
}

global func IsReachedbyLight(bool bNeedSun, object pObject)
{
	if (!pObject)
		pObject = this;
	if (!pObject)
		return 0;
	if (!IsNight())
	{
		if (PathFree(GetX(pObject), GetY(pObject) - 3, GetX(pObject), 0))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pObject), 0))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), 0, 0))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), LandscapeWidth(), 0))
			return 1;
		for (var i = 1; i <= 10; i++)
		{
			if (GetX(pObject) + 50 * i < LandscapeWidth())
				if (PathFree(GetX(pObject), GetY(pObject), GetX(pObject) + 50 * i, 0))
					return 1;
			if (GetX(pObject) + 50 * i > 0)
				if (PathFree(GetX(pObject), GetY(pObject), GetX(pObject) - 50 * i, 0))
					return 1;
		}
	}
	for (var pLight in FindObjects(Find_Func("IsSunLight"))) 
	{
		if (!pLight)
			continue;
		if (PathFree(GetX(pObject), GetY(pObject) - 3, GetX(pLight), GetY(pLight) - 3))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), GetY(pLight) - 3))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject) - 3, GetX(pLight), GetY(pLight)))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), GetY(pLight)))
			return 1;
	}
	if (bNeedSun)
		return 0;
	for (var pLight in FindObjects(Find_Func("IsLight"))) 
	{
		if (!pLight)
			continue;
		if (pLight == pObject)
			continue;
		if (!pLight->~IsWithinRange(this))
			continue;
		if (PathFree(GetX(pObject), GetY(pObject) - 3, GetX(pLight), GetY(pLight) - 3))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), GetY(pLight) - 3))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject) - 3, GetX(pLight), GetY(pLight)))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), GetY(pLight)))
			return 1;
	}
	return 0;
}

global func GetName(object pObj, id idID, bool bUseDefault)
{
	if ((idID) || (bUseDefault))
		return _inherited(pObj, idID);
	if (!pObj)
		pObj = this;
	if (!pObj)
		return _inherited(pObj, idID);
	var sName = pObj->~AlternativeName();
	if ((!sName) || (sName == ""))
		return _inherited(pObj, idID);
	return sName;
}