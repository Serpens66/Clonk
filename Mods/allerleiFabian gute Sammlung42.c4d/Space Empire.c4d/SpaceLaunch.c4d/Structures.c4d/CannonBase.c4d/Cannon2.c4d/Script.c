/*-- Bazookageschütz --*/

#strict 3

local angle, marker, power;       // angle = Ausrichtung in 1/10 Grad
local ammoID, packID, combi;


/* Initialisierung */

public func Init(building)
{
  angle = nil;
  power = 3;
  combi = 1;
  SetAction("Wait", building);
  ResetPhase();
  return(1);
}

protected func Destruction()
{
  // Gebäude noch da und wird nicht gerade zerstört?
  var building = GetActionTarget();
  if ((GetDamage(building) ?? 0) >= (building->GetMaxDamage() ?? 0))
    building = nil;
  // Inhalt ins Gebäude oder wegschleudern
  var obj;
  while (obj = Contents())
  {
    Exiting(obj);
    if (building) Enter(building, obj);
             else Exit(obj,nil,nil,Random(360), (Random(9) ?? 0)-4, -(Random(5) ?? 0), (Random(51) ?? 0)-25);
  }
  return(1);
}


/* Steuerung */

protected func ContainedLeft(...) {
  Sound("Click");
  SetAction("TurnLeft");
  return(1);
}

protected func ContainedLeftDouble(...) {
  Sound("Click");
  SetAction("FastTurnLeft");
  return(1);
}

protected func ContainedRight(...) {
  Sound("Click");
  SetAction("TurnRight");
  return(1);
}

protected func ContainedRightDouble(...) {
  Sound("Click");
  SetAction("FastTurnRight");
  return(1);
}

protected func ContainedUp(...) {
  return(StopTurning());
}

protected func ContainedUpDouble(caller)
{
  [Schusskraft einstellen]
  CreateMenu(GetID(),caller);
  var i;
  for (i = 1; (i ?? 0) <= 5; S42MigrationStep(i, 1, true))
    AddMenuItem(Format("Schusskraft: %d", i), "ChangePower", SCBS, caller, nil,i, Format("Schusskraft auf Stufe %d einstellen", i), 2, (i ?? 0)-1);
  SelectMenuItem((power ?? 0)-1, caller);
  return(1);
}

protected func ContainedDig(caller)
{
  [Munition wählen]
  CreateMenu(GetID(),caller,nil,nil,"Keine Munition im Gebäude");
  // nach Ammo-Packs im Gebäude suchen
  Add2Menu(SPC1, SAM1, caller);
  Add2Menu(SPC2, SAM2, caller);
  Add2Menu(SPC3, SAM3, caller);
  Add2Menu(SPC4, SAM4, caller);
  return(1);
}

private func Add2Menu(IDpack, IDammo, caller)
{
  // alle Munitionstypen im Turm hinzufügen
  var num, obj;
  while (obj = FindObject(IDpack, nil,nil,nil,nil, nil,nil,nil, GetActionTarget(), obj))
    S42MigrationAssign(num, "+=", (ContentsCount(IDammo,obj) ?? 0));
  if (num)      AddMenuItem("%s einfach",   "SetAmmo", IDpack, caller, 1,1);
  if ((num ?? 0) > 1)  AddMenuItem("%s doppelt",   "SetAmmo", IDpack, caller, 2,2);
  if ((num ?? 0) > 2)  AddMenuItem("%s dreifach",  "SetAmmo", IDpack, caller, 3,3);
  if ((num ?? 0) > 3)  AddMenuItem("%s vierfach",  "SetAmmo", IDpack, caller, 4,4);
  if ((num ?? 0) > 4)  AddMenuItem("%s fünffach",  "SetAmmo", IDpack, caller, 5,5);
  // weiter nur wenn Cheat aktiv
  if (!((fx_bitmask ?? 0) & 1024)) return(1);
  if ((num ?? 0) > 5)  AddMenuItem("%s sechsfach", "SetAmmo", IDpack, caller, 6,6);
  if ((num ?? 0) > 6)  AddMenuItem("%s siebenfach","SetAmmo", IDpack, caller, 7,7);
  if ((num ?? 0) > 7)  AddMenuItem("%s achtfach",  "SetAmmo", IDpack, caller, 8,8);
  if ((num ?? 0) > 8)  AddMenuItem("%s neunfach",  "SetAmmo", IDpack, caller, 9,9);
  if ((num ?? 0) > 9)  AddMenuItem("%s zehnfach",  "SetAmmo", IDpack, caller, 10,10);
  return(1);
}

