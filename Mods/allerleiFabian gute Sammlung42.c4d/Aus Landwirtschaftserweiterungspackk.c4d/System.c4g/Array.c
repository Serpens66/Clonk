/* --- Array Funktionen --- */

#strict 3

//Gibt das erste Vorkommen an
global func ArrayDataIndex(array anArray, xnSearch, int dnStart)
{
	dnStart ??= 0;
	if (!GetLength(anArray))
		return -1;
	if ((dnStart ?? 0) >= (GetLength(anArray) ?? 0))
		return -1;
	for (var i = dnStart; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if (S42MigrationEqual(anArray[(i ?? 0)], xnSearch))
			return i;
	return -1;
}

//Gibt die Anzahl der Vorkommen an
global func ArrayDataCount(array anArray, xnSearch)
{
	if (!GetLength(anArray))
		return -1;
	var dtAnz = nil;
	for (var xTemp in anArray) 
		if (S42MigrationEqual(xTemp, xnSearch))
			S42MigrationStep(dtAnz, 1, true);
	return dtAnz;
}

//Hängt einen Wert an den Array an
global func ArrayAdd(& anArray, xnNew, bool bNoDuplicates)
{
	bNoDuplicates ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (bNoDuplicates)
	{
		var dTemp = ArrayDataIndex(anArray, xnNew);
		if (!S42MigrationEqual(dTemp, -1))
			return dTemp;
	}
	anArray[(GetLength(anArray) ?? 0)] = xnNew;
	return (GetLength(anArray) ?? 0) - 1;
}

//Sortiert einen Wert an einer Bestimmten Position ein
global func ArraySortAdd(& anArray, xnNew, string sSortFunc, bool bReverse)
{
	bReverse ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!GetLength(anArray))
		return ArrayAdd(anArray, xnNew);
	var i = -1;
	while (S42MigrationStep(i, 1) < (GetLength(anArray) ?? 0))
		if (!S42MigrationEqual(anArray[(i ?? 0)], xnNew))
			if (S42MigrationEqual(eval(Format("Sort_%s(%v,%v)", sSortFunc, anArray[(i ?? 0)], xnNew)), bReverse))
				return ArrayInsert(anArray, xnNew, i);
	return ArrayAdd(anArray, xnNew);
}

//Fügt einen Wert an einer Position ein
global func ArrayInsert(& anArray, xnNew, int dPosition)
{
	dPosition ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if ((dPosition ?? 0) < 0)
		return -1;
	if ((dPosition ?? 0) >= (GetLength(anArray) ?? 0))
	{
		anArray[(dPosition ?? 0)] = xnNew;
		return dPosition;
	}
	var dTemp = GetLength(anArray);
	while (S42MigrationStep(dTemp, -1) >= (dPosition ?? 0))
	{
		anArray[(dTemp ?? 0) + 1] = anArray[(dTemp ?? 0)];
	}
	anArray[(dPosition ?? 0)] = xnNew;
	return dPosition;
}

//Löscht einen Index (andere Objekte rutschen nach)
global func ArrayDeleteIndex(& anArray, int dPosition)
{
	dPosition ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!Inside(dPosition, nil, (GetLength(anArray) ?? 0) - 1))
		return -1;
	var dTemp = (dPosition ?? 0) - 1;
	while (S42MigrationStep(dTemp, 1) < (GetLength(anArray) ?? 0) - 1)
	{
		anArray[(dTemp ?? 0)] = anArray[(dTemp ?? 0) + 1];
	}
	SetLength(anArray, (GetLength(anArray) ?? 0) - 1);
	return GetLength(anArray);
}

//Löscht das erste/alle Vorkommen des Eintrages
global func ArrayDeleteEntry(& anArray, xnEntry, bool bDuplicates)
{
	bDuplicates ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!GetLength(anArray))
		return nil;
	var dTemp, dAnz = nil;
	while ((!S42MigrationEqual((dTemp = ArrayDataIndex(anArray, xnEntry)), -1)) && ((dAnz ?? 0) < 20))
	{
		ArrayDeleteIndex(anArray, dTemp);
		if (!bDuplicates)
			return dTemp;
		S42MigrationStep(dAnz, 1, true);
	}
	return dAnz;
}

//Schnittmenge zweier Arrays
global func ArrayAndArray(& anArray, array anArray2, bool bNoDuplicates)
{
	bNoDuplicates ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetType(anArray2), C4V_Array))
		return -1;
	if ((!GetLength(anArray)) || (!GetLength(anArray2)))
	{
		anArray = [];
		return nil;
	}
	var aResult = [];
	for (var xTemp in anArray) 
		if (ArrayDataCount(anArray2, xTemp))
			ArrayAdd(aResult, xTemp, bNoDuplicates);
	anArray = aResult;
	return GetLength(anArray);
}

