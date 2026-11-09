/*--- ATOM DRUCKWELLE ---*/

#strict 3

protected func Initialize(...) {
	local count;
	count=200;
	SetAction("GO");

	local rechts_oder_links;
	local old;
	local geschw;
	local alpha;
	local fof_faktor;
	alpha=nil;
	
	return(1);
}
	



protected func CheckHindernis(...) {




	S42MigrationStep(count, -1);
	schleuder();
	SetYDir(2000);
	ObjectCall(CreateObject(ABDS,nil,45,-1),"set_werte",GetCon(),4,rechts_oder_links);
	if(GreaterThan(GetPhysical("Walk"),200)) 
	{	if(LessThan(count,200)) CreateObject(ABED,-50,-35,-1);
		if(LessThan(count,200)) CreateObject(ABED,50,-35,-1);
		if(LessThan(count,200)) CreateObject(ABED,nil,-35,-1);
	}

	geschw=1000*(GetPhysical("Walk") ?? 0)/500;
	if(LessThan(count,200))  if(Equal(GetX(),old)) 	free();
	if(LessThan(GetCon(),200)) SetVar(2,(Div(GetCon(),2) ?? 0)+1);
	DoCon(Var(2));

	set_dir();

	SetPhysical("Walk",(GetPhysical("Walk") ?? 0)-(Div(GetPhysical("Walk"),4) ?? 0)+1,2);
	fade_out();
	if(LessThan(GetPhysical("Walk"),12)) RemoveObject();

	old=GetX();
	
	return(1);
}


func schleuder(...) {

	while (SetVar(nil, FindObject(nil,-60,-(80*(GetCon() ?? 0)/100),120,110*(GetCon() ?? 0)/100,nil, nil,nil, NoContainer(), Var(nil)) ))
	{
	       if(!Equal(ObjectCall(Var(nil),"nicht_markieren"), 1))
	       {	
		if(Equal(GetXDir(Var(nil)), nil))
		if(Equal(GetYDir(Var(nil)), nil))
		if(!Equal(ObjectCall(Var(nil),"nicht_markieren"), 1))
		{
			SetVar(2,(GetMass(Var(nil)) ?? 0)/27);

			if((Var(2) ?? 0)<0) SetVar(2,nil);
			//Message("x:%d|y%d:",Var(0),Var(2),Var(2));

			Fling(Var(nil),nil,-1);
			SetYDir(-60+(Var(2) ?? 0),Var(nil));
			if(Equal(rechts_oder_links,1)) SetXDir(60-(Var(2) ?? 0),Var(nil));
			if(Equal(rechts_oder_links,2)) SetXDir(-60+(Var(2) ?? 0),Var(nil));
			if(Equal(rechts_oder_links,1)) SetRDir(10,Var(nil));
			if(Equal(rechts_oder_links,2)) SetRDir(-10,Var(nil));
			DoDamage(200,Var(nil));
			Punch(Var(nil),30);
		}
	       }


	}
	
	return(1);
}
	

func nach_rechts(...) {
	rechts_oder_links=1;
	set_dir();
	return(1);
}

func nach_links(...) {
	rechts_oder_links=2;
	set_dir();
	return(1);
}

	


protected func set_dir(...) {
	if(Equal(rechts_oder_links,1)) SetXDir(geschw);
	if(Equal(rechts_oder_links,2)) SetXDir(-(geschw ?? 0));
	return(1);
}

private func fade_out_fast(...) {
	
	alpha=(alpha ?? 0)+(fof_faktor ?? 0);
	SetVar(nil,alpha);
	SetClrModulation(RGBa(255,255,255,Var(nil)));
	if(GreaterThan(Var(nil),255))
	{
		RemoveObject();
	}
	return(1);
}

private func fade_out(...) {
	
	S42MigrationStep(alpha, 1);
	SetVar(nil,(alpha ?? 0)*2);
	SetClrModulation(RGBa(255,255,255,Var(nil)));
	if(Equal(Var(nil),255))
	{
		RemoveObject();
	}
	return(1);
}



//by Tushjd
private func free(...) {
	DigFree(GetX(),(GetY() ?? 0)-(25*(GetCon() ?? 0)/100),10*(GetCon() ?? 0)/100);
	DigFree(GetX(),(GetY() ?? 0)-(50*(GetCon() ?? 0)/100),20*(GetCon() ?? 0)/100);
	DigFree(GetX(),(GetY() ?? 0)-(75*(GetCon() ?? 0)/100),10*(GetCon() ?? 0)/100);
	FreeRect((GetX() ?? 0)-20,(GetY() ?? 0)-(105*(GetCon() ?? 0)/100),40,99*(GetCon() ?? 0)/100);
	if(Equal(rechts_oder_links,1))
	{	
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-11,11,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-12,12,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-13,13,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-14,14,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-15,15,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-16,16,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-17,17,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-18,18,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-19,19,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-20,20,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-21,21,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-22,22,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-23,23,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-24,24,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-25,25,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-26,26,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-27,27,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-28,28,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-29,29,2);
		FreeRect((GetX() ?? 0)-5,(GetY() ?? 0)-30,30,2);
	}
	if(Equal(rechts_oder_links,2))
	{	
FreeRect((GetX() ?? 0)-6,(GetY() ?? 0)-12,11,2);
FreeRect((GetX() ?? 0)-7,(GetY() ?? 0)-14,12,2);
FreeRect((GetX() ?? 0)-8,(GetY() ?? 0)-16,13,2);
FreeRect((GetX() ?? 0)-9,(GetY() ?? 0)-18,14,2);
FreeRect((GetX() ?? 0)-10,(GetY() ?? 0)-20,15,2);
FreeRect((GetX() ?? 0)-11,(GetY() ?? 0)-22,16,2);
FreeRect((GetX() ?? 0)-12,(GetY() ?? 0)-24,17,2);
FreeRect((GetX() ?? 0)-13,(GetY() ?? 0)-26,18,2);
FreeRect((GetX() ?? 0)-14,(GetY() ?? 0)-28,19,2);
FreeRect((GetX() ?? 0)-15,(GetY() ?? 0)-30,20,2);
FreeRect((GetX() ?? 0)-16,(GetY() ?? 0)-32,21,2);
FreeRect((GetX() ?? 0)-17,(GetY() ?? 0)-34,22,2);
FreeRect((GetX() ?? 0)-18,(GetY() ?? 0)-36,23,2);
FreeRect((GetX() ?? 0)-19,(GetY() ?? 0)-38,24,2);
FreeRect((GetX() ?? 0)-20,(GetY() ?? 0)-40,25,2);
FreeRect((GetX() ?? 0)-21,(GetY() ?? 0)-42,26,2);
FreeRect((GetX() ?? 0)-22,(GetY() ?? 0)-44,27,2);
FreeRect((GetX() ?? 0)-23,(GetY() ?? 0)-46,28,2);
FreeRect((GetX() ?? 0)-24,(GetY() ?? 0)-48,29,2);
FreeRect((GetX() ?? 0)-25,(GetY() ?? 0)-50,30,2);
	}	

	return(1);
}


func nicht_markieren(...) { return(1);
}