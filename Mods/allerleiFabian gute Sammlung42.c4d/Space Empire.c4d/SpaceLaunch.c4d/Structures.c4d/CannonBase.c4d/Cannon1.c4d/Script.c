/*-- Artilleriegeschütz --*/

#strict 3

local angle, marker, power;       // angle = Ausrichtung in 1/10 Grad
local ammoID;


/* Initialisierung */

public func Init(building)
{
  angle = nil;
  power = 3;
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
  CreateMenu(GetID(),caller);
  var building = GetActionTarget();
  // alle Objekte im Gebäude durchsuchen
  var objID, count;
  var obj = FindObject(nil, nil,nil,nil,nil, OCF_Collectible,"",nil, building);
  while (1)
  {
    // alle Objekttypen
    objID = GetID(obj);
    if (Or(!obj, !objID)) break;
    count = 1;
    while (1)
    {
      // alle Objekte
      obj = FindObject(nil, nil,nil,nil,nil, OCF_Collectible,"",nil, building, obj);
      if (!obj) break;
      if (Equal(GetID(obj), objID)) S42MigrationStep(count, 1);
                          else break;
    }
    AddMenuItem("Munition: %s", "SetAmmo", objID, caller, count);
  }
  // den Clonk auch hinzufügen
 if(Equal(GetID(caller), HZCK)) AddMenuItem(Format("%s verschiessen", GetName(caller)), "SetAmmo", HZCK, caller);
  if(Equal(GetID(caller), CLNK)) AddMenuItem(Format("%s verschiessen", GetName(caller)), "SetAmmo", CLNK, caller);
  return(1);
}

protected func ContainedThrow(caller)
{
  [Feuern]
  // Munition suchen
  var ammo = FindContents(ammoID);
  if (!ammo)
      ammo = FindContents(ammoID, GetActionTarget());

//schiepßulver suchen .. erstmal wieder deaktiviert, indem schießpulver erstellt wird..
  var gunpowder = CreateContents(GUNP,nil,1); //FindContents(GUNP);
  if (!gunpowder)
      gunpowder = FindContents(GUNP, GetActionTarget());

  // Feuern!
  if (And(ammo, gunpowder))
    {Fire(ammo, caller);RemoveObject(gunpowder);}
  else
  {
    Sound("Error");PlayerMessage(GetOwner(caller),"Keine Munition gewählt oder kein Schießpulver",caller);
    ContainedDig(caller);   // Munitionsmenü
  }
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
  var clonk = FindContents(CLNK);
  var hazardclonk = FindContents(HZCK);
  if (clonk)
    ContainedDown(clonk);   // derjenige muss erst raus
if(hazardclonk)
    ContainedDown(hazardclonk);   // derjenige muss erst raus
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

private func Fire(ammo, caller)
{
  // Objekt bzw. Clonk abfeuern
  var speed = 60 + (power ?? 0)*30 + (Random(20) ?? 0);
  var energy = (speed ?? 0) * (GetMass(ammo) ?? 0) / 20;
  if (!ObjectCall(GetActionTarget(), "UseEnergy", energy))
  {
    Message("nicht genug|Energie!", this());
    Sound("Error");
    return(nil);
  }
  // Clonk oder Munition?
  if (Or(Equal(GetID(ammo), HZCK), Equal(GetID(ammo), CLNK)))
  {
    ObjectSetAction(ammo, "Tumble");
    if (Equal(Contained(ammo), this()))
      Exiting(ammo);
    ammoID = nil;      // Clonks nicht dauerhaft als Munnition verwenden
  }
  else
    SetOwner(GetOwner(caller), ammo);
  var realangle = ((angle ?? 0)+5) / 10;
  Exit(ammo, Sin(realangle,30), -(Cos(realangle,30) ?? 0), realangle, nil,nil, (realangle ?? 0)/5 + (Random(15) ?? 0)-7);
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
  ammoID = Par(nil);
  return(ShowCannonInfo());
}

private func ShowCannonInfo()
{
  Message("Munition:  %s|Schusskraft:  %d", this(), GetName(nil,ammoID), power);
  return(1);
}

