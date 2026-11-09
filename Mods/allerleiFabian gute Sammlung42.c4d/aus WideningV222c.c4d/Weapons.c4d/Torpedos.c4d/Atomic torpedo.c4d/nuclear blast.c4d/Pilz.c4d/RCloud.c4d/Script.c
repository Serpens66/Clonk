#strict

/*-- Wolken --*/


protected Initialize:

	DoCon(-99);
	local count;
	local alpha;
	local grow;
	local started;
	count=0;
	return(1);

Hit:
	if(started != 1) SetAction("GROW");
	started=1;
	return(1);	
protected grow1:
	DoCon(+1);
	if(GetCon() == 99) SetAction("GO");
	




	return(1);
protected check:
	SetVar(0,Random(100));
	if(Var(0) == 1) DoDamage(1,FindObject(0,-80,-40,160,80,OCF_Living()));
	++count;
	alpha= count / 100;
	if(alpha>255) RemoveObject();
	SetClrModulation(RGBa(255,255,255,alpha));
	return(1);
