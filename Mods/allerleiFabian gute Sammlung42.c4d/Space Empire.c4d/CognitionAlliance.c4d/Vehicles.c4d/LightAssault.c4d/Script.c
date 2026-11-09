/*-- leichter Jäger --*/

#strict 3

public func IsHazardShip()  { return(true); }

local lander, stopper, flame, weapon, ready;

protected func Initialize()
{
  SetAction("GearDown");
  lander = CreateObject(VEHL,nil,nil,GetOwner());
  lander->Init(this(),-7);
  ready=1;
  return(1);
}

protected func Destruction()
{
  if (lander)  RemoveObject(lander);
  if (stopper) RemoveObject(stopper);
  if (flame)   RemoveObject(flame);
  return(1);
}

// Timer-Call

protected func CheckStatus()
{
  // nachladen, wenn nötig
  if (!ready) ready=1;
  // nicht zu hoch fliegen
  if ((GetY() ?? 0) < -25)
  {
    SetPosition(GetX(),-25);
    if ((GetYDir() ?? 0) < 0) SetYDir(nil);
  }
  // kein Pilot da?
  if (!FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this()))
    if (!FindObject(SRCT,nil,nil,nil,nil,CrewMember(),"Attach",this()))
      if (!GBackLiquid())
        SetComDir(COMD_Down);
  // nicht rückwärts fliegen
  if (!!SEqual(GetAction(), "Hover"))
  {
    if (And(((GetXDir() ?? 0) > 0), (Equal(GetDir(), DIR_Left))))  SetAction("Rotate");
    if (And(((GetXDir() ?? 0) < 0), (Equal(GetDir(), DIR_Right)))) SetAction("Rotate");
  }
  return(1);
}

private func Thrust()
{
  // Beschleunigen
  var speed = Min((GetActTime() ?? 0)+20,150);
  if (Equal(GetDir(), DIR_Left))
    speed = -(speed ?? 0);
  SetXDir(speed);
  return(1);
}

// Türsteuerung

private func OpenEntrance() { return(SetEntrance(1)); }
private func CloseEntrance() { return(SetEntrance(nil)); }

private func SoundOpenDoor()
{ 
  Sound("SteelGate2");
  ShowInfoBar(IBRS,this(),(GetMaxDamage() ?? 0)-(GetDamage() ?? 0),GetMaxDamage(),5);
  return(1);
}

private func SoundCloseDoor()
{ 
  Sound("SteelGate2");
  // Außer einem Hazardclonk nichts mitnehmen
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    {
      if (!((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0)))
        SetCommand(obj,"Exit");
      else if ((ContentsCount(GetID(obj)) ?? 0) >= 2)
        for (var i=nil;(i ?? 0)<((ContentsCount(GetID(obj)) ?? 0)-1);S42MigrationStep(i, 1, true)) SetCommand(FindContents(GetID(obj)),"Exit");
    }
  return(1);
}

public func ActivateEntrance()
{
  // nur einen Piloten reinlassen
  if (And((!Equal(Contained(Par(nil)), this())), (ContentsCount(nil,this()))))
    {
      Message("Leichter Jäger ist schon besetzt",this());
      return(nil);
    }
  // nur ein/aussteigen, wenn gelandet
  if (!!SEqual(GetAction(), "Stand")) SetAction("OpenWindow");
  return(1);
}

/* Platzhalter im Clonk */

protected func Collection2(clonk)
{
  // Platzhalter erzeugen
  if (Equal(GetID(clonk), HZCK))
    CreateContents(SPCR, clonk);
  return(1);
}

protected func Ejection(clonk)
{
  // Platzhalter löschen
  var obj = FindContents(SPCR, clonk);
  if (obj) RemoveObject(obj);
  SetController(-1);
  return(1);
}

protected func RejectCollect(clonk)
{  
  // schon was im Jäger?
  if (FindContents()) return(SetCommand(clonk,"None"));
  // checken, ob da ein Clonk vor der Tür steht
  if (!clonk->~IsClonk()) return(SetCommand(clonk,"None"));
  // ansonsten reinlassen
  return(nil);
}

// Action-Calls

protected func CheckDir()
{
  SetDir(1-(GetDir() ?? 0));
  return(1);
}

private func StartSwimming()
{
  SetComDir(COMD_Stop);
  // Triebwerk aus
  if (flame) flame->SwitchOff();
  return(1);
}

// Steuerung

protected func ContainedLeftDouble()
{
  if (Equal(GetComDir(), COMD_Left)) ThrustersOn();
  return(1);
}

protected func ContainedRightDouble()
{
  if (Equal(GetComDir(), COMD_Right)) ThrustersOn();
  return(1);
}

