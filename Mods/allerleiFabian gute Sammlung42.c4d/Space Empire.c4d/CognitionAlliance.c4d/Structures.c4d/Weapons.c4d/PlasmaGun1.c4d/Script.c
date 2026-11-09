/*-- Plasmakanone --*/

#strict

local target, aimphase, phase;
local energymode;

func RejectGraphChange() {return(1);}

func Munitionsverbrauch(){return(1);} // regelt den Munitionsverbrauch pro Salve

/* Initialisierung */

public func Init(building, mode, vx, vy, direction)
{
  SetAction("Wait", building);
  energymode = mode;
  SetVertex(0,0, GetVertex(0,0)+vx);
  SetVertex(0,1, GetVertex(0,1)+vy);
  // Ausrichtung
  if (direction == 0)
    if (GetX() < LandscapeWidth()/2) direction = +1;
                                else direction = -1;
  if (direction < 0) phase = 31;
                else phase = 5;
  SetPhase(phase);
}


/* Action-Calls */

protected func Waiting()
{
  // Phase resetten
  SetPhase(phase);
  // Ziel suchen & ausrichten
  return(ResetAimPhase());
}

private func EndReloading()
{
  // genug Nachladezeit
  SetAction("Wait");
  SetPhase(phase);
  // gleich wieder ein Ziel suchen
  return(ResetAimPhase());
}

private func CheckTurnLeft()
{
  // solange weiterdrehen, bis richtige Ausrichtung erreicht
  if (--phase < 0) phase = 35;
  if (phase != aimphase) return(1);
  // Ausrichtung passt genau
  SetAction("Wait");
  SetPhase(phase);
  return(ResetAimPhase());
}

private func CheckTurnRight()
{
  // solange weiterdrehen, bis richtige Ausrichtung erreicht
  if (++phase > 35) phase = 0;
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
  if (energymode && (GetEnergy(building) < 30))
    return(0);
  // Ziel suchen
  target = GetTargetCrew(650,AbsX(GetX()),AbsY(GetY()));                 // gegnerischer Clonk
  if (!target) target = GetTargetVehicle(650,AbsX(GetX()),AbsY(GetY())); // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(650,AbsX(GetX()),AbsY(GetY()));  // wenigstens ein Tier als Ziel verfügbar?
  if (!target) target = GetOtherThreats(650,AbsX(GetX()),AbsY(GetY())); // feindliche Raumgleiter, gefährliche tiere und aliens
  if (!target) return(1);
  // Ausrichtungswinkel
  var angle = Angle(GetX(),GetY(), GetX(target)+GetXDir(target), GetY(target)+GetYDir(target));
  aimphase = ((angle + 7) / 10) % 36;
  // passt die Ausrichtung schon?
  if (phase == aimphase)
  {

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(PTAP,0,0,0,0,0,0,0,building);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert

    // Drehung anhalten
    SetAction("Wait");
    SetPhase(phase);
    // Munition suchen
    var shot=CreateObject(CPS1,0,0,GetOwner(building));
    shot->Launch(GetX(),GetY(),Sin(angle,100), -Cos(angle,100)); 
    
    // Sound
    Sound("Plasmashot");
    // Energieverbrauch
    if (energymode)
      DoEnergy(-10, building);
    // nachladen
    SetAction("Reload");
    SetPhase(phase);
    return(1);
  }
  // neu ausrichten
  if ((phase < aimphase) ^ Inside(phase-aimphase, -18, +18))
  {
    SetAction("TurnLeft");
    SetPhase((36-phase) % 36);
    return(1);
  }
  SetAction("TurnRight");
  SetPhase(phase);
  return(1);
}

