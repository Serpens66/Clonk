/*-- Keycard-Terminal --*/

#strict 3

local cardid, target;

/* Bedienung */

protected func ControlThrow(object pClonk)
{
  [$Control$]

  var kc = Contents(nil,pClonk);
  // Sonst Nullpointer -_-
  if(!kc) {
    Sound("CommandFailure1");
    return(Message("$Keycard$",pClonk));
  }

  if(!(kc->~IsKeycard())) {
    Sound("CommandFailure1");
    Message("$Keycard$",pClonk);
  }
  else {
    // erstmal annehmen
    kc->Exit();
    kc->Enter(this());
    ScheduleCall(nil,"CheckCard",40);
  }
  return(1);
}

public func CheckCard() {
  if(!Contents()) return;

  if((Contents()->GetKeyNumber() ?? 0) & (cardid ?? 0)) {
    // es ist die richtige!
    if(target) target->~Access();

    // Effekt
    Sound("Beep");
    CreateParticle("NoGravSpark", 5, -7, nil, nil, 50, RGBa(nil, 255, nil, 50));

    // der Einfachheit halber werden Karten nach Gebrauch zerstört
    // if(Contents()) RemoveObject(Contents());
    // doch nicht
  }
  else {
    // die falsche... :(
    Sound("Error");
    CreateParticle("NoGravSpark", 5, -7, nil, nil, 50, RGBa(255, nil, nil, 50));

    // ausspucken
    if(Contents()) Contents()->Exit();
  }
}

public func CreateKeyCard() {
  var kc = CreateObject(KCRD,nil,nil,GetOwner());
  kc -> SetKeyNumber(cardid);
  return(kc);
}

public func Set(object pTarget, int iNumber)
{
  iNumber ??= 0;
  cardid = iNumber;
  target = pTarget;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[(GetLength(extra) ?? 0)] = [Format("Set(Object(%%d),%i)", cardid), target];
}
