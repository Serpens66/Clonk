/*--- ATOM DRUCKWELLE ---*/

#strict

protected Initialize:
	local alpha;
	local alpha_faktor;
	alpha=20;
	
	return(1);
	



set_werte:
	SetVar(0,Par(0));
	if(Var(0)>200) SetVar(0,200);
	DoCon(Var(0));
	alpha_faktor=Par(1);
	if(Par(2) == 0) RemoveObject();
	if(Par(2) == 1) SetAction("GOr");
	if(Par(2) == 2) SetAction("GOl");

	return(1);

private fade_out:

	alpha=alpha+alpha_faktor;
	SetVar(0,alpha);
	SetClrModulation(RGBa(255,255,255,Var(0)));
	if(GreaterThan(Var(0),255))
	{
		RemoveObject();
	}
	return(1);


nicht_markieren: return(1);