/*-- Vulcan-Waffe --*/

#strict
#include STW1     // basiert auf dem MG

func RejectGraphChange() {return(1);}

private func FireBullet(cannon)
{
  // Kugel abschiessen
  var bullet = CreateObject(SMG5,0,0,GetOwner());
  var angle = phase*15/2 + Random(13)-6;
  var xdir = +Sin(angle, 100);
  var ydir = -Cos(angle, 100);
  var xofs = GetXoffset(phase,cannon);
  var yofs = GetYoffset(phase);
  bullet->Launch(GetX()+xofs, GetY()+yofs, xdir,ydir, angle);
  // Mündungsfeuer
  CreateObject(SMGF)->Config(this(), xofs,yofs, phase*15/2);
  // zusätzlicher Effekt
  for (var i = 0; i < 3; ++i)
    CreateParticle("FlameSpark", xofs,yofs, xdir/10 + Random(21)-10,
                                            ydir/10 + Random(21)-10, 30+Random(11), GetSparkColor());
}

protected func Firing()
{
  // Direction setzen
  inherited();
  SetDir(1 - GetDir());
}

private func GetShotsPerAttack()  { return(5); }

private func GetSparkColor()
{
  var lg = Random(256);
  return(RGBa(255,lg,0,Random(192)));
}

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
    munitionspack = FindObject(GSAP,0,0,0,0,0,0,0,buildingg);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(1); // restmunition im Paket wird um 1 pro Salve reduziert
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
