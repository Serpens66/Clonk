#strict 3

local revoluzzer, zeit, revzeit, regierungsform;

public func GetRevTimeL(spieler)
{
	return(Prozent(10+(GetScore(revoluzzer) ?? 0)/10, GetRevTime()));
}

public func StarteRevolution(spieler,reg)
{
	SetOwner(revoluzzer=spieler);
	SetVisibility((VIS_Owner ?? 0) | (VIS_Allies ?? 0));
	SetAction("IgnoreDay",GetCursor(revoluzzer));
	SetGovernment(revoluzzer,REVO);
	zeit = revzeit	= GetRevTimeL(revoluzzer);
	regierungsform 	= reg;
	return(1);
}

protected func RevCheck(...) {
{
	if(ActIdle())
		SetAction("IgnoreDay",GetCursor(revoluzzer));
	if(!Equal(GetActionTarget(), GetCursor(revoluzzer)))
		SetAction("IgnoreDay",GetCursor(revoluzzer));
	if(S42MigrationStep(zeit, -1, true)) return(1);
	SetGovernment(revoluzzer, regierungsform);
	return(RemoveObject());
}
}