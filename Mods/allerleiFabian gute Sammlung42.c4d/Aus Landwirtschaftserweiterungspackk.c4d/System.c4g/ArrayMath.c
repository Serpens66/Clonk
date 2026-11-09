/* --- Array Mathematik --- */

#strict 3

global func ArrayMathAddArray(& anArray, array anArrayAdd)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetLength(anArray), GetLength(anArrayAdd)))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if ((IsNumeric(anArray[(i ?? 0)])) && (IsNumeric(anArrayAdd[(i ?? 0)])))
			S42MigrationAssign(anArray[(i ?? 0)], "+=", (anArrayAdd[(i ?? 0)] ?? 0));
	return GetLength(anArray);
}

global func ArrayMathAddInt(& anArray, int dAdd)
{
	dAdd ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if (IsNumeric(anArray[(i ?? 0)]))
			S42MigrationAssign(anArray[(i ?? 0)], "+=", (dAdd ?? 0));
	return GetLength(anArray);
}

global func ArrayMathSubArray(& anArray, array anArraySub)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetLength(anArray), GetLength(anArraySub)))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if ((IsNumeric(anArray[(i ?? 0)])) && (IsNumeric(anArraySub[(i ?? 0)])))
			S42MigrationAssign(anArray[(i ?? 0)], "-=", (anArraySub[(i ?? 0)] ?? 0));
	return GetLength(anArray);
}

global func ArrayMathSubInt(& anArray, int dSub)
{
	dSub ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if (IsNumeric(anArray[(i ?? 0)]))
			S42MigrationAssign(anArray[(i ?? 0)], "-=", (dSub ?? 0));
	return GetLength(anArray);
}

global func ArrayMathMulArray(& anArray, array anArrayMul)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetLength(anArray), GetLength(anArrayMul)))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if ((IsNumeric(anArray[(i ?? 0)])) && (IsNumeric(anArrayMul[(i ?? 0)])))
			S42MigrationAssign(anArray[(i ?? 0)], "*=", (anArrayMul[(i ?? 0)] ?? 0));
	return GetLength(anArray);
}

global func ArrayMathMulInt(& anArray, int dMul)
{
	dMul ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if (IsNumeric(anArray[(i ?? 0)]))
			S42MigrationAssign(anArray[(i ?? 0)], "*=", (dMul ?? 0));
	return GetLength(anArray);
}

global func ArrayMathDivArray(& anArray, array anArrayDiv)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetLength(anArray), GetLength(anArrayDiv)))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if ((S42MigrationEqual(GetType(anArray[(i ?? 0)]), C4V_Int)) && (S42MigrationEqual(GetType(anArrayDiv[(i ?? 0)]), C4V_Int)) && (!S42MigrationEqual(anArrayDiv[(i ?? 0)], nil)))
			S42MigrationAssign(anArray[(i ?? 0)], "/=", (anArrayDiv[(i ?? 0)] ?? 0));
	return GetLength(anArray);
}

global func ArrayMathDivInt(& anArray, int dDiv)
{
	dDiv ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!dDiv)
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if (IsNumeric(anArray[(i ?? 0)]))
			S42MigrationAssign(anArray[(i ?? 0)], "/=", (dDiv ?? 0));
	return GetLength(anArray);
}

global func ArrayMathModArray(& anArray, array anArrayMod)
{
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!S42MigrationEqual(GetLength(anArray), GetLength(anArrayMod)))
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if ((IsNumeric(anArray[(i ?? 0)])) && (IsNumeric(anArrayMod[(i ?? 0)])) && (!S42MigrationEqual(anArrayMod[(i ?? 0)], nil)))
			S42MigrationAssign(anArray[(i ?? 0)], "%=", (anArrayMod[(i ?? 0)] ?? 0));
	return GetLength(anArray);
}

global func ArrayMathModInt(& anArray, int dMod)
{
	dMod ??= 0;
	if (!S42MigrationEqual(GetType(anArray), C4V_Array))
		return -1;
	if (!dMod)
		return -1;
	if (S42MigrationEqual(GetLength(anArray), nil))
		return GetLength(anArray);
	for (var i = nil; (i ?? 0) < (GetLength(anArray) ?? 0); S42MigrationStep(i, 1, true))
		if (IsNumeric(anArray[(i ?? 0)]))
			S42MigrationAssign(anArray[(i ?? 0)], "%=", (dMod ?? 0));
	return GetLength(anArray);
}

global func ArraySum(array danArray)
{
	var dResult = nil;
	for (var dValue in danArray) 
		S42MigrationAssign(dResult, "+=", (dValue ?? 0));
	return dResult;
}

//Ist eine Zahl
global func IsNumeric(xValue)
{
	return (S42MigrationEqual(xValue, nil)) || (S42MigrationEqual(GetType(xValue), C4V_Int));
} 