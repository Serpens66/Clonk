/*-- Lichtquellen Include --*/

#strict 3

func Initialize() 
{
	var h=-60;
	var a=nil;

	SetClrModulation(RGBa(h,h,h,a));
	SetCategory(8388609);
	SetObjectBlitMode(1);
	
	return(_inherited());
}

public func ist_lichtquelle(...) {	return(1);
}

