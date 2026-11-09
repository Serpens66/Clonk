/*-- Keycard-Terminal --*/

#strict 3
#include SENS

local cardid;

func Initialize()
{
  _inherited();
  AddState("$VALID$");

  Set(Random(255));
}

/* Bedienung */

protected func ControlThrow(object pClonk)
{
  [$Control$]

  pUser = pClonk;

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
    cardloaded(kc);
    ScheduleCall(nil,"CheckCard",40);
    Sound("load_card");
  }
  return(1);
}

func cardloaded(kc)//Grafischer Effekt
{
  SetGraphics ("CARD",this(),GetID(this()),3,GFXOV_MODE_Base,nil);
  SetClrModulation (GetClrModulation(kc),this(),3);
}

func nocard()
{
  SetGraphics(nil,nil,nil,3);//Overlay löschen
}

public func CheckCard() {
  if(!Contents()) return;

  if(Equal(Contents()->GetKeyNumber(), cardid)) {
    // es ist die richtige!
    SetState(state,nil,1);//ok

    // Effekt
    Sound("Bip");
    CreateParticle("PSpark", -5, 1, nil, nil, 50, RGBa(nil, 255, nil));
  }
  else {
    // die falsche... :(
    Sound("Error");
    CreateParticle("PSpark", -5, 1, nil, nil, 50, RGBa(255, nil, nil));

    SetState(state,nil,nil);//no
  }
  if(Contents())   ScheduleCall(nil,"eject",10,nil,Contents());
  TriggerActions();
}

public func CreateKeyCard() {
  var kc = CreateObject(KCRD,nil,nil,GetOwner());
  kc -> SetKeyNumber(cardid);
  eject(kc);
  return(kc);
}

public func Set(int iNumber)
{
  iNumber ??= 0;
  cardid = iNumber;
  SetGraphics ("ID",this(),GetID(this()),1,GFXOV_MODE_Base,nil);
  SetGraphics ("ID",this(),GetID(this()),2,GFXOV_MODE_Picture,nil);
  SetClrModulation (HSL (BoundBy(cardid,nil,255),255,127),this(),1);
  SetClrModulation (HSL (BoundBy(cardid,nil,255),255,127),this(),2);
}

public func GetAdditionalInfo()
{
	return(pUser);
}

func ExtraSetup()
{
  if(test_overwrite())
    AddMenuItem ("$OVERWRITE$","overwrite",GetID(),pUser,nil,nil,"$OVERWRITE_DESC$");

  if(test_setcode())
    AddMenuItem ("$SETCODE$","setcode",GetID(),pUser,nil,nil,"$SETCODE_DESC$");
  return(1);
}

func test_overwrite()
{
  var kc = Contents(nil,pUser);

  if(!kc)
    return(nil);

  if(!(kc->~IsKeycard()))
    return(nil);

  if(!Equal(kc->~GetKeyNumber(), cardid))
    return(1);
}

func test_setcode()
{
  return(test_overwrite());
}

func setcode()
{
  var kc = Contents(nil,pUser);
  // Sonst Nullpointer -_-
  if(!kc) {
    Sound("CommandFailure1");
    return(nil);
  }

  if(!(kc->~IsKeycard())) {
    Sound("CommandFailure1");
    return(nil);
  }
  else {
    // erstmal annehmen
    kc->Exit();
    kc->Enter(this());
    cardloaded(kc);
    ScheduleCall(nil,"setcode2",40,nil,kc);
    Sound("load_card");
  }
}

func setcode2(pKeycard)
{
  if(!pKeycard) return(nil);
  Sound("Bip");
  cardid = pKeycard->GetKeyNumber();
  ScheduleCall(nil,"eject",10,nil,pKeycard);
}

func overwrite()
{
  var kc = Contents(nil,pUser);
  // Sonst Nullpointer -_-
  if(!kc) {
    Sound("CommandFailure1");
    return(nil);
  }

  if(!(kc->~IsKeycard())) {
    Sound("CommandFailure1");
    return(nil);
  }
  else {
    // erstmal annehmen
    kc->Exit();
    kc->Enter(this());
    cardloaded(kc);
    ScheduleCall(nil,"overwrite2",40,nil,kc);
    Sound("load_card");
  }
}

func overwrite2(pKeycard)
{
  if(!pKeycard) return(nil);
  Sound("Bip");
  pKeycard->SetKeyNumber(cardid);
  ScheduleCall(nil,"eject",10,nil,pKeycard);
}

func eject(pKeycard)
{
  pKeycard->Exit();
  nocard();
  Sound("eject_card");
}