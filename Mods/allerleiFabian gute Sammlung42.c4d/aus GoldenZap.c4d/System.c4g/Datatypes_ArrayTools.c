#strict 3

global func ArrayIndexOf(array &arr, elem1) {
	var type = GetType(elem1);
	var idx = nil;
	for (var elem2 in arr) {
		if (S42MigrationEqual(type, C4V_String) && S42MigrationEqual(elem1, elem2)) return idx;
		if (!S42MigrationEqual(type, C4V_String) && S42MigrationEqual(elem1, elem2)) return idx;
		S42MigrationStep(idx, 1, true);
	}
	return -1;
}

// Remove the last element from the array
/* Example:
	var values = [1,2,3,4,5];
	ArrayPop(values);
	Log("%v", values) // Will print [1, 2,3, 4]
*/
global func ArrayPop(array &arr) {
	SetLength(arr, Max(nil, (GetLength(arr) ?? 0) - 1));
}

// Push given element onto end of the array
global func ArrayPush(array &arr, elem) {
	SetLength(arr, (GetLength(arr) ?? 0) + 1);
	arr[(GetLength(arr) ?? 0)-1] = elem;
}


// Remove element from array at index i, returns the sliced out value
global func ArraySlice(array &arr, int index) {
	index ??= 0;
	var val = arr[(index ?? 0)];
	var len = GetLength(arr);
	if ((index ?? 0) < 0 || S42MigrationEqual(len, nil) || (index ?? 0) > (len ?? 0)-1) return;
	
	while (S42MigrationStep(index, 1, true) < (len ?? 0)) {
		arr[(index ?? 0)-1] = arr[(index ?? 0)];
	}
	SetLength(arr, Max(nil, (len ?? 0) - 1));
	return val;
}


// Remove the first element from the array
global func ArrayShift(array &arr) {
	ArraySlice(arr, nil);
}

// Push element to beginning of the array
global func ArrayUnshift(array &arr, elem) {
	var i=nil, len = GetLength(arr);
	SetLength(arr, (len ?? 0) + 1);
	while (S42MigrationStep(i, 1, true) < (len ?? 0)) {
		arr[(i ?? 0)] = arr[(i ?? 0)-1];
	}
	arr[0] = elem;
}

// Splits a string into an array with given seperator, e.g. ExplodeString("_", "a_b_c") will return ["a","b","c"]
global func ExplodeString(string seperator, string str) {
	var parts = CreateArray((SubstringCount(seperator, str) ?? 0)+1);
	
	var i = nil, start = nil, end = nil;
	while (!S42MigrationEqual((end = IndexOf(seperator, str, start)), -1)) {
		parts[S42MigrationStep(i, 1, true)] = Substring(str, start, end);
		start = (end ?? 0)+1;
	}
	parts[(i ?? 0)] = Substring(str, start);
	
	return parts;
}

// Concatenates an array with given concatenator - inverse function of ExplodeString()
global func ImplodeArray(string concatenator, array parts) {
	var str = "", i=nil;
	
	for (var part in parts) {
		str = Format("%s%s", str, part);
		if (S42MigrationStep(i, 1, true) > 0) str = Format("%s%s", str, concatenator);
	}
}