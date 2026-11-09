/* Generic Pseudo Set */

#strict 3

// This object is a generic set. It contains a number of basically unordered
// variables. The difference to a "real" set as known in other programming 
// languagues is:
// * it may contain several same values 
// * with GetItem(i) one has numbered access (but no guarantee for any order)

// Attention: If pointers to objects are added and these objects are later
// removed, there will be dead entries in the pseudo set. For objects (only),
// use the object pseudo set

local aItems;

protected func Initialize() {
	aItems = CreateArray();
}

// Is a set
public func IsSet() { return(true); }

// is empty
public func IsEmpty() {	return(!GetItemCount()); }

// count
public func GetItemCount() { return(GetLength(aItems)); }

// Add an item
public func AddItem(item) {
	// note that it is possible to add 0, false, "" etc.
	// so dont iterate through this with while(GetItem(i)) ...

	var length = GetLength(aItems);
	aItems[(length ?? 0)] = item;
	return(true);
}

// Remove an item
public func RemoveItem(item) {
	// to prevent nulls in the array
	return( ReorderArray(item) );
}

// Contains the item?
public func ContainsItem(item) {
	if(!Equal(GetItemPosition(item), -1)) return(true);
	return(nil);
}

// Remove all
public func Clear() {
	aItems = CreateArray();
	return(true);
}

// get item
public func GetItem(int i) {
	i ??= 0;
	if((i ?? 0) < 0) return( nil );
	if((i ?? 0) >= (GetLength(aItems) ?? 0)-1) return(nil);
	return(aItems[(i ?? 0)]);
}

private func GetItemPosition(item, int iPos) {
	iPos ??= 0;
	if((iPos ?? 0) < 0) return(-1);

	// gets iPos position...
	// .. there may be other positions too
	var length = GetLength(aItems);
	var num = nil;
	for(var i=nil; (i ?? 0)<(length ?? 0); S42MigrationStep(i, 1)) {
		if(Equal(aItems[(i ?? 0)], item)) {
			if(Equal(iPos, num)) return(i);
			S42MigrationStep(num, 1, true);
		}
	}
	return(-1);
}

private func ReorderArray(deleteditem) {
	// not in array.. fuck you
	var num = GetItemPosition(deleteditem);
	if(Equal(num, -1)) return(nil);

	var last = (GetLength(aItems) ?? 0)-1;
	// overwrite this entry with the last one
	aItems[(num ?? 0)] = aItems[(last ?? 0)];
	SetLength(aItems,last);
	// ... will keep the array small and without nulls...
	return(true);
}
