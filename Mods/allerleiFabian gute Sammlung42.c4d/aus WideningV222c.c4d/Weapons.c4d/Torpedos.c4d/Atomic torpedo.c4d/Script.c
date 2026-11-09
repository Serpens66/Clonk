/*-- Torpedo --*/

#strict 3

public func IsWaste(){ return(1);}

func Initialize(...) {
  DoScore(nil, -5);
  return(1);
}

/* Initialisierung */

protected func Initialize() {
  DoScore(nil, 500);
  SetAction("Walk");
  SetComDir(COMD_Stop);
}

public func Launch (object by) {
  SetAction ("Swim");
  SetDir(GetDir(by));
  SetXDir(GetXDir(by));
  SetYDir(nil);
  if (Equal(GetDir(), DIR_Right)) SetComDir(COMD_Right);
  if (Equal(GetDir(), DIR_Left)) SetComDir(COMD_Left);
  return(1);
}

/* Einsammeln */

func Entrance (object container) {
  SetAction("Walk");
}

/* Effekte */

private func Swimming() {
  var iDir=((GetDir() ?? 0)*2-1);
  Bubble(-16*(iDir ?? 0),nil);
  Bubble(-16*(iDir ?? 0),nil);
  if ((GetActTime() ?? 0) > 25) SetYDir(nil);
}

/* Aufschlag */

public func Hit(...) {
  Sound("HEADCRAS");	
  CreateObject(ABEX);
  // Druckwelle & Strahlung
var obj;
  obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 300,GetX(),GetY(), 200+(Random(100) ?? 0), 300+(Random(200) ?? 0));
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());

//Der Umwelt globalnature schaden. Wenn es soviele A-Bomben wie Spieler gezündet wurden, dann sinkt die globalnature auf 90% von dem Wert davor.
DecNature(((((startnature ?? 0)*90/100)-(startnature ?? 0))/(GetPlayerCount() ?? 0)));

  RemoveObject();
  return(1);
}
  


/* Produkteigenschaften */

public func IsChemicalProduct(...) { return(1);
}

/* Status */

public func IsTorpedo(...) { return(1);
}

/* Forschung */

public func GetResearchBase(...) { return(TRP2);
}

