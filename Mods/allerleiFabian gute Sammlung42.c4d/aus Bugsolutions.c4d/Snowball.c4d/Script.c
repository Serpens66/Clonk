/*-- Schneeball --*/

#strict 3
#include ICE1

protected func Construction() {
  // Entfernen, wenn keine Schneeballregel und freigegrabener Schneeball
  if (And(And(Equal(GetID(), SNWB), !Contained()), !FindObject(RSNB, nil, nil, -1, -1)))
    RemoveObject();
}

protected func Hit() {
  CastPXS("Snow", 100, 20);
  RemoveObject();
  return(1);
}

protected func Departure(thrower) {
  var proc = GetProcedure(thrower);
  var comd = GetComDir(thrower);
  // Ablegen -> Abbruch
  if (GetPlrDownDouble(GetOwner(thrower)))
    // Nur Ablegen in der Luft mit Befehlsrichtung? Luftwurf erlauben!
    if (Or(!SEqual(proc, "FLIGHT"), Equal(GetComDir(thrower), COMD_None)))
      return(nil);
  
  // Ablegen im stehenden Hangeln -> Abbruch
  if (And(!!SEqual(proc, "HANGLE"), Equal(comd, COMD_None)))
    return(nil);
  
  // Ablegen im Schwimmen / Klettern -> Abbruch
  if (Or(!!SEqual(proc, "SWIM"), !!SEqual(proc, "SCALE")))
    return(nil);
  
  // Wurfrichtung rausfinden
  var dir;
  // Nach Befehlsrichtung, wenn sinnvolle Werte vorhanden
  if (Or(Equal(comd, COMD_Left), Equal(comd, COMD_Right))) {
    if (Equal(comd, COMD_Left))  dir = -1;
    if (Equal(comd, COMD_Right)) dir = +1;
  }
  // sonst nach Blickrichtung
  else {
    if (Equal(GetDir(thrower), DIR_Left)) dir = -1;
    else dir = +1;
  }

  // Wurfgeschwindigkeiten berechnen
  var x_dir = (dir ?? 0) * (GetPhysical("Throw", nil, thrower) ?? 0)/1000 + (GetXDir(thrower) ?? 0) / 3;
  var y_dir = -30;

  if (!x_dir) return(nil);
  
  SetPosition((GetX() ?? 0) + (x_dir ?? 0) / 5, (GetY() ?? 0) - 2 + (GetYDir(thrower) ?? 0) /  2);

  // Geschwindigkeit setzen
  SetXDir(x_dir);
  if(!!SEqual(GetAction(thrower), "Hangle")) // Beim Hangeln
    SetYDir(-(y_dir ?? 0)/2);               // wird nicht nach oben geworfen
  else
    SetYDir(y_dir);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(100); }
