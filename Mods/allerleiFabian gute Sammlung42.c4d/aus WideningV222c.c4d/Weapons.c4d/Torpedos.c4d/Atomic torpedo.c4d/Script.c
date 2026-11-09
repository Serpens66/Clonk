/*-- Torpedo --*/

#strict

public func IsWaste(){ return(1);}

Initialize:
  DoScore(0, -5);
  return(1);

/* Initialisierung */

protected func Initialize() {
  DoScore(0, 500);	
  SetAction("Walk");
  SetComDir(COMD_Stop());
}

public func Launch (object by) {
  SetAction ("Swim");
  SetDir(GetDir(by));
  SetXDir(GetXDir(by));
  SetYDir(0);
  if (GetDir()==DIR_Right()) SetComDir(COMD_Right());
  if (GetDir()==DIR_Left()) SetComDir(COMD_Left());
  return(1);
}

/* Einsammeln */

func Entrance (object container) {
  SetAction("Walk");
}

/* Effekte */

private func Swimming() {
  var iDir=(GetDir()*2-1);
  Bubble(-16*iDir,0);
  Bubble(-16*iDir,0);
  if (GetActTime() > 25) SetYDir(0);
}

/* Aufschlag */

public Hit:
  Sound("HEADCRAS");	
  CreateObject(ABEX);
  // Druckwelle & Strahlung
var obj;
  obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 300,GetX(),GetY(), 200+Random(100), 300+Random(200));
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());

//Der Umwelt globalnature schaden. Wenn es soviele A-Bomben wie Spieler gezündet wurden, dann sinkt die globalnature auf 90% von dem Wert davor.
DecNature((((startnature*90/100)-startnature)/GetPlayerCount()));

  RemoveObject();
  return(1);
  


/* Produkteigenschaften */

public IsChemicalProduct: return(1);

/* Status */

public IsTorpedo: return(1);

/* Forschung */

public GetResearchBase: return(TRP2);

