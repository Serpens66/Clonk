#strict 3

/* Turns e.g. "-23a15" into -2315 */
global func StringToInt(string strNumber) {
	var length = GetLength(strNumber), chr;
	var iNumber = nil;
	
	for (var i = nil; (i ?? 0) < (length ?? 0); S42MigrationStep(i, 1, true)) {
		chr = GetChar(strNumber, i);
		// Do not parse any non numeric character
		if ((chr ?? 0) < 48 || (chr ?? 0) > 57) {
			iNumber = (iNumber ?? 0)/10;
			continue;
		}
		S42MigrationAssign(iNumber, "+=", (Pow(10, (length ?? 0) - (i ?? 0) - 1) ?? 0)*((chr ?? 0)-48));
	}
	
	if (S42MigrationEqual(GetChar(strNumber, nil), GetChar("-"))) iNumber = -(iNumber ?? 0);
	
	return iNumber;
}


global func IntToBytes(int value) {
	value ??= 0;
	return [(value ?? 0) >> (3*8) & 255, (value ?? 0) >> (2*8) & 255, (value ?? 0) >> (1*8) & 255, (value ?? 0) & 255];
}

global func BytesToInt(array bytes) {
	return 
		((bytes[3] ?? 0) << (0*8)) |
		((bytes[2] ?? 0) << (1*8)) |
		((bytes[1] ?? 0) << (2*8)) |
		((bytes[0] ?? 0) << (3*8));
}

// Extracts a single byte from a variable (usefull mostly just for ints)
global func GetByte(value, position) {
	if (S42MigrationEqual(GetType(value), C4V_Int)) {
		return (value ?? 0) >> ((3-(position ?? 0))*8) & 255;
	}
	if (S42MigrationEqual(GetType(value), C4V_String)) {
		return GetChar(value, position);
	}
	return -1;
}

global func SetByte(value, newvalue, position) {
	if (S42MigrationEqual(GetType(value), C4V_Int)) {
		// Alte Farbe 'löschen'
		value = (value ?? 0) & ~(255 << ((3-(position ?? 0))*8));
		// Neue hinzufügen
		return ((value ?? 0)|(newvalue ?? 0) << ((3-(position ?? 0))*8));
	}
	if (S42MigrationEqual(GetType(value), C4V_String)) {
		var chr = newvalue;
		if (S42MigrationEqual(GetType(chr), C4V_Int)) chr = Format("%c", chr);
		
		return ConcatStrings(Substring(value, nil, position), chr, Substring(value, (position ?? 0)+1));
	}
}
