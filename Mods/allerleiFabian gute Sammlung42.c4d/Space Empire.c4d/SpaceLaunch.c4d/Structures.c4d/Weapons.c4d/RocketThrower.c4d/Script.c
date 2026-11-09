/*-- Raketenwerfer --*/

//verwendet SROC Raketen 

#strict

local target, aimphase, phase, cannon;
local energyneed;

func Munitionsverbrauch(){return(1);} // regelt den Munitionsverbrauch pro Salve

func RejectGraphChange() {return(1);}

/* Initialisierung */

protected func Initialize()
{
  InitLocals();
}

public func Init(building, mode, vx, vy, direction)
{
  SetAction("Wait", building);
  if (mode) energyneed = 30;
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
  if (GetEnergy(building) < energyneed)
    return(0);
  // Ziel suchen
  target = GetTargetVehicle(700);      // gegnerisches Fahrzeug
  if (!target) target = GetTargetCrew(700);    // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(700);    // alle tiere
  if (!target) target = GetOtherThreats(700); // feindliche Raumgleiter, gefährliche tiere und aliens
  if (!target) return(1);
  // Ausrichtungswinkel
  var angle = Angle(GetX(),GetY(), GetX(target)+GetXDir(target), GetY(target)+GetYDir(target));
  aimphase = ((angle*2 + 7) / 15) % 48;
  // passt die Ausrichtung schon?
  if (phase == aimphase)
  {
    // Drehung anhalten
    SetAction("Wait");
    SetPhase(phase);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(MIAP,0,0,0,0,0,0,0,building);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert 
      // abfeuern
      var rocket= CreateContents(SROC,building,1);
      var xofs = GetXoffset(phase,cannon);
      var yofs = GetYoffset(phase);
      Exit(rocket, xofs,yofs, angle);
      SetPosition(GetX()+xofs, GetY()+yofs, rocket);
      rocket->Launch(this(),angle);
      // Sound
      Sound("Launch");
      // Energieverbrauch
      DoEnergy(-energyneed, building);
      // nachladen
      SetAction("Reload");
      SetPhase(phase);
      // nächster Schuss aus anderer Kanone        
      cannon = !cannon;
    
    return(1);
  }
  // neu ausrichten
  if ((phase < aimphase) ^ Inside(phase-aimphase, -24, +24))
  {
    SetAction("TurnLeft");
    SetPhase((48-phase) % 48);
    return(1);
  }
  SetAction("TurnRight");
  SetPhase(phase);
  return(1);
}


/* nummerierte Locals enthalten die Positionen
   der beiden Mündungen je nach Animationsphase */

private func InitLocals()
{
  // X-Werte          // mehr X-Werte     // Y-Werte
  Local(0)  = -4;     Local(24) = -4;     Local(48) = -11;
  Local(1)  = -3;     Local(25) = -5;     Local(49) = -11;
  Local(2)  = -1;     Local(26) = -6;     Local(50) = -11;
  Local(3)  = 0;      Local(27) = -7;     Local(51) = -10;
  Local(4)  = +1;     Local(28) = -8;     Local(52) = -10;
  Local(5)  = +3;     Local(29) = -9;     Local(53) = -9;
  Local(6)  = +5;     Local(30) = -10;    Local(54) = -8;
  Local(7)  = +6;     Local(31) = -11;    Local(55) = -7;
  Local(8)  = +8;     Local(32) = -13;    Local(56) = -6;
  Local(9)  = +10;    Local(33) = -14;    Local(57) = -4;
  Local(10) = +12;    Local(34) = -15;    Local(58) = -3;
  Local(11) = +14;    Local(35) = -15;    Local(59) = -1;
  Local(12) = +14;    Local(36) = -15;    Local(60) = +1;
  Local(13) = +14;    Local(37) = -15;    Local(61) = +3;
  Local(14) = +12;    Local(38) = -14;    Local(62) = +5;
  Local(15) = +9;     Local(39) = -13;    Local(63) = +6;
  Local(16) = +7;     Local(40) = -12;    Local(64) = +7;
  Local(17) = +5;     Local(41) = -11;    Local(65) = +8;
  Local(18) = +3;     Local(42) = -10;    Local(66) = +9;
  Local(19) = +1;     Local(43) = -9;     Local(67) = +9;
  Local(20) = 0;      Local(44) = -8;     Local(68) = +10;
  Local(21) = -1;     Local(45) = -7;     Local(69) = +10;
  Local(22) = -2;     Local(46) = -6;     Local(70) = +10;
  Local(23) = -3;     Local(47) = -5;     Local(71) = +10;
  return(1);
}

private func GetXoffset(phase, cannon)
{
  // rechte Kanone
  if (cannon)
  { 
    phase = (48-phase) % 48;
    return(-Local(phase)-1);
  }
  // linke Kanone
  return(Local(phase));
}

private func GetYoffset(phase)
{
  // auf Phase zwischen 0 und 24 zurückrechnen
  if (phase > 24)
    phase = (48-phase) % 48;
  return(Local(phase+48));
}


/* Energieverbrauch */

public EnergyNeed: return(energyneed);

