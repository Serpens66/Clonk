/*-- Maschinengewehr --*/

#strict

local target, aimphase, phase, restshots;
local energyneed,buildingg;

func Munitionsverbrauch(){return(2);}  // regelt den Munitionsverbrauch pro Salve

func RejectGraphChange() {return(1);}  // damit nicht ständig versucht wird eine Wintergrafik zu finden

/* Initialisierung */

protected func Initialize()
{
  InitLocals();
}

public func Init(building, mode, vx, vy, direction)
{
buildingg=building;
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
}

protected func Destruction()
{
  var obj;
  while (obj = FindObject(SMGF, 0,0,0,0, 0,"Show",this()))
    RemoveObject(obj);
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

private func FireBullet(cannon)
{
  // Kugel abschiessen
  var bullet = CreateObject(SMG4,0,0,GetOwner());
  var angle = phase*15/2 + Random(13)-6;
  var xdir = +Sin(angle, 100);
  var ydir = -Cos(angle, 100);
  var xofs = GetXoffset(phase,cannon);
  var yofs = GetYoffset(phase);
  bullet->Launch(GetX()+xofs, GetY()+yofs, xdir,ydir, angle);
  // Mündungsfeuer
  CreateObject(SMGF)->Config(this(), xofs,yofs, phase*15/2);
}

protected func Firing()
{
  // Phase resetten
  SetPhase(phase);
  // beide Kanonen schiessen je eine MG-Kugel
  --restshots;
  var cannon;
  for (cannon; cannon < 2; ++cannon)
    FireBullet(cannon);
  // genug geballert?
  if (restshots) return(1);
  SetAction("Wait");
  SetPhase(phase);
}


/* Steuerung der Kanone */

private func ResetAimPhase()
{
  // Energieverbrauch
  var building = GetActionTarget();
  if (GetEnergy(building) < energyneed)
    return(0);
  // Ziel suchen
  target = GetTargetCrew(500);                    // verfeindeter Clonk
  if (!target) target = GetTargetVehicle(500);    // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(500);     // alle Tier
  if (!target) target = GetOtherThreats(500); // feindliche Raumgleiter, gefährliche tiere und aliens
  if (!target) return(1);
  // Ausrichtungswinkel
  var angle = Angle(GetX(),GetY(), GetX(target)+GetXDir(target), GetY(target)+GetYDir(target));
  aimphase = ((angle*2 + 7) / 15) % 48;
  // passt die Ausrichtung schon?
  if (phase == aimphase)
  {

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(STAP,0,0,0,0,0,0,0,buildingg);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird pro Salve reduziert
    // Feuern!
    restshots = GetShotsPerAttack();
    SetAction("Fire");
    SetPhase(phase);
    // Sound
    Sound("AutoGun*");
    // Energieverbrauch
    DoEnergy(-energyneed, building);
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
}

private func GetShotsPerAttack()  { return(7); }


/* nummerierte Locals enthalten die Positionen
   der beiden Mündungen je nach Animationsphase */

private func InitLocals()
{
  // X-Werte          // mehr X-Werte     // Y-Werte
  Local(0)  = -4;     Local(24) = -4;     Local(48) = -11;
  Local(1)  = -3;     Local(25) = -5;     Local(49) = -11;
  Local(2)  = -2;     Local(26) = -6;     Local(50) = -11;
  Local(3)  = -1;     Local(27) = -7;     Local(51) = -10;
  Local(4)  = +1;     Local(28) = -8;     Local(52) = -10;
  Local(5)  = +2;     Local(29) = -9;     Local(53) = -9;
  Local(6)  = +4;     Local(30) = -11;    Local(54) = -9;
  Local(7)  = +6;     Local(31) = -12;    Local(55) = -8;
  Local(8)  = +8;     Local(32) = -13;    Local(56) = -7;
  Local(9)  = +10;    Local(33) = -14;    Local(57) = -5;
  Local(10) = +12;    Local(34) = -15;    Local(58) = -4;
  Local(11) = +13;    Local(35) = -15;    Local(59) = -2;
  Local(12) = +13;    Local(36) = -15;    Local(60) = +1;
  Local(13) = +14;    Local(37) = -14;    Local(61) = +3;
  Local(14) = +11;    Local(38) = -13;    Local(62) = +5;
  Local(15) = +9;     Local(39) = -13;    Local(63) = +7;
  Local(16) = +7;     Local(40) = -12;    Local(64) = +8;
  Local(17) = +5;     Local(41) = -11;    Local(65) = +9;
  Local(18) = +3;     Local(42) = -10;    Local(66) = +9;
  Local(19) = +2;     Local(43) = -9;     Local(67) = +10;
  Local(20) = 0;      Local(44) = -8;     Local(68) = +10;
  Local(21) = -1;     Local(45) = -7;     Local(69) = +10;
  Local(22) = -2;     Local(46) = -6;     Local(70) = +11;
  Local(23) = -3;     Local(47) = -5;     Local(71) = +11;
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


//Munitionsverbrauch eingebaut
private func EndReloading()
{
  // genug Nachladezeit
  SetAction("Wait");
  SetPhase(phase);
  // gleich wieder ein Ziel suchen
  return(ResetAimPhase());
}

