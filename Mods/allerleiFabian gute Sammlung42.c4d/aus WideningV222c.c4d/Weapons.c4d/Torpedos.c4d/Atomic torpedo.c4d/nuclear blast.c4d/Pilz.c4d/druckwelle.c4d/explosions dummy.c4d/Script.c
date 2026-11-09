/*-- Explosions Dummy  --*/

#strict 3

protected func Initialize(...) {
	local kiddie_modus;
	kiddie_modus=nil;
	local count;
	count=3;
	SetAction("LOOP");
  return(1);
}
	

protected func countdown(...) {
	S42MigrationStep(count, -1);
	if(Equal(kiddie_modus, nil))
	{
		if(LessThan(count,nil))  Explode(60);
	}
	if(Equal(kiddie_modus, 1))
	{
		if(LessThan(count,nil))  Message("B",this());
		if(LessThan(count,-100))  RemoveObject();
	}

	return(1);
}
func nicht_markieren(...) { return(1);
}
