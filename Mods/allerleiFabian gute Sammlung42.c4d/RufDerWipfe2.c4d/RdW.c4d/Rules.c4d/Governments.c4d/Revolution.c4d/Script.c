#strict

local revoluzzer, zeit, revzeit, regierungsform;

public func GetRevTimeL(spieler)
{
	return(Prozent(10+GetScore(revoluzzer)/10, GetRevTime()));
}

public func StarteRevolution(spieler,reg)
{
	SetOwner(revoluzzer=spieler);
	SetVisibility(VIS_Owner | VIS_Allies);
	SetAction("IgnoreDay",GetCursor(revoluzzer));
	SetGovernment(revoluzzer,REVO);
	zeit = revzeit	= GetRevTimeL(revoluzzer);
	regierungsform 	= reg;
	return(1);
}

protected RevCheck:
{
	if(ActIdle())
		SetAction("IgnoreDay",GetCursor(revoluzzer));
	if(GetActionTarget()!=GetCursor(revoluzzer))
		SetAction("IgnoreDay",GetCursor(revoluzzer));
	if(zeit--) return(1);
	SetGovernment(revoluzzer, regierungsform);
	return(RemoveObject());
}