/*-- Vulcan-Waffe --*/

#strict 3
#include STW1     // basiert auf dem MG

func RejectGraphChange() {return(1);}

private func FireBullet(cannon)
{
  // Kugel abschiessen
  var bullet = CreateObject(SMG5,nil,nil,GetOwner());
  var angle = (phase ?? 0)*15/2 + (Random(13) ?? 0)-6;
  var xdir = +(Sin(angle, 100) ?? 0);
  var ydir = -(Cos(angle, 100) ?? 0);
  var xofs = GetXoffset(phase,cannon);
  var yofs = GetYoffset(phase);
  bullet->Launch((GetX() ?? 0)+(xofs ?? 0), (GetY() ?? 0)+(yofs ?? 0), xdir,ydir, angle);
  // Mündungsfeuer
  CreateObject(SMGF)->Config(this(), xofs,yofs, (phase ?? 0)*15/2);
  // zusätzlicher Effekt
  for (var i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1))
    CreateParticle("FlameSpark", xofs,yofs, (xdir ?? 0)/10 + (Random(21) ?? 0)-10,
                                            (ydir ?? 0)/10 + (Random(21) ?? 0)-10, 30+(Random(11) ?? 0), GetSparkColor());
}

protected func Firing()
{
  // Direction setzen
  inherited();
  SetDir(1 - (GetDir() ?? 0));
}

private func GetShotsPerAttack()  { return(5); }

private func GetSparkColor()
{
  var lg = Random(256);
  return(RGBa(255,lg,nil,Random(192)));
}

private func ResetAimPhase()
{
  // Energieverbrauch
  var building = GetActionTarget();
  if ((GetEnergy(building) ?? 0) < (energyneed ?? 0))
    return(nil);
  // Ziel suchen
  target = GetTargetCrew(500);                    // verfeindeter Clonk
  if (!target) target = GetTargetVehicle(500);    // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(500);     // alle Tier
  if (!target) target = GetOtherThreats(500); // feindliche Raumgleiter, gefährliche tiere und aliens
  if (!target) return(1);
  // Ausrichtungswinkel
  var angle = Angle(GetX(),GetY(), (GetX(target) ?? 0)+(GetXDir(target) ?? 0), (GetY(target) ?? 0)+(GetYDir(target) ?? 0));
  aimphase = (((angle ?? 0)*2 + 7) / 15) % 48;
  // passt die Ausrichtung schon?
  if (Equal(phase, aimphase))
  {

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(GSAP,nil,nil,nil,nil,nil,nil,nil,buildingg);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(1); // restmunition im Paket wird um 1 pro Salve reduziert
    // Feuern!
    restshots = GetShotsPerAttack();
    SetAction("Fire");
    SetPhase(phase);
    // Sound
    Sound("AutoGun*");
    // Energieverbrauch
    DoEnergy(-(energyneed ?? 0), building);
    return(1);
}
  // neu ausrichten
  if ((((phase ?? 0) < (aimphase ?? 0)) ?? 0) ^ (Inside((phase ?? 0)-(aimphase ?? 0), -24, +24) ?? 0))
  {
    SetAction("TurnLeft");
    SetPhase((48-(phase ?? 0)) % 48);
    return(1);
  }
  SetAction("TurnRight");
  SetPhase(phase);
}
