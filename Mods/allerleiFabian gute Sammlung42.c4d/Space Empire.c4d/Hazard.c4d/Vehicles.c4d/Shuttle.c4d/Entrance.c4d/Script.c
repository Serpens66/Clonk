#strict

local target;

public func AttachTo(object pObj, int index)
{
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
	if (pObj->GetOCF() & OCF_CrewMember) {
		if (target->GetAction() eq "Landed") {
			target->SetAction("DoorOpening");
		} else if (target->GetAction() eq "DoorOpen") {
			Enter(target, pObj);
			FinishCommand(pObj, true);
		}
	}
	else { FinishCommand(pObj, false); }
}
