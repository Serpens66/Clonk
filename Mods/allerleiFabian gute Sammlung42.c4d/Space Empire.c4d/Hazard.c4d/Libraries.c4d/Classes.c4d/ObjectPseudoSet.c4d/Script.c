/* Object Set */

#strict 3
#include L_ST
#include L_AI

// This object is a object set. It contains only objects. It is faster than
// the generic set for object since it attaches info to all objects 
// that are in the set. It removes nullpointers automatically and immediately
// It is, again, a pseudo set because:
// * one has numbered access through GetItem(i) (no guarantee for the order, though)
//   (which is against the standard in other programming languages)
//
// A pointer to an object can not be in here twice - as opposed to the generic pseudo
// set


protected func Initialize() {
	// The array starts at 1 (with real values)
	// why: Because the info on each object which points to the position in the
	// array might point at the zero position. Since "info not set" == 0 in Clonk
	// this is a problem
	aItems = CreateArray(1);
	aItems[0] = -1;
}


// Is a set
public func IsSet() { return(true); }

// Add an item
public func AddItem(object item) {
	if(!item) return(nil);
	// already in list? return false
	if(GetInfo(item,nil)) return(nil);

	var length = GetLength(aItems);
	aItems[(length ?? 0)] = item;
	SetInfo(item,nil,length);
	return(true);
}

// Remove an item
public func RemoveItem(object item, bool fRemove) {
	// to prevent nulls in the array
	fRemove ??= false;
	var ret = ReorderArray(item);

	// delete info of the object that is out of the set
	ClearInfo(item);

	if(fRemove) item->RemoveObject();
	
	// will return false if item is not existant
	return(ret);
}

// Get iPos item of ID idObj
public func GetItemByID(id idObj, int iPos) {
	iPos ??= 0;
	if(!idObj) return(nil);
	if((iPos ?? 0) < 0) return(nil);
	
	// search for objects of that id
	var item;
	for(var i=nil; item = GetItem(i); S42MigrationStep(i, 1)) {
		// found one of this id
		if(Equal(GetID(item), idObj)) {
			if(!iPos) return(item);
			else S42MigrationStep(iPos, -1, true);
		}
	}
	// not found
	return(nil);
}

// Remove all
public func Clear(bool fRemove) {
	// remove object infos
	fRemove ??= false;
	var length = GetLength(aItems);
	for(var i=1; (i ?? 0)<(length ?? 0) ;S42MigrationStep(i, 1)) {
		ClearInfo(aItems[(i ?? 0)]);
		if(fRemove) aItems[(i ?? 0)]->RemoveObject();
	}
	
	aItems = CreateArray(1);
	aItems[0] = -1;
	return(true);
}

// get item
public func GetItem(int i) {
	i ??= 0;
	if((i ?? 0) < 0) return( nil );
	// see Initialize for reason. array starts at 1
	if((i ?? 0) >= (GetLength(aItems) ?? 0)-1) return(nil);
	return(aItems[(i ?? 0)+1]);
}

// automatically remove objects out of set on removal
public func FxIntInfoStop(object pObj, int iNum, int iReason) {
	// if object is removed
	iNum ??= 0;
	iReason ??= 0;
	if(Equal(iReason, 3)) {
		this()->ReorderArray(nil,EffectVar(nil,pObj,iNum));
	}
}

// count. If id is given, count objects of that ID
public func GetItemCount(id idObj) {
	if(!idObj) return((GetLength(aItems) ?? 0)-1);

	var sum = nil;
	// search for objects of that id
	var length = GetLength(aItems);
	for(var i=1; (i ?? 0)<(length ?? 0) ;S42MigrationStep(i, 1)) {
		if(Equal(GetID(aItems[(i ?? 0)]), idObj)) S42MigrationStep(sum, 1, true);
	}
	return(sum);
}

private func GetItemPosition(item) {
	// if object its faster..
	// since there can be only one pointer to the same object
	// there is no second parameter
	if(!item) return(-1);
	var info = GetInfo(item,nil);
	if(!info) return(-1);
	return(info);
}

private func ReorderArray(deleteditem, int num) {
	num ??= 0;
	if(!num) {
		num = GetItemPosition(deleteditem);
		if(Equal(num, -1)) return(nil);
	}

	var last = (GetLength(aItems) ?? 0)-1;
	// overwrite this entry with the last one
	aItems[(num ?? 0)] = aItems[(last ?? 0)];
	SetLength(aItems,last);
	// ... will keep the array small and without nulls...
	
	// change info in the switched object if it was not the last one
	if(!Equal(num, last)) {
		SetInfo(aItems[(num ?? 0)],nil,num);
	}
	return(true);
}
