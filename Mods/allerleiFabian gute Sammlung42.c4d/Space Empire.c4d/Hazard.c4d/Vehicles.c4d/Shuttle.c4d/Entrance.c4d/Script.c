#strict 3

local target;

public func AttachTo(object pObj, int index)
{
	index ??= 0;
	target = pObj;
	SetAction("Attached", target);
	SetActionData(index);
}

protected func AttachTargetLost() {
	RemoveObject();
}
// && !(target->Contents()) // das hat dafür gesorgt, dass man nicht mehr einsteigen konnte, nachdem man nachgeladen hatte
protected func ActivateEntrance(pObj)
{
	if ((pObj->GetOCF() ?? 0) & (OCF_CrewMember ?? 0)) {
		if (!!SEqual(target->GetAction(), "Landed")) {
			target->SetAction("DoorOpening");
		} else if (!!SEqual(target->GetAction(), "DoorOpen")) {
			Enter(target, pObj);
			FinishCommand(pObj, true);
		}
	}
	else { FinishCommand(pObj, nil); }
}