//Verschmiltz zwei Arrays
global func ArrayAddArray(& anArray, array anArray2, bool bNoDuplicates)
{
	bNoDuplicates ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetType(anArray2), C4V_Array))
		return -1;
	if (!GetLength(anArray2))
		return GetLength(anArray);
	for (var xTemp in anArray2) 
		ArrayAdd(anArray, xTemp, bNoDuplicates);
	return GetLength(anArray);
}

//Löscht alle einträge von Array2 in Array1
global func ArrayRemoveArrayEntries(& anArray, array anArray2)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetType(anArray2), C4V_Array))
		return -1;
	if (!GetLength(anArray2))
		return GetLength(anArray);
	for (var xTemp in anArray2) 
		ArrayDeleteEntry(anArray, xTemp, 1);
	return GetLength(anArray);
}

//Löscht alle doppelten Einträge
global func ArrayRemoveDuplicates(& anArray)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	var atArray = anArray;
	anArray = [];
	ArrayAddArray(anArray, atArray, 1);
	return GetLength(anArray);
}

//Sortiert Arrays
global func ArraySort(& anArray, string sSortFunc, bool bReverse)
{
	bReverse ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!GetLength(anArray))
		return nil;
	var i = -1;
	while (S42MigrationStep(i, 1) < (GetLength(anArray) ?? 0) - 1)
		if (!S42MigrationEqual(anArray[(i ?? 0)], anArray[(i ?? 0) + 1]))
			if (S42MigrationEqual(eval(Format("Sort_%s(%v,%v)", sSortFunc, anArray[(i ?? 0)], anArray[(i ?? 0) + 1])), bReverse))
			{
				ArraySortAdd(anArray, anArray[(i ?? 0) + 1], sSortFunc, bReverse);
				ArrayDeleteIndex(anArray, (i ?? 0) + 2);
			}
	return GetLength(anArray);
}

global func Sort_IDOrder(id idDef, id idDef2)
{
	return Sort_NumericOrder(GetDefinitionIndex(idDef), GetDefinitionIndex(idDef2));
}

global func Sort_NumericOrder(int dInt, int dInt2)
{
	dInt ??= 0;
	dInt2 ??= 0;
	return (dInt ?? 0) < (dInt2 ?? 0);
}

//Filtert den Arrayinhalt
global func ArrayFilter(& anArray, string sFilterFunc, xnFilterPara, bool bInvert)
{
	bInvert ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!GetLength(anArray))
		return nil;
	var atArray = [];
	for (var xtEntry in anArray) 
		if (S42MigrationEqual(GetType(xtEntry), C4V_C4Object))
		{
			if (!S42MigrationEqual(eval(Format("Filter_%s(Object(%d),%v)", sFilterFunc, ObjectNumber(xtEntry), xnFilterPara)), bInvert))
				ArrayAdd(atArray, xtEntry);
		}
		else
		{
			if (!S42MigrationEqual(eval(Format("Filter_%s(%v,%v)", sFilterFunc, xtEntry, xnFilterPara)), bInvert))
				ArrayAdd(atArray, xtEntry);
		}
	anArray = atArray;
	return GetLength(anArray);
}

//Geschützer Array Zugriff
global func ArrayData(array anArray, int dnIndex, xExeption)
{
	dnIndex ??= 0;
	if (Inside(dnIndex, nil, (GetLength(anArray) ?? 0) - 1))
		return anArray[(dnIndex ?? 0)];
	return xExeption;
}

//Zahlen im Array anpassen
global func ArrayBoundBy(& anArray, int dnMin, int dnMax, bool bRemoveDuplicates)
{
	dnMin ??= 0;
	dnMax ??= 0;
	bRemoveDuplicates ??= false;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if ((S42MigrationEqual(GetType(anArray[(i ?? 0)]), C4V_Int)) || (S42MigrationEqual(anArray[(i ?? 0)], nil)))
			anArray[(i ?? 0)] = BoundBy(anArray[(i ?? 0)], dnMin, dnMax);
	if (bRemoveDuplicates)
		ArrayRemoveDuplicates(anArray);
	return GetLength(anArray);
}

//Zufälliger Array Eintrag
global func ArrayRandomEntry(array anArray)
{
	return anArray[(Random(GetLength(anArray)) ?? 0)];
}