protected func ContainedLeft()
{
  // im Wasser nichts tun
  if (GBackLiquid()) return(1);
  // Zugriff auf Inhalt
  if (IsStanding()) return(nil);
  // im Flug
  if (!!SEqual(GetAction(), "Flight"))
    if (Equal(GetDir(), DIR_Right)) return(ThrustersOff());
                          else return(SetComDir(COMD_Left));
  // nach links fliegen
  SetComDir(COMD_Left);
  if (!!SEqual(GetAction(), "Hover"))
    if (Equal(GetDir(), DIR_Right)) SetAction("Rotate");
         else if ((XDir2() ?? 0) < 0) ThrustersOn();
  return(1);
}

protected func ContainedRight()
{
  // im Wasser nichts tun
  if (GBackLiquid()) return(nil);
  // Zugriff auf Inhalt
  if (IsStanding()) return(nil);
  // im Flug
  if (!!SEqual(GetAction(), "Flight"))
    if (Equal(GetDir(), DIR_Left)) return(ThrustersOff());
                          else return(SetComDir(COMD_Right));
  // nach links fliegen
  SetComDir(COMD_Right);
  if (!!SEqual(GetAction(), "Hover"))
    if (Equal(GetDir(), DIR_Left)) SetAction("Rotate");
       else if ((XDir2() ?? 0) > 0) ThrustersOn();
  return(1);
}

protected func ContainedUp()
{
  SetComDir(COMD_Up);
  // Starten
  if (!!SEqual(GetAction(), "Stand")) SetAction("GearUp");
  if (!!SEqual(GetAction(), "Swim"))  SetAction("SwimStart");
  if (!!SEqual(GetAction(), "GearDown"))
  {
    var ph = GetPhase();
    SetAction("GearUp");
    SetPhase(ph);
  }
  return(1);
}

protected func ContainedUpDouble()
{  
  if (!!SEqual(GetAction(), "Hover")) {
    // umdrehen, falls nötig
    if (And(((GetXDir() ?? 0) > 0), (Equal(GetDir(), DIR_Left))))  return(SetAction("Rotate"));
    if (And(((GetXDir() ?? 0) < 0), (Equal(GetDir(), DIR_Right)))) return(SetAction("Rotate"));
    // Immelmann beginnen
    SetAction("ImmelPhase1");
    // neue Richtung einstellen
    if (Equal(GetDir(), DIR_Left)) return(SetComDir(COMD_Right));
    if (Equal(GetDir(), DIR_Right)) return(SetComDir(COMD_Left));
  }
  return(1);
}

protected func ContainedDown()
{
  // im Wasser nichts tun
  if (GBackLiquid()) return(1);
  // Benutzer-Command löschen, falls nötig 
  if (!(!!SEqual(GetAction(), "Stand"))) {
    var controller=FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,this());
    if (controller) SetCommand(controller,"None");
  }
  // nach unten
  SetComDir(COMD_Down);
  return(1);
}

protected func ContainedDig()
{
  SetComDir(COMD_Stop);
  if (Or(Or((!!SEqual(GetAction(), "Hover")), (!!SEqual(GetAction(), "Rotate"))), (!!SEqual(GetAction(), "Flight"))))
    return(StopEngines());
  return(1);
}

protected func ContainedThrow()
{
  if (Or((!ready), (!GetWeapon(1)))) return(Sound("Error"));
  if (Or((!!SEqual(GetAction(), "Hover")), (!!SEqual(GetAction(), "Flight")))) {
    var ammo;
    if (Equal(weapon, CWA1)) ammo=CreateObject(C_S6,(GetDir() ?? 0)*128-64,3,GetOwner());
    if (Equal(weapon, CWA2)) ammo=CreateObject(C_S7,(GetDir() ?? 0)*128-64,3,GetOwner());
    ammo->Launch(GetDir());
    ready=nil;
  }
  return(1);
}

// Hilfsfunktionen

protected func StopEngines()
{
  // Shuttle durch Hilfsobjekt stoppen
  if (!!SEqual(GetAction(), "Flight"))
    ThrustersOff();
  stopper = CreateObject(VEHS);
  stopper->~Init(this());
  return(1);
}

protected func ThrustersOn()
{
  // Action ändern
  if (!SEqual(GetAction(), "Hover")) return(1);
  SetAction("Flight");
  // Triebwerksflamme an
  if (Equal(GetDir(), DIR_Right))
  {
    flame = CreateObject(CAH1,-57,+2,GetOwner());
    if (flame) {
      flame->~SetOwnerObject(this());
      SetVertex(nil,nil,30,flame);
      SetDir(DIR_Right, flame);
    }
  }
  else
  {
    flame = CreateObject(CAH1,+57,+2,GetOwner());
    if (flame) {
      flame->~SetOwnerObject(this());
      SetVertex(nil,nil,-75,flame);
    }
  }
  return(1);
}