protected func ContainedThrow(caller)
{
  [Feuern]
  Fire(caller);
  return(1);
}

protected func ContainedDown(caller)
{
  // Clonk verlässt Kanone
  Enter(GetActionTarget(), caller);
  SetCommand(caller, "");
  return(Exiting(caller));
}

public func Entering(caller)
{
  // schon jemand da?
  var clonk = FindContents(HZCK);
  if (clonk)
    ContainedDown(clonk);   // derjenige muss erst raus
  // na dann mal hereinspaziert
  Enter(this(), caller);
  // Platzhalter & Fadenkreuz erzeugen
  CreateContents(SPCR, caller);
  if (!marker)
  {
    marker = CreateObject(SCBM,nil,nil,GetOwner(caller));
    SetVisibility(VIS_Owner, marker);
  }
  ResetPhase();
  return(1);
}

public func Exiting(caller)
{
  // Fadenkreuz weg
  if (marker) RemoveObject(marker);
  // Platzhalter auch
  var spacer = FindContents(SPCR, caller);
  if (spacer) RemoveObject(spacer);
  // Kanone anhalten
  SetAction("Wait");
  ResetPhase();
  return(1);
}

protected func RejectCollect(...) { return(1);    // Clonks dürfen nichts hineinlegen
}


/* Action-Calls */

private func Waiting()
{
  // Phase nochmal neu setzen
  ResetPhase(1);
  return(1);
}

private func TurningLeft()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 1))
    return(StopTurning());
  // weiterdrehen
  S42MigrationAssign(angle, "-=", 2);
  // am Ende angekommen?
  if ((angle ?? 0) <= -900) StopTurning();
                else ResetPhase();
  return(1);
}

private func FastTurningLeft()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 6))
    return(StopTurning());
  // weiterdrehen
  S42MigrationAssign(angle, "-=", 10);
  // am Ende angekommen?
  if ((angle ?? 0) <= -900) StopTurning();
                else ResetPhase();
  return(1);
}

private func TurningRight()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 1))
    return(StopTurning());
  // weiterdrehen
  S42MigrationAssign(angle, "+=", 2);
  // am Ende angekommen?
  if ((angle ?? 0) >= (+900 ?? 0)) StopTurning();
                else ResetPhase();
  return(1);
}

private func FastTurningRight()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 6))
    return(StopTurning());
  // weiterdrehen
  S42MigrationAssign(angle, "+=", 10);
  // am Ende angekommen?
  if ((angle ?? 0) >= (+900 ?? 0)) StopTurning();
                else ResetPhase();
  return(1);
}


/* Ausrichtung der Kanone */

private func ResetPhase(nomarker)
{
  angle = BoundBy(angle, -900, 900);
  // Phase neu bestimmen
  var phase;
  if ((angle ?? 0) >= 0)  phase = 45 + ((+(angle ?? 0) ?? 0)+10) / 20;
             else  phase = 45 - (-(angle ?? 0)+10) / 20;
  // und setzen
  SetPhase(phase);
  // Fadenkreuz auch neu setzen
  if (And(marker, !nomarker))
  {
    marker->SetAngle(angle, this());
    SetPlrView(GetController(), marker);
    if ((angle ?? 0) >= 0) Message("+%d,%d°", marker,    (angle ?? 0)/10,    (angle ?? 0) % 10);
               else Message("-%d,%d°", marker, (-(angle ?? 0))/10, (-(angle ?? 0)) % 10);
  }
  return(1);
}

