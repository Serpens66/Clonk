/*-- Maschinengewehr --*/

#strict 3

local target, aimphase, angle, phase, control, energyneed,schaaden;

func RejectGraphChange() {return(1);}

func Munitionsverbrauch(){if(!Random(5)) return(1); return(nil);} // regelt den Munitionsverbrauch pro Salve


func Schaaden() {return(1);}  // zusätzlicher Schaden der MG Kugel, damit minigun mehr schaden macht als maschinengewehr


/* Initialisierung */

protected func Initialize()
{
  schaaden= Schaaden();
  return(1);
}

public func Init(building, mode, vx, vy, direction)
{
  // Waffenkontrolle aktivieren
  control=CreateObject(_CMG,nil,nil,GetOwner());
  control->Init(this(), building);
  
  SetAction("Wait", building);
  if (mode) energyneed = 20;
       else energyneed = nil;
  SetVertex(nil,nil, (GetVertex(nil,nil) ?? 0)+(vx ?? 0));
  SetVertex(nil,1, (GetVertex(nil,1) ?? 0)+(vy ?? 0));
  // Ausrichtung
  if (Equal(direction, nil))
    if ((GetX() ?? 0) < (LandscapeWidth() ?? 0)/2) direction = +1;
                                else direction = -1;
  if ((direction ?? 0) < 0) phase = 40;
                else phase = 8;
  SetPhase(phase);
  aimphase=phase;
  return(1);
}

protected func Destruction()
{
  var obj;
  while (obj = FindObject(SMGF, nil,nil,nil,nil, nil,"Show",this()))
    RemoveObject(obj);
  if (control) RemoveObject(control);
  return(1);
}


/* Action-Calls */

protected func Waiting()
{
  // Phase resetten
  SetPhase(phase);
  // Ziel suchen & ausrichten
  return(ResetAimPhase());
}

private func CheckTurnLeft()
{
  // solange weiterdrehen, bis richtige Ausrichtung erreicht
  if (S42MigrationStep(phase, -1) < 0) phase = 47;
  if (!Equal(phase, aimphase)) return(1);
  // Ausrichtung passt genau
  SetAction("Wait");
  SetPhase(phase);
  return(ResetAimPhase());
}

private func CheckTurnRight()
{
  // solange weiterdrehen, bis richtige Ausrichtung erreicht
  if (S42MigrationStep(phase, 1) > 47) phase = nil;
  if (!Equal(phase, aimphase)) return(1);
  // Ausrichtung passt genau
  SetAction("Wait");
  SetPhase(phase);
  return(ResetAimPhase());
}

/* Steuerung der Kanone */

private func ResetAimPhase()
{
  // Energieverbrauch
  var building = GetActionTarget();
  if ((GetEnergy(building) ?? 0) < (energyneed ?? 0)) StopFiring();
  // Ziel suchen
  target = GetTargetCrew(500,AbsX(GetX()),AbsY(GetY()));                    // verfeindeter Clonk
  if (!target) target = GetTargetVehicle(500,AbsX(GetX()),AbsY(GetY()));    // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(500,AbsX(GetX()),AbsY(GetY()));     // Tiere.. sind harmlos
  if (!target) target = GetOtherThreats(500,AbsX(GetX()),AbsY(GetY()));  // feindliche Raumgleiter , aliens und gefährliche tiere
  
  if (!target) StopFiring(); // Feuer einstellen oder Zielwinkel berechnen
  else {
    angle = Angle(GetX(),GetY(), (GetX(target) ?? 0)+(GetXDir(target) ?? 0), (GetY(target) ?? 0)+(GetYDir(target) ?? 0));
    aimphase = (((angle ?? 0)*2 + 7) / 15) % 48;
  }
  // passt die Ausrichtung schon?
  if (Equal(phase, aimphase))
  {
    // Feuern!
    if (And((!SEqual(GetAction(control), "Fire")), target)) ObjectSetAction(control,"Fire");
    SetPhase(phase);
    return(1);
  }  
  // neu ausrichten
  if ((((phase ?? 0) < (aimphase ?? 0)) ?? 0) ^ (Inside((phase ?? 0)-(aimphase ?? 0), -24, +24) ?? 0))
  {
    SetAction("TurnLeft");
    SetPhase((48-(phase ?? 0)) % 48);
    // Abschalten der Kanone, wenn Phasendifferenz zu groß
    if (Or((((phase ?? 0)-(aimphase ?? 0)) > 8), (((phase ?? 0) - (aimphase ?? 0)) < -40))) StopFiring();
    return(1);
  }
  SetAction("TurnRight");
  SetPhase(phase);  
  // Abschalten der Kanone, wenn Phasendifferenz zu groß
  if (Or((((aimphase ?? 0)-(phase ?? 0)) > 8), (((aimphase ?? 0)-(phase ?? 0)) < -40))) StopFiring();
  return(1);
}

// Feuerroutinen

private func StopFiring()
{
  if (!ActIdle(control)) ObjectSetAction(control, "Idle");
  return(1);
}

public func FireShot(xofs, yofs, xdir, ydir)
{
  angle = (phase ?? 0)*15/2 + (Random(13) ?? 0)-6;
  xdir = +(Sin(angle, 100) ?? 0);
  ydir = -(Cos(angle, 100) ?? 0);
  
  var bullet = CreateObject(SMG4,GetX(),GetY(),GetOwner());
  bullet->Launch((GetX() ?? 0)+(xofs ?? 0), (GetY() ?? 0)+(yofs ?? 0), xdir,ydir, angle, schaaden);
  
  // Energie abzapfen
  DoEnergy(-2,GetActionTarget());
  // Mündungsfeuer
  CreateObject(SMGF)->Config(this(), xofs,yofs, (phase ?? 0)*15/2);
  return(1);
}

/* Energieverbrauch */

public func EnergyNeed(...) { return(energyneed);
}

