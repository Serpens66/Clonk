/*-- Lichtquellen Include --*/

#strict

func Initialize() 
{
	var h=-60;
	var a=0;

	SetClrModulation(RGBa(h,h,h,a));
	SetCategory(8388609);
	SetObjectBlitMode(1);
	
	return(_inherited());
}

public ist_lichtquelle:	return(1);

