/*-- Neues Objekt --*/

#strict 3

	local random_animation;
	local links_rechts;
	local ruttel_wirkung;
	local nach_rechtsDIR;
	local counter;
	local alpha;

protected func Initialize(...) {

	alpha=nil;
	ruttel_wirkung=2;
	links_rechts=1;
	counter=200;



	random_animation=(Random(15) ?? 0)+1;
	if(Equal(random_animation,1)) SetAction("1");
	if(Equal(random_animation,2)) SetAction("2");
	if(Equal(random_animation,3)) SetAction("3");
	if(Equal(random_animation,4)) SetAction("4");
	if(Equal(random_animation,5)) SetAction("5");
	if(Equal(random_animation,6)) SetAction("6");
	if(Equal(random_animation,7)) SetAction("7");
	if(Equal(random_animation,8)) SetAction("8");
	if(Equal(random_animation,9)) SetAction("9");
	if(Equal(random_animation,10)) SetAction("10");
	if(Equal(random_animation,11)) SetAction("11");
	if(Equal(random_animation,12)) SetAction("12");
	if(Equal(random_animation,13)) SetAction("13");
	if(Equal(random_animation,14)) SetAction("14");
	if(Equal(random_animation,15)) SetAction("15");
  return(1);
}

protected func fade_out_rauch(...) {
	S42MigrationStep(counter, -1);
	if(GreaterThan(counter,nil))
	{
		if(Equal(nach_rechtsDIR,2)) SetPosition((GetX() ?? 0)+(Random(2) ?? 0),GetY());
		if(Equal(nach_rechtsDIR,1)) SetPosition((GetX() ?? 0)-(Random(2) ?? 0),GetY());
		if(Equal(nach_rechtsDIR,nil)) SetPosition(GetX(),GetY());
	}

	fade_out();
	if(LessThan(GetCon(),250)) SetVar(2,(Div(GetCon(),1000) ?? 0)+1);
	DoCon(Var(2));
	if(Equal(links_rechts,1))
	{
		SetPosition((GetX() ?? 0)-(Random(ruttel_wirkung) ?? 0),GetY());
		links_rechts=2;
		return(1);
	}
	if(Equal(links_rechts,2))
	{
		SetPosition((GetX() ?? 0)+(Random(ruttel_wirkung) ?? 0),GetY());
		links_rechts=1;
		return(1);
	}
}

public func nach_rechts(...) {
	nach_rechtsDIR=2;
	return(1);
}
public func nach_links(...) {
	nach_rechtsDIR=1;
	return(1);
}
public func be_static(...) {
	nach_rechtsDIR=nil;
	return(1);
}

private func fade_out(...) {
	
	S42MigrationStep(alpha, 1);
	SetVar(nil,(alpha ?? 0)/20);
	SetClrModulation(RGBa(255,255,255,Var(nil)));
	if((Var(nil) ?? 0) > 225)
	{
		RemoveObject();
	}
	return(1);
}
func nicht_markieren(...) { return(1);
}