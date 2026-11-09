/*-- Chassis --*/

#strict 3

public func AttachTo(attachto) {	// attach to tank
	SetDir(DIR_Left);
	SetAction("Attach",attachto);
}

public func Remove() {			// remove
	if(!!SEqual(GetAction(), "Idle"))
		RemoveObject();
}

public func Turn() {
	SetAction("Turn",GetActionTarget());
}

protected func ChangeDir() {
	var a = GetAction(GetActionTarget());
	if(Or(!!SEqual(a, "Turn"), !!SEqual(a, "TurnStalled"))) return(SetAction("Turn"));

	SetAction("Attach",GetActionTarget());
	if(Equal(GetDir(), DIR_Left)) SetDir(DIR_Right);
	else  SetDir(DIR_Left);
}

/* Engine effect */

protected func Engine() {
	SetR(GetR(GetActionTarget()));

	// set matrix values
	if(And(GetActionTarget()->Contents(), !(GetActionTarget()->EMPShocked())))
		SetObjDrawTransform (1000, nil, nil, nil, 1000, Sin((GetActTime() ?? 0)*50,300));
}
