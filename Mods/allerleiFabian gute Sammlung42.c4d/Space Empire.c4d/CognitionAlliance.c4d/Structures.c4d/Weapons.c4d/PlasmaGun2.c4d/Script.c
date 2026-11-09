/*-- Plasmakanone --*/

#strict

local target, aimphase, angle, phase, control, energyneed;

func RejectGraphChange() {return(1);}

func Munitionsverbrauch(){return(1);} // regelt den Munitionsverbrauch pro Salve

/* Initialisierung */

protected func Initialize()
{
  return(1);
}

public func Init(building, mode, vx, vy, direction)
{
  // Waffenkontrolle aktivieren
  control=CreateObject(_CP2,0,0,GetOwner());
  control->Init(this(), building);
  
  SetAction("Wait", building);
  if (mode) energyneed = 20;
       else energyneed = 0;
  SetVertex(0,0, GetVertex(0,0)+vx);
  SetVertex(0,1, GetVertex(0,1)+vy);
  // Ausrichtung
  if (direction == 0)
    if (GetX() < LandscapeWidth()/2) direction = +1;
                                else direction = -1;
  if (direction < 0) phase = 40;
                else phase = 8;
  SetPhase(phase);
  aimphase=phase;
  return(1);
}

protected func Destruction()
{
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
  if (--phase < 0) phase = 47;
  if (phase != aimphase) return(1);
  // Ausrichtung passt genau
  SetAction("Wait");
  SetPhase(phase);
  return(ResetAimPhase());
}

private func CheckTurnRight()
{
  // solange weiterdrehen, bis richtige Ausrichtung erreicht
  if (++phase > 47) phase = 0;
  if (phase != aimphase) return(1);
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
  if (GetEnergy(building) < energyneed) StopFiring();
  // Ziel suchen
  target = GetTargetCrew(500,AbsX(GetX()),AbsY(GetY()));                    // verfeindeter Clonk
  if (!target) target = GetTargetVehicle(500,AbsX(GetX()),AbsY(GetY()));    // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(500,AbsX(GetX()),AbsY(GetY()));     // gefährliches Tier
  if (!target) target = GetOtherThreats(500,AbsX(GetX()),AbsY(GetY())); // feindliche Raumgleiter, gefährliche tiere und aliens
  
  if (!target) StopFiring(); // Feuer einstellen oder Zielwinkel berechnen
  else {
    angle = Angle(GetX(),GetY(), GetX(target)+GetXDir(target), GetY(target)+GetYDir(target));
    aimphase = ((angle*2 + 7) / 15) % 48;
  }
  // passt die Ausrichtung schon?
  if (phase == aimphase)
  {
    // Feuern!
    if (ActIdle(control) && target) ObjectSetAction(control,"Fire");
    SetPhase(phase);
    return(1);
  }  
  // neu ausrichten
  if ((phase < aimphase) ^ Inside(phase-aimphase, -24, +24))
  {
    SetAction("TurnLeft");
    SetPhase((48-phase) % 48);
    // Abschalten der Kanone, wenn Phasendifferenz zu groß
    if (((phase-aimphase) > 8) || ((phase - aimphase) < -40)) StopFiring();
    return(1);
  }
  SetAction("TurnRight");
  SetPhase(phase);  
  // Abschalten der Kanone, wenn Phasendifferenz zu groß
  if (((aimphase-phase) > 8) || ((aimphase-phase) < -40)) StopFiring();
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
  angle = phase*15/2+Random(5)-2;
  xdir = +Sin(angle, 150);
  ydir = -Cos(angle, 150);
  
  var shot = CreateObject(CPS1,GetX(),GetY(),GetOwner());
  shot->Launch(GetX()+xofs, GetY()+yofs, xdir,ydir, angle);
  
  DoEnergy(-10,GetActionTarget());
  return(1);
}


