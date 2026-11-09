/*-- Neues Objekt --*/

#strict 3

protected func Initialize(...) {
	local counter;
	local initilized;
	local wait;	
	wait=10;
	local raucherzeugung;
	raucherzeugung=10;
	if(!Equal(initilized, 1))
	{
		counter=50;
		SetAction("Pre");		
		local links_rechts;
		local ruttel_wirkung;
		DoCon(400);
		local alpha;
		local material_gegeben;
		material_gegeben=GetMaterial(nil,70);
		SetClrModulation(RGBa(255,255,255,40));
		
		alpha=nil;
		ruttel_wirkung=2;
		links_rechts=1;
	}
	initilized=1;
	return(1);
}

protected func start(...) {
	S42MigrationStep(counter, -1);
	if(And((counter ?? 0)<20,(counter ?? 0)>10))
	{
		CastPXS(MaterialName(material_gegeben),100,100,-50,-120);
		CastPXS(MaterialName(material_gegeben),100,100,50,-120);
	}
	fade_out();
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
	 
	return(1);
}
private func wait1(...) {
	S42MigrationStep(wait, -1);
	if((wait ?? 0)<0) SetAction("WACHSE");
	return(1);
}
	

private func fade_out(...) {
	S42MigrationStep(raucherzeugung, -1);
	if((raucherzeugung ?? 0) <0)
	{
		ObjectCall(CreateObject(ABPR,(Random(40) ?? 0)-17,60),"start");
		raucherzeugung=2;
	}	
	if(!!SEqual(GetAction(), "Off")) if(Equal(Random(1), 1)) SetPhase(1);
	
	S42MigrationStep(alpha, 1);
	SetVar(nil,(alpha ?? 0)/22*10);
	SetClrModulation(RGBa(255,255,255,Var(nil)));
	if((Var(nil) ?? 0)>100)
	{
		RemoveObject();
	}
	if((alpha ?? 0)>500) DoCon(((GetCon() ?? 0)/1000)-1-(Random(Random(4)) ?? 0));
	return(1);
}

func nicht_markieren(...) { return(1);
}
