/*-- Neues Objekt --*/

#strict

protected Initialize:
	local counter;
	local initilized;
	local wait;	
	wait=10;
	local raucherzeugung;
	raucherzeugung=10;
	if(initilized != 1)
	{
		counter=50;
		SetAction("Pre");		
		local links_rechts;
		local ruttel_wirkung;
		DoCon(400);
		local alpha;
		local material_gegeben;
		material_gegeben=GetMaterial(0,70);
		SetClrModulation(RGBa(255,255,255,40));
		
		alpha=0;
		ruttel_wirkung=2;
		links_rechts=1;
	}
	initilized=1;
	return(1);

protected start:
	--counter;	
	if(And(counter<20,counter>10))
	{
		CastPXS(MaterialName(material_gegeben),100,100,-50,-120);
		CastPXS(MaterialName(material_gegeben),100,100,50,-120);
	}
	fade_out();
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
	 
	return(1);
private wait1:
	--wait;
	if(wait<0) SetAction("WACHSE");
	return(1);
	

private fade_out:
	--raucherzeugung;
	if(raucherzeugung <0)
	{
		ObjectCall(CreateObject(ABPR,Random(40)-17,60),"start");
		raucherzeugung=2;
	}	
	if(GetAction() S= "Off") if(Random(1) == 1) SetPhase(1);
	
	++alpha;
	SetVar(0,alpha/22*10);
	SetClrModulation(RGBa(255,255,255,Var(0)));
	if(Var(0)>100)
	{
		RemoveObject();
	}
	if(alpha>500) DoCon((GetCon()/1000)-1-Random(Random(4)));
	return(1);

nicht_markieren: return(1);