protected func ThrustersOff()
{
  // Triebwerksflamme aus
  if (flame) flame->~SwitchOff();
  SetAction("Hover");
  return(1);
}

private func XDir2()
{
  // Gibt XDir zurück, wenn nicht gestoppt wird
  if (stopper)
    if (stopper->~DoStopX())
      return(nil);
  return(GetXDir());
}

public func DoLand()
{
  // nicht bei EMP-Einwirkung
  if (HasEMP()) return(nil);
  // nicht bei Flugrichtung nach oben
  if (Or(Or((Equal(GetComDir(), COMD_Up))
   , (Equal(GetComDir(), COMD_UpLeft)))
   , (Equal(GetComDir(), COMD_UpRight)))) return(1);
  // Landen möglich?
  if (!!SEqual(GetAction(), "Hover"))
    SetAction("GearDown");
}


private func IsStanding()
{
  if (Or(Or(Or(Or((!!SEqual(GetAction(), "Stand")), (!!SEqual(GetAction(), "OpenWindow"))), (!!SEqual(GetAction(), "GearUp")))
   , (!!SEqual(GetAction(), "WindowOpen"))), (!!SEqual(GetAction(), "CloseWindow")))) return(1);
  return(nil);
}

/* Kontakt */

protected func ContactLeft()
{
  if (!!SEqual(GetAction(), "Flight")) ThrustersOff();
  return(nil);
}

protected func ContactRight()
{
  if (!!SEqual(GetAction(), "Flight")) ThrustersOff();
  return(nil);
}

protected func Damage(amount)
{
  // Info-Symbol
  if ((amount ?? 0) > 0)
    if (Equal(GetPlayerParty(GetOwner()), 1)) CreateObject(CE10,nil,nil,GetOwner());
                                    else CreateObject(SE10,nil,nil,GetOwner());
  // zu hoher Schaden führt zur Zerstörung
  if ((GetDamage() ?? 0) >= (GetMaxDamage() ?? 0))
    return(Destroy());
  // Anzeige
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), (max ?? 0)-(GetDamage() ?? 0), max, 5, -1);
  return(1);
}

public func Destroy()
{
  // Info-Symbol
  if (Equal(GetPlayerParty(GetOwner()), 1)) CreateObject(CE11,nil,nil,GetOwner());
                                  else CreateObject(SE11,nil,nil,GetOwner());
  // Inhalt rausschleudern
  var obj;
  while (obj = Contents())
    Exit(obj,nil,nil,Random(360), (Random(9) ?? 0)-4, -(Random(5) ?? 0), (Random(51) ?? 0)-25);
  // Zerstörung durch Explosion
  var i = (GetMaxDamage() ?? 0) / 8;
  while (i)
  {
    obj = CreateObject(SFRX, (Random((GetObjWidth() ?? 0) * 2/3) ?? 0) - (GetObjWidth() ?? 0) / 3,
                             (Random((GetObjHeight() ?? 0) * 2/3) ?? 0) - (GetObjHeight() ?? 0) / 3, GetOwner());
    obj->Settings((i ?? 0)*3/2, 25, 1);
    S42MigrationStep(i, -1);
  }
  // Explosion
  obj = CreateObject(SFRX, nil,nil,GetOwner());
  obj->Settings((GetMaxDamage() ?? 0)/10, 50);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(25); else FakeExplode(25,nil,nil,25);
  return(1);
}

/* EMP-Einwirkung */

public func EMP_Reaction(...) {
  // nur auf Start-Aufruf reagieren
  if (Par(nil)) return(nil);
  // abstürzen lassen, wenn im Flug
  if (Or((!!SEqual(GetAction(), "Hover")), (!!SEqual(GetAction(), "Flight"))))
    SetAction("Swim");
  return(1);
}

private func HasEMP(...) { return(FindObject(SEMP, nil,nil,nil,nil, nil,"Active",this()));
}


protected func Hit()
{
  Sound("ShuttleHit");
  return(1);
}


/* Bewaffnung */

public func SetWeapon(num, wea)
{
  if (Equal(num, 1))  weapon = wea;
}

public func GetWeapon(num)
{
  if (Equal(num, 1))  return(weapon);
  return(nil);
}

public func GetSlotCount(...) { return(1);
}

public func GetSlotInfo(slot, index)
{
  // es gibt nur Slot 1
  if (!Equal(slot, 1)) return(nil);
  // alle Waffensysteme auflisten
  if (Equal(index, nil)) return(CWA1);
  if (Equal(index, 1)) return(CWA2);
  return(nil);
}


/* Eigenschaften */

public func InfoBarPos(...) {  return(-16);
}
public func InfoBarSize(...) { return(5);
}
public func GetMaxDamage(...) { return(300);
}
public func Repairable(...) { return(1);
}
public func IsVehicleProduct(...) { return(1);
}

