#strict 3

/*-- Wolken --*/


protected func Initialize(...) {

	DoCon(-99);
	local count;
	local alpha;
	local grow;
	local started;
	count=nil;
	return(1);
}

func Hit(...) {
	if(!Equal(started, 1)) SetAction("GROW");
	started=1;
	return(1);	
}
protected func grow1(...) {
	DoCon(+1);
	if(Equal(GetCon(), 99)) SetAction("GO");
	




	return(1);
}
protected func check(...) {
	SetVar(nil,Random(100));
	if(Equal(Var(nil), 1)) DoDamage(1,FindObject(nil,-80,-40,160,80,OCF_Living));
	S42MigrationStep(count, 1);
	alpha= (count ?? 0) / 100;
	if((alpha ?? 0)>255) RemoveObject();
	SetClrModulation(RGBa(255,255,255,alpha));
	return(1);
}
