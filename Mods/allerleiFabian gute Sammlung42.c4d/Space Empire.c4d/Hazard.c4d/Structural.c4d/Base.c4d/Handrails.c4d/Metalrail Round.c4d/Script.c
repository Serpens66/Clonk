/*-- Rounded Metalrail --*/

#strict 3

#include RAI1

/* Status */

public func Index(int index) {
	index ??= 0;
	if(Equal(index, nil)) return("End1");
	if(Equal(index, 1)) return("Linkage");
	if(Equal(index, 2)) return("End2");
}
	
public func Left()		{ return(nil); }
public func Right()	{ return(2); }
public func Linkage()	{ return(1); }
public func Backing()	{ return(-1); }	// no such element