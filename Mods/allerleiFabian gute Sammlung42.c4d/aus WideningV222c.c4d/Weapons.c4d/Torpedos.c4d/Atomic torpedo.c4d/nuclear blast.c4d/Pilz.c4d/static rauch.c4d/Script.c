/*-- Neues Objekt --*/

#strict

	local random_animation;
	local links_rechts;
	local ruttel_wirkung;
	local nach_rechtsDIR;
	local counter;
	local alpha;

protected Initialize:

	alpha=0;
	ruttel_wirkung=2;
	links_rechts=1;
	counter=200;



	random_animation=Random(15)+1;
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

protected fade_out_rauch: 
	--counter;
	if(GreaterThan(counter,0))
	{
		if(Equal(nach_rechtsDIR,2)) SetPosition(GetX()+Random(2),GetY());
		if(Equal(nach_rechtsDIR,1)) SetPosition(GetX()-Random(2),GetY());
		if(Equal(nach_rechtsDIR,0)) SetPosition(GetX(),GetY());
	}

	fade_out();
	if(LessThan(GetCon(),250)) SetVar(2,Div(GetCon(),1000)+1);
	DoCon(Var(2));
	if(Equal(links_rechts,1))
	{
		SetPosition(GetX()-Random(ruttel_wirkung),GetY());
		links_rechts=2;
		return(1);
	}
	if(Equal(links_rechts,2))
	{
		SetPosition(GetX()+Random(ruttel_wirkung),GetY());
		links_rechts=1;
		return(1);
	}

public nach_rechts:
	nach_rechtsDIR=2;
	return(1);
public nach_links:
	nach_rechtsDIR=1;
	return(1);
public be_static:
	nach_rechtsDIR=0;
	return(1);

private fade_out:
	
	++alpha;
	SetVar(0,alpha/20);
	SetClrModulation(RGBa(255,255,255,Var(0)));
	if(Var(0) > 225)
	{
		RemoveObject();
	}
	return(1);
nicht_markieren: return(1);