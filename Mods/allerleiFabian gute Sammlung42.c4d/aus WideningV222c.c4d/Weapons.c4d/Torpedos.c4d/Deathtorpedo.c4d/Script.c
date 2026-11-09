/*-- Torpedo --*/

#strict 3

/* Initialisierung */

protected func Initialize() {
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
  CastObjects(SPL1,40,300);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(50);
  else FakeExplode(50,nil,nil,50);
  Sound("HEADCRAS");
  return(1);
}


/* Produkteigenschaften */

public func IsChemicalProduct(...) { return(1);
}

/* Status */

public func IsTorpedo(...) { return(1);
}

/* Forschung */

public func GetResearchBase(...) { return(TRP3);
}

