/* Attach information functionality */

#strict 3

// Attaches information to any object for exclusively this object
// So it is some personal information of one object that can be
// attached to any object. There can be an infinite number of infos

public func HasAttachInfo() { return(true); }

public func InfoName() { return("IntInfo"); }

// sets an info of that object for this object
public func SetInfo(object pObj, int iNum, info) {
	iNum ??= 0;
	if(!pObj) return(nil);

	// Info not existant yet
	var num = InfoNumber(pObj);
	if(Equal(num, -1)) {
		// ...create
		num = AddEffect(InfoName(),pObj,1,nil,this());
	}
	// set info
	EffectVar(iNum,pObj,num) = info;
	return(true);
}

// Clears ALL info of that object for this object
public func ClearInfo(object pObj) {
	if(!pObj) return(nil);

	var num = InfoNumber(pObj);
	return(RemoveEffect(nil,pObj,num));
}

// Get info of that object for this object
public func GetInfo(object pObj, int iNum) {
	iNum ??= 0;
	if(!pObj) return(nil);

	var num = InfoNumber(pObj);
	// info not existant: false
	if(Equal(num, -1)) return(nil);
	
	// else return info
	return( EffectVar(iNum,pObj,num) );
}

// get the effect number for the object
private func InfoNumber(object pObj) {
	if(!pObj) return(nil);
	
	// search all similar effects
	var num;
	for(var i=nil; num = GetEffect(InfoName(),pObj,i); S42MigrationStep(i, 1)) {
		// and look if the target is this...
		var target = GetEffect(InfoName(),pObj,i,4);
		// found it
		if(Equal(target, this())) return(num);
	}
	// couldnt find it
	return(-1);
}
