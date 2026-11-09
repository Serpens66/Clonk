/*-- Keycard-Terminal --*/

#strict
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

  var kc = Contents(0,pClonk);
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
    ScheduleCall(0,"CheckCard",40);
    Sound("load_card");
  }
  return(1);
}

func cardloaded(kc)//Grafischer Effekt
{
  SetGraphics ("CARD",this(),GetID(this()),3,GFXOV_MODE_Base,0); 
  SetClrModulation (GetClrModulation(kc),this(),3);
}

func nocard()
{
  SetGraphics(0,0,0,3);//Overlay löschen
}

public func CheckCard() {
  if(!Contents()) return();

  if(Contents()->GetKeyNumber() == cardid) {
    // es ist die richtige!
    SetState(state,0,1);//ok

    // Effekt
    Sound("Bip");
    CreateParticle("PSpark", -5, 1, 0, 0, 50, RGBa(0, 255, 0));
  }
  else {
    // die falsche... :(
    Sound("Error");
    CreateParticle("PSpark", -5, 1, 0, 0, 50, RGBa(255, 0, 0));

    SetState(state,0,0);//no
  }
  if(Contents())   ScheduleCall(0,"eject",10,0,Contents());
  TriggerActions();
}

public func CreateKeyCard() {
  var kc = CreateObject(KCRD,0,0,GetOwner());
  kc -> SetKeyNumber(cardid);
  eject(kc);
  return(kc);
}

public func Set(int iNumber)
{
  cardid = iNumber;
  SetGraphics ("ID",this(),GetID(this()),1,GFXOV_MODE_Base,0);
  SetGraphics ("ID",this(),GetID(this()),2,GFXOV_MODE_Picture,0);
  SetClrModulation (HSL (BoundBy(cardid,0,255),255,127),this(),1);
  SetClrModulation (HSL (BoundBy(cardid,0,255),255,127),this(),2);
}

public func GetAdditionalInfo()
{
	return(pUser);
}

func ExtraSetup()
{
  if(test_overwrite())
    AddMenuItem ("$OVERWRITE$","overwrite",GetID(),pUser,0,0,"$OVERWRITE_DESC$");

  if(test_setcode())
    AddMenuItem ("$SETCODE$","setcode",GetID(),pUser,0,0,"$SETCODE_DESC$");
  return(1);
}

func test_overwrite()
{
  var kc = Contents(0,pUser);

  if(!kc)
    return(0);

  if(!(kc->~IsKeycard()))
    return(0);

  if(kc->~GetKeyNumber() != cardid)
    return(1);
}

func test_setcode()
{
  return(test_overwrite());
}

func setcode()
{
  var kc = Contents(0,pUser);
  // Sonst Nullpointer -_-
  if(!kc) {
    Sound("CommandFailure1");
    return(0);
  }

  if(!(kc->~IsKeycard())) {
    Sound("CommandFailure1");
    return(0);
  }
  else {
    // erstmal annehmen
    kc->Exit();
    kc->Enter(this());
    cardloaded(kc);
    ScheduleCall(0,"setcode2",40,0,kc);
    Sound("load_card");
  }
}

func setcode2(pKeycard)
{
  if(!pKeycard) return(0);
  Sound("Bip");
  cardid = pKeycard->GetKeyNumber();
  ScheduleCall(0,"eject",10,0,pKeycard);
}

func overwrite()
{
  var kc = Contents(0,pUser);
  // Sonst Nullpointer -_-
  if(!kc) {
    Sound("CommandFailure1");
    return(0);
  }

  if(!(kc->~IsKeycard())) {
    Sound("CommandFailure1");
    return(0);
  }
  else {
    // erstmal annehmen
    kc->Exit();
    kc->Enter(this());
    cardloaded(kc);
    ScheduleCall(0,"overwrite2",40,0,kc);
    Sound("load_card");
  }
}

func overwrite2(pKeycard)
{
  if(!pKeycard) return(0);
  Sound("Bip");
  pKeycard->SetKeyNumber(cardid);
  ScheduleCall(0,"eject",10,0,pKeycard);
}

func eject(pKeycard)
{
  pKeycard->Exit();
  nocard();
  Sound("eject_card");
}