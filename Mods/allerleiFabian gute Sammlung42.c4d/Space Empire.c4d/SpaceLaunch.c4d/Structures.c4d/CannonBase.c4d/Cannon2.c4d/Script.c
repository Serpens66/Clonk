/*-- Bazookageschütz --*/

#strict

local angle, marker, power;       // angle = Ausrichtung in 1/10 Grad
local ammoID, packID, combi;


/* Initialisierung */

public func Init(building)
{
  angle = 0;
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
  if (GetDamage(building) >= building->GetMaxDamage())
    building = 0;
  // Inhalt ins Gebäude oder wegschleudern
  var obj;
  while (obj = Contents())
  {
    Exiting(obj);
    if (building) Enter(building, obj);
             else Exit(obj,0,0,Random(360), Random(9)-4, -Random(5), Random(51)-25);
  }
  return(1);
}


/* Steuerung */

protected ContainedLeft:
  Sound("Click");
  SetAction("TurnLeft");
  return(1);

protected ContainedLeftDouble:
  Sound("Click");
  SetAction("FastTurnLeft");
  return(1);

protected ContainedRight:
  Sound("Click");
  SetAction("TurnRight");
  return(1);

protected ContainedRightDouble:
  Sound("Click");
  SetAction("FastTurnRight");
  return(1);

protected ContainedUp:
  return(StopTurning());

protected func ContainedUpDouble(caller)
{
  [Schusskraft einstellen]
  CreateMenu(GetID(),caller);
  var i;
  for (i = 1; i <= 5; i++)
    AddMenuItem(Format("Schusskraft: %d", i), "ChangePower", SCBS, caller, 0,i, Format("Schusskraft auf Stufe %d einstellen", i), 2, i-1);
  SelectMenuItem(power-1, caller);
  return(1);
}

protected func ContainedDig(caller)
{
  [Munition wählen]
  CreateMenu(GetID(),caller,0,0,"Keine Munition im Gebäude");
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
  while (obj = FindObject(IDpack, 0,0,0,0, 0,0,0, GetActionTarget(), obj))
    num += ContentsCount(IDammo,obj); 
  if (num)      AddMenuItem("%s einfach",   "SetAmmo", IDpack, caller, 1,1);
  if (num > 1)  AddMenuItem("%s doppelt",   "SetAmmo", IDpack, caller, 2,2);
  if (num > 2)  AddMenuItem("%s dreifach",  "SetAmmo", IDpack, caller, 3,3);
  if (num > 3)  AddMenuItem("%s vierfach",  "SetAmmo", IDpack, caller, 4,4);
  if (num > 4)  AddMenuItem("%s fünffach",  "SetAmmo", IDpack, caller, 5,5);
  // weiter nur wenn Cheat aktiv
  if (!(fx_bitmask & 1024)) return(1);
  if (num > 5)  AddMenuItem("%s sechsfach", "SetAmmo", IDpack, caller, 6,6);
  if (num > 6)  AddMenuItem("%s siebenfach","SetAmmo", IDpack, caller, 7,7);
  if (num > 7)  AddMenuItem("%s achtfach",  "SetAmmo", IDpack, caller, 8,8);
  if (num > 8)  AddMenuItem("%s neunfach",  "SetAmmo", IDpack, caller, 9,9);
  if (num > 9)  AddMenuItem("%s zehnfach",  "SetAmmo", IDpack, caller, 10,10);
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
    marker = CreateObject(SCBM,0,0,GetOwner(caller));
    SetVisibility(VIS_Owner(), marker);
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

protected RejectCollect: return(1);    // Clonks dürfen nichts hineinlegen


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
  angle -= 2;
  // am Ende angekommen?
  if (angle <= -900) StopTurning();
                else ResetPhase();
  return(1);
}

private func FastTurningLeft()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 6))
    return(StopTurning());
  // weiterdrehen
  angle -= 10;
  // am Ende angekommen?
  if (angle <= -900) StopTurning();
                else ResetPhase();
  return(1);
}

