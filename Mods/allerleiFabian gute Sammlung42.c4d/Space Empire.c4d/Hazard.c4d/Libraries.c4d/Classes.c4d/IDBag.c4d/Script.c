/* Generic ID bag */

#strict 3
#include L_AI

// This object contains a number of IDs.

// The IDs have no properties. For example the Hazard-Munition-System and the
// Alchemy-Ingredient-System uses these kind of ID bags.
// It is faster than storing a number of (icon)-objects in it but doesn't
// allow non-definition properties.

// Attention: The bag should only contain Items. All contents are considered
// as items.

// Is a bag
public func IsBag() { return(true); }

// Add one item
public func AddItem(id idItem) { return(AddItems(idItem,1)); }

// Delete on item
public func RemoveItem(id idItem) { return(RemoveItems(idItem,1)); }

// Add items
public func AddItems(id idItem, int iNum) {
	iNum ??= 0;
	if((iNum ?? 0) < 0) return( nil );

	var obj;
	// if no object yet...
	if(!(obj=FindContents(idItem))) {
		obj = CreateContents(idItem);
	}
	// still no object (id not existant)
	if(!obj) return(nil);

	// increase..
	var newinfo = (GetInfo(obj,nil) ?? 0)+(iNum ?? 0);
	SetInfo(obj,nil,newinfo);
	return (true);
}

// Delete items
public func RemoveItems(id idItem, int iNum) {
	iNum ??= 0;
	if((iNum ?? 0) < 0) return(nil);

	var obj;
	if(!(obj=FindContents(idItem)))
		return( nil );

	// decrease..
	var newinfo = (GetInfo(obj,nil) ?? 0)-(iNum ?? 0);
	SetInfo(obj,nil,newinfo);

	// delete ID object if empty
	if((GetInfo(obj,nil) ?? 0) <= 0) {
		obj->RemoveObject();
	}

	return (true);
}

// Get item (like Contents only that each ID only comes one time)
public func GetItem(int i) {
	i ??= 0;
	var c = Contents(i);
	if(!c) return(nil);
	return(c->GetID());
}

// bag is empty
public func IsEmpty() {
	return(!Contents(nil));
}

// Remove all of idItem. If idItem = 0, remove all
public func RemoveAllItems(id idItem) {
	// none to remove: return false
	if(IsEmpty()) return(nil);

	// no id: remove all
	if(!idItem) {
		var obj;
		while(obj=Contents(nil))
			obj->RemoveObject();
	}
	// id: remove id
	else {
		var obj;
		if(obj=FindContents(idItem))
			obj->RemoveObject();
		else
			return(nil);
	}
	return (true);
}

// Get item count. If idItem = 0, get all
public func GetItemCount(id idItem) {
	// recursive add...
	if(!idItem) {
		var sum = nil;
		for(var i=nil; Contents(i); S42MigrationStep(i, 1)) {
			S42MigrationAssign(sum, "+=", (GetItemCount(GetID(Contents(i))) ?? 0));
		}
		return (sum);
	}
	// number of the contents
	else {
		var obj = FindContents(idItem);
		if(!obj) return(nil);
		
		return(GetInfo(obj,nil));
	}
}

// Transfer items to another bag
// if iCount = 0, transfer all
// if bStrict, only transfer if iCount is not more than actually in bag
public func TransferItems(object pBag, id idItem, int iCount, bool bStrict) {
	// All parameters must be given 
	iCount ??= 0;
	bStrict ??= false;
	if(!pBag) return(nil);
	if(!idItem) return(nil);
	if((iCount ?? 0) < 0) return(nil);

	var top = GetItemCount(idItem);
	// no sense in transfer if it is 0
	if(Equal(top, nil)) return( true );

	// transfer all
	if(!iCount) {
		// just displace the object (no calls)
		//obj->Exit();
		//obj->Enter(pBag);
		// could produce problems with RejectEntrance etc...

		iCount = top;
	}
	// iCount is too big?
	if((iCount ?? 0) > (top ?? 0)) {
		if(bStrict) {
			return(nil);
		}
		else {
			iCount = top;
		}
	}

	// transfer...
	pBag->AddItems(idItem,iCount);
	RemoveItems(idItem,iCount);

	return(true);
}

// Transfer all items into another bag
public func TransferAllItems(object pBag) {
	if(!pBag) return(nil);

	var obj;
	while(obj = Contents(nil))
		TransferItems(obj, GetID(obj), nil);

	return(true);
}

// Destruction
// protected func Destruction() {
//	RemoveAllItems();
//}
// Contents are removed automatically