/*--- ATOM DRUCKWELLE ---*/

#strict 3

protected func Initialize(...) {
	local alpha;
	local alpha_faktor;
	alpha=20;
	
	return(1);
}
	



func set_werte(...) {
	SetVar(nil,Par(nil));
	if((Var(nil) ?? 0)>200) SetVar(nil,200);
	DoCon(Var(nil));
	alpha_faktor=Par(1);
	if(Equal(Par(2), nil)) RemoveObject();
	if(Equal(Par(2), 1)) SetAction("GOr");
	if(Equal(Par(2), 2)) SetAction("GOl");

	return(1);
}

private func fade_out(...) {

	alpha=(alpha ?? 0)+(alpha_faktor ?? 0);
	SetVar(nil,alpha);
	SetClrModulation(RGBa(255,255,255,Var(nil)));
	if(GreaterThan(Var(nil),255))
	{
		RemoveObject();
	}
	return(1);
}


func nicht_markieren(...) { return(1);
}