private func TurningRight()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 1))
    return(StopTurning());
  // weiterdrehen
  angle += 2;
  // am Ende angekommen?
  if (angle >= +900) StopTurning();
                else ResetPhase();
  return(1);
}

private func FastTurningRight()
{
  if (!ObjectCall(GetActionTarget(), "UseEnergy", 6))
    return(StopTurning());
  // weiterdrehen
  angle += 10;
  // am Ende angekommen?
  if (angle >= +900) StopTurning();
                else ResetPhase();
  return(1);
}


/* Ausrichtung der Kanone */

private func ResetPhase(nomarker)
{
  angle = BoundBy(angle, -900, 900);
  // Phase neu bestimmen
  var phase;
  if (angle >= 0)  phase = 45 + (+angle+10) / 20;
             else  phase = 45 - (-angle+10) / 20;
  // und setzen
  SetPhase(phase);
  // Fadenkreuz auch neu setzen
  if (marker && !nomarker)
  {
    marker->SetAngle(angle, this());
    SetPlrView(GetController(), marker);
    if (angle >= 0) Message("+%d,%d°", marker,    angle/10,    angle % 10);
               else Message("-%d,%d°", marker, (-angle)/10, (-angle) % 10);
  }
  return(1);
}

protected func StopTurning()
{
  // Drehung anhalten
  if (GetAction() ne "Wait")
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
  while (ammopack = FindObject(packID, 0,0,0,0, 0,"",0, GetActionTarget(), ammopack))
    num += ContentsCount(ammoID, ammopack); 
  var usecombi = Min(num, combi);
  // keine Munition da?
  if (!num)
  {
    Sound("Error");
    return(ContainedDig(caller));
  }
  // Energie überprüfen
  var speed = 60 + power*30 + Random(20);
  var energy = speed * usecombi / 3;
  if (!ObjectCall(GetActionTarget(), "UseEnergy", energy))
  {
    Message("nicht genug|Energie!", this());
    Sound("Error");
    return(0);
  }
  // entsprechend viel Munition verbrauchen
  var ammo;
  ammopack = 0;
  num = usecombi;
  while (num)
  {
    ammopack = FindObject(packID, 0,0,0,0, 0,0,0,GetActionTarget(), ammopack);
    if (!ammopack) break;
    ammo = FindContents(ammoID, ammopack);
    while (num)
    {
      // Munition löschen
      if (ammo)
      {
        RemoveObject(ammo);
        --num;
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
  var realangle = (angle+5) / 10;
  Exit(ammo, Sin(realangle,30), -Cos(realangle,30), realangle, 0,0, realangle/5 + Random(15)-7);
  ammo->Launch(usecombi);
  // bei Geschwindigkeit höhere Genauigkeit verwenden
  SetXDir(+Sin10(angle,speed), ammo, 10);
  SetYDir(-Cos10(angle,speed), ammo, 10);
  // Effekte
  var i = 0;
  for (i; i <= power; ++i)
    Smoke(Sin(realangle,30+i*10), -Cos(realangle,30+i*10), 5+Random(10));
  // Spielersicht soll hinterher
  SetPlrView(GetOwner(caller), ammo);
  Sound("Blast3");
  return(1);
}


/* Aufrufe */

private ChangePower:
  power = Par(1);
  return(ShowCannonInfo());

private SetAmmo:
  packID = Par(0);
  if (packID == SPC1)  ammoID = SAM1;
  if (packID == SPC2)  ammoID = SAM2;
  if (packID == SPC3)  ammoID = SAM3;
  if (packID == SPC4)  ammoID = SAM4;
  combi = Par(1);
  return(ShowCannonInfo());

private func ShowCannonInfo()
{
  Message("Munition:  %dx %s|Schusskraft:  %d", this(), combi, GetName(0,packID), power);
  return(1);
}

