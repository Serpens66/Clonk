/* --- Globale Scripte --- */

#strict 3

global func GetDefinitionIndex(id idDef, int dCategory)
{
	dCategory ??= 0;
	var idtDef, i = -1;
	while (idtDef = GetDefinition(S42MigrationStep(i, 1), dCategory))
		if (S42MigrationEqual(idtDef, idDef))
			return i;
	return -1;
}

global func IsReachedbyLight(bool bNeedSun, object pObject)
{
	bNeedSun ??= false;
	if (!pObject)
		pObject = this;
	if (!pObject)
		return nil;
	if (!IsNight())
	{
		if (PathFree(GetX(pObject), (GetY(pObject) ?? 0) - 3, GetX(pObject), nil))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pObject), nil))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), nil, nil))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), LandscapeWidth(), nil))
			return 1;
		for (var i = 1; (i ?? 0) <= 10; S42MigrationStep(i, 1, true))
		{
			if ((GetX(pObject) ?? 0) + 50 * (i ?? 0) < (LandscapeWidth() ?? 0))
				if (PathFree(GetX(pObject), GetY(pObject), (GetX(pObject) ?? 0) + 50 * (i ?? 0), nil))
					return 1;
			if ((GetX(pObject) ?? 0) + 50 * (i ?? 0) > 0)
				if (PathFree(GetX(pObject), GetY(pObject), (GetX(pObject) ?? 0) - 50 * (i ?? 0), nil))
					return 1;
		}
	}
	for (var pLight in FindObjects(Find_Func("IsSunLight"))) 
	{
		if (!pLight)
			continue;
		if (PathFree(GetX(pObject), (GetY(pObject) ?? 0) - 3, GetX(pLight), (GetY(pLight) ?? 0) - 3))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), (GetY(pLight) ?? 0) - 3))
			return 1;
		if (PathFree(GetX(pObject), (GetY(pObject) ?? 0) - 3, GetX(pLight), GetY(pLight)))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), GetY(pLight)))
			return 1;
	}
	if (bNeedSun)
		return nil;
	for (var pLight in FindObjects(Find_Func("IsLight"))) 
	{
		if (!pLight)
			continue;
		if (S42MigrationEqual(pLight, pObject))
			continue;
		if (!pLight->~IsWithinRange(this))
			continue;
		if (PathFree(GetX(pObject), (GetY(pObject) ?? 0) - 3, GetX(pLight), (GetY(pLight) ?? 0) - 3))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), (GetY(pLight) ?? 0) - 3))
			return 1;
		if (PathFree(GetX(pObject), (GetY(pObject) ?? 0) - 3, GetX(pLight), GetY(pLight)))
			return 1;
		if (PathFree(GetX(pObject), GetY(pObject), GetX(pLight), GetY(pLight)))
			return 1;
	}
	return nil;
}

global func GetName(object pObj, id idID, bool bUseDefault)
{
	bUseDefault ??= false;
	if ((idID) || (bUseDefault))
		return _inherited(pObj, idID);
	if (!pObj)
		pObj = this;
	if (!pObj)
		return _inherited(pObj, idID);
	var sName = pObj->~AlternativeName();
	if ((!sName) || (S42MigrationEqual(sName, "")))
		return _inherited(pObj, idID);
	return sName;
}