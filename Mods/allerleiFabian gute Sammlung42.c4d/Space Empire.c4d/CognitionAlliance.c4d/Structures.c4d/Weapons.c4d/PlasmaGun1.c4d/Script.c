/*-- Plasmakanone --*/

#strict 3

local target, aimphase, phase;
local energymode;

func RejectGraphChange() {return(1);}

func Munitionsverbrauch(){return(1);} // regelt den Munitionsverbrauch pro Salve

/* Initialisierung */

public func Init(building, mode, vx, vy, direction)
{
  SetAction("Wait", building);
  energymode = mode;
  SetVertex(nil,nil, (GetVertex(nil,nil) ?? 0)+(vx ?? 0));
  SetVertex(nil,1, (GetVertex(nil,1) ?? 0)+(vy ?? 0));
  // Ausrichtung
  if (Equal(direction, nil))
    if ((GetX() ?? 0) < (LandscapeWidth() ?? 0)/2) direction = +1;
                                else direction = -1;
  if ((direction ?? 0) < 0) phase = 31;
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
  if (S42MigrationStep(phase, -1) < 0) phase = 35;
  if (!Equal(phase, aimphase)) return(1);
  // Ausrichtung passt genau
  SetAction("Wait");
  SetPhase(phase);
  return(ResetAimPhase());
}

private func CheckTurnRight()
{
  // solange weiterdrehen, bis richtige Ausrichtung erreicht
  if (S42MigrationStep(phase, 1) > 35) phase = nil;
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
  if (And(energymode, ((GetEnergy(building) ?? 0) < 30)))
    return(nil);
  // Ziel suchen
  target = GetTargetCrew(650,AbsX(GetX()),AbsY(GetY()));                 // gegnerischer Clonk
  if (!target) target = GetTargetVehicle(650,AbsX(GetX()),AbsY(GetY())); // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(650,AbsX(GetX()),AbsY(GetY()));  // wenigstens ein Tier als Ziel verfügbar?
  if (!target) target = GetOtherThreats(650,AbsX(GetX()),AbsY(GetY())); // feindliche Raumgleiter, gefährliche tiere und aliens
  if (!target) return(1);
  // Ausrichtungswinkel
  var angle = Angle(GetX(),GetY(), (GetX(target) ?? 0)+(GetXDir(target) ?? 0), (GetY(target) ?? 0)+(GetYDir(target) ?? 0));
  aimphase = (((angle ?? 0) + 7) / 10) % 36;
  // passt die Ausrichtung schon?
  if (Equal(phase, aimphase))
  {

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(PTAP,nil,nil,nil,nil,nil,nil,nil,building);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert

    // Drehung anhalten
    SetAction("Wait");
    SetPhase(phase);
    // Munition suchen
    var shot=CreateObject(CPS1,nil,nil,GetOwner(building));
    shot->Launch(GetX(),GetY(),Sin(angle,100), -(Cos(angle,100) ?? 0));
    
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
  if ((((phase ?? 0) < (aimphase ?? 0)) ?? 0) ^ (Inside((phase ?? 0)-(aimphase ?? 0), -18, +18) ?? 0))
  {
    SetAction("TurnLeft");
    SetPhase((36-(phase ?? 0)) % 36);
    return(1);
  }
  SetAction("TurnRight");
  SetPhase(phase);
  return(1);
}