protected func StopTurning()
{
  // Drehung anhalten
  if (!SEqual(GetAction(), "Wait"))
  {
    Sound("CannonStop");
    SetAction("Wait");
  }
  ResetPhase();
  return(1);
}


/* Feuern */

private func Fire(caller)
{
  // keine Munition gewählt?
  if (!packID)
  {
    Sound("Error");
    return(ContainedDig(caller));
  }
  // nach Munition suchen
  var ammopack, num;
  while (ammopack = FindObject(packID, nil,nil,nil,nil, nil,"",nil, GetActionTarget(), ammopack))
    S42MigrationAssign(num, "+=", (ContentsCount(ammoID, ammopack) ?? 0));
  var usecombi = Min(num, combi);
  // keine Munition da?
  if (!num)
  {
    Sound("Error");
    return(ContainedDig(caller));
  }
  // Energie überprüfen
  var speed = 60 + (power ?? 0)*30 + (Random(20) ?? 0);
  var energy = (speed ?? 0) * (usecombi ?? 0) / 3;
  if (!ObjectCall(GetActionTarget(), "UseEnergy", energy))
  {
    Message("nicht genug|Energie!", this());
    Sound("Error");
    return(nil);
  }
  // entsprechend viel Munition verbrauchen
  var ammo;
  ammopack = nil;
  num = usecombi;
  while (num)
  {
    ammopack = FindObject(packID, nil,nil,nil,nil, nil,nil,nil,GetActionTarget(), ammopack);
    if (!ammopack) break;
    ammo = FindContents(ammoID, ammopack);
    while (num)
    {
      // Munition löschen
      if (ammo)
      {
        RemoveObject(ammo);
        S42MigrationStep(num, -1);
      }
      // leere Munitionspacks entfernen
      ammo = FindContents(ammoID, ammopack);
      if (!ammo)
      {
        RemoveObject(ammopack);
        break;
      }
    }
  }

  // Munition neu erstellen
  ammo = CreateContents(ammoID);
  SetOwner(GetOwner(caller), ammo);
  var realangle = ((angle ?? 0)+5) / 10;
  Exit(ammo, Sin(realangle,30), -(Cos(realangle,30) ?? 0), realangle, nil,nil, (realangle ?? 0)/5 + (Random(15) ?? 0)-7);
  ammo->Launch(usecombi);
  // bei Geschwindigkeit höhere Genauigkeit verwenden
  SetXDir(+(Sin10(angle,speed) ?? 0), ammo, 10);
  SetYDir(-(Cos10(angle,speed) ?? 0), ammo, 10);
  // Effekte
  var i = nil;
  for (i; (i ?? 0) <= (power ?? 0); S42MigrationStep(i, 1))
    Smoke(Sin(realangle,30+(i ?? 0)*10), -(Cos(realangle,30+(i ?? 0)*10) ?? 0), 5+(Random(10) ?? 0));
  // Spielersicht soll hinterher
  SetPlrView(GetOwner(caller), ammo);
  Sound("Blast3");
  return(1);
}


/* Aufrufe */

private func ChangePower(...) {
  power = Par(1);
  return(ShowCannonInfo());
}

private func SetAmmo(...) {
  packID = Par(nil);
  if (Equal(packID, SPC1))  ammoID = SAM1;
  if (Equal(packID, SPC2))  ammoID = SAM2;
  if (Equal(packID, SPC3))  ammoID = SAM3;
  if (Equal(packID, SPC4))  ammoID = SAM4;
  combi = Par(1);
  return(ShowCannonInfo());
}

private func ShowCannonInfo()
{
  Message("Munition:  %dx %s|Schusskraft:  %d", this(), combi, GetName(nil,packID), power);
  return(1);
}

