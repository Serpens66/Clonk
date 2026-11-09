/*--- ATOM DRUCKWELLE ---*/

#strict

protected Initialize:
	local count;
	count=200;
	SetAction("GO");

	local rechts_oder_links;
	local old;
	local geschw;
	local alpha;
	local fof_faktor;
	alpha=0;
	
	return(1);
	



protected CheckHindernis:




	--count;	
	schleuder();
	SetYDir(2000);
	ObjectCall(CreateObject(ABDS,0,45,-1),"set_werte",GetCon(),4,rechts_oder_links);
	if(GreaterThan(GetPhysical("Walk"),200)) 
	{	if(LessThan(count,200)) CreateObject(ABED,-50,-35,-1);
		if(LessThan(count,200)) CreateObject(ABED,50,-35,-1);
		if(LessThan(count,200)) CreateObject(ABED,0,-35,-1);
	}

	geschw=1000*GetPhysical("Walk")/500;
	if(LessThan(count,200))  if(Equal(GetX(),old)) 	free();
	if(LessThan(GetCon(),200)) SetVar(2,Div(GetCon(),2)+1);
	DoCon(Var(2));

	set_dir();

	SetPhysical("Walk",GetPhysical("Walk")-Div(GetPhysical("Walk"),4)+1,2);
	fade_out();
	if(LessThan(GetPhysical("Walk"),12)) RemoveObject();

	old=GetX();
	
	return(1);


schleuder:

	while (SetVar(0, FindObject(0,-60,-(80*GetCon()/100),120,110*GetCon()/100,0, 0,0, NoContainer(), Var(0)) ))
	{
	       if(ObjectCall(Var(0),"nicht_markieren") != 1)
	       {	
		if(GetXDir(Var(0)) == 0)
		if(GetYDir(Var(0)) == 0)
		if(ObjectCall(Var(0),"nicht_markieren") != 1)
		{
			SetVar(2,GetMass(Var(0))/27);

			if(Var(2)<0) SetVar(2,0);
			//Message("x:%d|y%d:",Var(0),Var(2),Var(2));

			Fling(Var(0),0,-1);
			SetYDir(-60+Var(2),Var(0));
			if(Equal(rechts_oder_links,1)) SetXDir(60-Var(2),Var(0));
			if(Equal(rechts_oder_links,2)) SetXDir(-60+Var(2),Var(0));
			if(Equal(rechts_oder_links,1)) SetRDir(10,Var(0));
			if(Equal(rechts_oder_links,2)) SetRDir(-10,Var(0));
			DoDamage(200,Var(0));
			Punch(Var(0),30);
		}
	       }


	}
	
	return(1);
	

nach_rechts:
	rechts_oder_links=1;
	set_dir();
	return(1);

nach_links:
	rechts_oder_links=2;
	set_dir();
	return(1);

	


protected set_dir:
	if(Equal(rechts_oder_links,1)) SetXDir(geschw);
	if(Equal(rechts_oder_links,2)) SetXDir(-geschw);
	return(1);

private fade_out_fast:
	
	alpha=alpha+fof_faktor;
	SetVar(0,alpha);
	SetClrModulation(RGBa(255,255,255,Var(0)));
	if(GreaterThan(Var(0),255))
	{
		RemoveObject();
	}
	return(1);

private fade_out:
	
	++alpha;
	SetVar(0,alpha*2);
	SetClrModulation(RGBa(255,255,255,Var(0)));
	if(Equal(Var(0),255))
	{
		RemoveObject();
	}
	return(1);



//by Tushjd
private free:
	DigFree(GetX(),GetY()-(25*GetCon()/100),10*GetCon()/100);
	DigFree(GetX(),GetY()-(50*GetCon()/100),20*GetCon()/100);
	DigFree(GetX(),GetY()-(75*GetCon()/100),10*GetCon()/100);
	FreeRect(GetX()-20,GetY()-(105*GetCon()/100),40,99*GetCon()/100);
	if(Equal(rechts_oder_links,1))
	{	
		FreeRect(GetX()-5,GetY()-11,11,2); 
		FreeRect(GetX()-5,GetY()-12,12,2); 
		FreeRect(GetX()-5,GetY()-13,13,2); 
		FreeRect(GetX()-5,GetY()-14,14,2); 
		FreeRect(GetX()-5,GetY()-15,15,2); 
		FreeRect(GetX()-5,GetY()-16,16,2); 
		FreeRect(GetX()-5,GetY()-17,17,2); 
		FreeRect(GetX()-5,GetY()-18,18,2); 
		FreeRect(GetX()-5,GetY()-19,19,2); 
		FreeRect(GetX()-5,GetY()-20,20,2); 
		FreeRect(GetX()-5,GetY()-21,21,2); 
		FreeRect(GetX()-5,GetY()-22,22,2); 
		FreeRect(GetX()-5,GetY()-23,23,2); 
		FreeRect(GetX()-5,GetY()-24,24,2); 
		FreeRect(GetX()-5,GetY()-25,25,2); 
		FreeRect(GetX()-5,GetY()-26,26,2); 
		FreeRect(GetX()-5,GetY()-27,27,2); 
		FreeRect(GetX()-5,GetY()-28,28,2); 
		FreeRect(GetX()-5,GetY()-29,29,2); 
		FreeRect(GetX()-5,GetY()-30,30,2); 
	}
	if(Equal(rechts_oder_links,2))
	{	
FreeRect(GetX()-6,GetY()-12,11,2); 
FreeRect(GetX()-7,GetY()-14,12,2); 
FreeRect(GetX()-8,GetY()-16,13,2); 
FreeRect(GetX()-9,GetY()-18,14,2); 
FreeRect(GetX()-10,GetY()-20,15,2); 
FreeRect(GetX()-11,GetY()-22,16,2); 
FreeRect(GetX()-12,GetY()-24,17,2); 
FreeRect(GetX()-13,GetY()-26,18,2); 
FreeRect(GetX()-14,GetY()-28,19,2); 
FreeRect(GetX()-15,GetY()-30,20,2); 
FreeRect(GetX()-16,GetY()-32,21,2); 
FreeRect(GetX()-17,GetY()-34,22,2); 
FreeRect(GetX()-18,GetY()-36,23,2); 
FreeRect(GetX()-19,GetY()-38,24,2); 
FreeRect(GetX()-20,GetY()-40,25,2); 
FreeRect(GetX()-21,GetY()-42,26,2); 
FreeRect(GetX()-22,GetY()-44,27,2); 
FreeRect(GetX()-23,GetY()-46,28,2); 
FreeRect(GetX()-24,GetY()-48,29,2); 
FreeRect(GetX()-25,GetY()-50,30,2); 
	}	

	return(1);


nicht_markieren: return(1);