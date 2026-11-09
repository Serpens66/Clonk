/*-- Neues Objekt --*/

#strict 3

protected func Initialize(...) {
	
	local random_animation;
	local links_rechts;
	local ruttel_wirkung;
	local nach_rechtsDIR;
	local counter_x;
	local counter_y;
	local alpha;
	alpha=200;
	ruttel_wirkung=2;
	links_rechts=1;
	counter_x=125;
	counter_y=230;



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
	
	if(Equal(nach_rechtsDIR, 1))
	{
		S42MigrationStep(counter_y, -1);
		if((counter_y ?? 0) > 40) SetPosition(GetX(),(GetY() ?? 0)-1);
		if((counter_y ?? 0) < 40) SetPosition(GetX(),(GetY() ?? 0)+1);
		if((counter_y ?? 0) < -20)	counter_y=100;
		if((counter_y ?? 0) <100)
		{
			S42MigrationStep(counter_x, -1);
			if((counter_x ?? 0) > 40) SetPosition((GetX() ?? 0)-2,GetY());
			if((counter_x ?? 0) < 40) SetPosition((GetX() ?? 0)+2,GetY());
			if((counter_x ?? 0) < -20)	counter_x=100;
		}
	}
	
	if(Equal(nach_rechtsDIR, 2))
	{
		S42MigrationStep(counter_y, -1);
		if((counter_y ?? 0) > 40) SetPosition(GetX(),(GetY() ?? 0)-1);
		if((counter_y ?? 0) < 40) SetPosition(GetX(),(GetY() ?? 0)+1);
		if((counter_y ?? 0) < -20)	counter_y=100;
		if((counter_y ?? 0) <100)
		{
			S42MigrationStep(counter_x, -1);
			if((counter_x ?? 0) > 40) SetPosition((GetX() ?? 0)+2,GetY());
			if((counter_x ?? 0) < 40) SetPosition((GetX() ?? 0)-2,GetY());
			if((counter_x ?? 0) < -20)	counter_x=100;
		}
	}
	if(Equal(nach_rechtsDIR, nil))
	{
		S42MigrationStep(counter_y, -1);
		if((counter_y ?? 0) > 0) SetPosition(GetX(),(GetY() ?? 0)-1);
		if((counter_y ?? 0) < 0) SetPosition(GetX(),(GetY() ?? 0)+1);
		if((counter_y ?? 0) < -20)	counter_y=20;
	
	}
	
	
	

	fade_out();
	if(LessThan(GetCon(),400)) SetVar(2,(Div(GetCon(),1000) ?? 0)+3);
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

public func start(...) {
	ObjectCall(CreateObject(ABPR),"nach_links");
	ObjectCall(CreateObject(ABPR),"static");
	nach_rechtsDIR=2;
	return(1);
}
public func nach_links(...) {
	nach_rechtsDIR=1;
	return(1);
}
public func static(...) {
	counter_y=(counter_y ?? 0)+50;
	nach_rechtsDIR=nil;
	return(1);
}

private func fade_out(...) {
	
	S42MigrationStep(alpha, 1);
	S42MigrationStep(alpha, 1);
	S42MigrationStep(alpha, 1);
	S42MigrationStep(alpha, 1);
	S42MigrationStep(alpha, 1);
	S42MigrationStep(alpha, 1);
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
