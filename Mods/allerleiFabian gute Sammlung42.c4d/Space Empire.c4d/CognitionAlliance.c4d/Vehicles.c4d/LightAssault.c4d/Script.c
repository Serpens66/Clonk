/*-- leichter Jäger --*/

#strict

public func IsHazardShip()  { return(true); }

local lander, stopper, flame, weapon, ready;

protected func Initialize()
{
  SetAction("GearDown");
  lander = CreateObject(VEHL,0,0,GetOwner());
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
  if (GetY() < -25)
  {
    SetPosition(GetX(),-25);
    if (GetYDir() < 0) SetYDir(0);
  }
  // kein Pilot da?
  if (!FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this()))
    if (!FindObject(SRCT,0,0,0,0,CrewMember(),"Attach",this()))
      if (!GBackLiquid())
        SetComDir(COMD_Down());
  // nicht rückwärts fliegen
  if (GetAction() eq "Hover")
  {
    if ((GetXDir() > 0) && (GetDir()==DIR_Left()))  SetAction("Rotate");
    if ((GetXDir() < 0) && (GetDir()==DIR_Right())) SetAction("Rotate");
  }
  return(1);
}

private func Thrust()
{
  // Beschleunigen
  var speed = Min(GetActTime()+20,150);
  if (GetDir()==DIR_Left())
    speed = -speed;
  SetXDir(speed);
  return(1);
}

// Türsteuerung

private func OpenEntrance() { return(SetEntrance(1)); }
private func CloseEntrance() { return(SetEntrance(0)); }

private func SoundOpenDoor()
{ 
  Sound("SteelGate2");
  ShowInfoBar(IBRS,this(),GetMaxDamage()-GetDamage(),GetMaxDamage(),5);
  return(1);
}

private func SoundCloseDoor()
{ 
  Sound("SteelGate2");
  // Außer einem Hazardclonk nichts mitnehmen
  var num = -1;
  var obj;
  while (obj = Contents(++num))
    {
      if (!(GetOCF(obj) & OCF_CrewMember()))
        SetCommand(obj,"Exit");
      else if (ContentsCount(GetID(obj)) >= 2) 
        for (var i=0;i<(ContentsCount(GetID(obj))-1);i++) SetCommand(FindContents(GetID(obj)),"Exit");
    }
  return(1);
}

public func ActivateEntrance()
{
  // nur einen Piloten reinlassen
  if ((Contained(Par(0)) != this()) && (ContentsCount(0,this())))
    {
      Message("Leichter Jäger ist schon besetzt",this());
      return(0);
    }
  // nur ein/aussteigen, wenn gelandet
  if (GetAction() eq "Stand") SetAction("OpenWindow");
  return(1);
}

/* Platzhalter im Clonk */

protected func Collection2(clonk)
{
  // Platzhalter erzeugen
  if (GetID(clonk) == HZCK)
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
  return(0);
}

// Action-Calls

protected func CheckDir()
{
  SetDir(1-GetDir());
  return(1);
}

private func StartSwimming()
{
  SetComDir(COMD_Stop());
  // Triebwerk aus
  if (flame) flame->SwitchOff();
  return(1);
}

// Steuerung

protected func ContainedLeftDouble()
{
  if (GetComDir() == COMD_Left()) ThrustersOn();
  return(1);
}

protected func ContainedRightDouble()
{
  if (GetComDir() == COMD_Right()) ThrustersOn();
  return(1);
}

protected func ContainedLeft()
{
  // im Wasser nichts tun
  if (GBackLiquid()) return(1);
  // Zugriff auf Inhalt
  if (IsStanding()) return(0);   
  // im Flug
  if (GetAction() eq "Flight")
    if (GetDir()==DIR_Right()) return(ThrustersOff());
                          else return(SetComDir(COMD_Left()));
  // nach links fliegen
  SetComDir(COMD_Left());
  if (GetAction() eq "Hover")
    if (GetDir()==DIR_Right()) SetAction("Rotate");
         else if (XDir2() < 0) ThrustersOn();
  return(1);
}

protected func ContainedRight()
{
  // im Wasser nichts tun
  if (GBackLiquid()) return(0);
  // Zugriff auf Inhalt
  if (IsStanding()) return(0);
  // im Flug
  if (GetAction() eq "Flight")
    if (GetDir()==DIR_Left()) return(ThrustersOff());
                          else return(SetComDir(COMD_Right()));
  // nach links fliegen
  SetComDir(COMD_Right());
  if (GetAction() eq "Hover")
    if (GetDir()==DIR_Left()) SetAction("Rotate");
       else if (XDir2() > 0) ThrustersOn();
  return(1);
}

protected func ContainedUp()
{
  SetComDir(COMD_Up());
  // Starten
  if (GetAction() eq "Stand") SetAction("GearUp");
  if (GetAction() eq "Swim")  SetAction("SwimStart");
  if (GetAction() eq "GearDown")
  {
    var ph = GetPhase();
    SetAction("GearUp");
    SetPhase(ph);
  }
  return(1);
}

protected func ContainedUpDouble()
{  
  if (GetAction() eq "Hover") {
    // umdrehen, falls nötig
    if ((GetXDir() > 0) && (GetDir()==DIR_Left()))  return(SetAction("Rotate"));
    if ((GetXDir() < 0) && (GetDir()==DIR_Right())) return(SetAction("Rotate"));
    // Immelmann beginnen
    SetAction("ImmelPhase1");
    // neue Richtung einstellen
    if (GetDir() == DIR_Left()) return(SetComDir(COMD_Right()));
    if (GetDir() == DIR_Right()) return(SetComDir(COMD_Left()));
  }
  return(1);
}

protected func ContainedDown()
{
  // im Wasser nichts tun
  if (GBackLiquid()) return(1);
  // Benutzer-Command löschen, falls nötig 
  if (!(GetAction() eq "Stand")) {
    var controller=FindObject(0,0,0,0,0,OCF_CrewMember(),0,0,this());
    if (controller) SetCommand(controller,"None");
  }
  // nach unten
  SetComDir(COMD_Down());
  return(1);
}

protected func ContainedDig()
{
  SetComDir(COMD_Stop());
  if ((GetAction() eq "Hover") || (GetAction() eq "Rotate") || (GetAction() eq "Flight"))
    return(StopEngines());
  return(1);
}

protected func ContainedThrow()
{
  if ((!ready) || (!GetWeapon(1))) return(Sound("Error"));
  if ((GetAction() eq "Hover") || (GetAction() eq "Flight")) {
    var ammo;
    if (weapon == CWA1) ammo=CreateObject(C_S6,GetDir()*128-64,3,GetOwner());
    if (weapon == CWA2) ammo=CreateObject(C_S7,GetDir()*128-64,3,GetOwner());
    ammo->Launch(GetDir());
    ready=0; 
  }
  return(1);
}

// Hilfsfunktionen

protected func StopEngines()
{
  // Shuttle durch Hilfsobjekt stoppen
  if (GetAction() eq "Flight")
    ThrustersOff();
  stopper = CreateObject(VEHS);
  stopper->~Init(this());
  return(1);
}

protected func ThrustersOn()
{
  // Action ändern
  if (GetAction() ne "Hover") return(1);
  SetAction("Flight");
  // Triebwerksflamme an
  if (GetDir()==DIR_Right())
  {
    flame = CreateObject(CAH1,-57,+2,GetOwner());
    if (flame) {
      flame->~SetOwnerObject(this());
      SetVertex(0,0,30,flame);
      SetDir(DIR_Right(), flame);
    }
  }
  else
  {
    flame = CreateObject(CAH1,+57,+2,GetOwner());
    if (flame) {
      flame->~SetOwnerObject(this());
      SetVertex(0,0,-75,flame);
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
      return(0);
  return(GetXDir());
}

public func DoLand()
{
  // nicht bei EMP-Einwirkung
  if (HasEMP()) return(0);
  // nicht bei Flugrichtung nach oben
  if ((GetComDir()==COMD_Up())
   || (GetComDir()==COMD_UpLeft())
   || (GetComDir()==COMD_UpRight())) return(1);
  // Landen möglich?
  if (GetAction() eq "Hover")
    SetAction("GearDown");
}


private func IsStanding()
{
  if ((GetAction() eq "Stand") || (GetAction() eq "OpenWindow")|| (GetAction() eq "GearUp")
   || (GetAction() eq "WindowOpen") || (GetAction() eq "CloseWindow")) return(1);
  return(0);
}

/* Kontakt */

protected func ContactLeft()
{
  if (GetAction() eq "Flight") ThrustersOff();
  return(0);
}

protected func ContactRight()
{
  if (GetAction() eq "Flight") ThrustersOff();
  return(0);
}

protected func Damage(amount)
{
  // Info-Symbol
  if (amount > 0)
    if (GetPlayerParty(GetOwner()) == 1) CreateObject(CE10,0,0,GetOwner());
                                    else CreateObject(SE10,0,0,GetOwner());
  // zu hoher Schaden führt zur Zerstörung
  if (GetDamage() >= GetMaxDamage())
    return(Destroy());
  // Anzeige
  var max = GetMaxDamage();
  ShowInfoBar(IBRS, this(), max-GetDamage(), max, 5, -1);
  return(1);
}

public func Destroy()
{
  // Info-Symbol
  if (GetPlayerParty(GetOwner()) == 1) CreateObject(CE11,0,0,GetOwner());
                                  else CreateObject(SE11,0,0,GetOwner());
  // Inhalt rausschleudern
  var obj;
  while (obj = Contents())
    Exit(obj,0,0,Random(360), Random(9)-4, -Random(5), Random(51)-25);
  // Zerstörung durch Explosion
  var i = GetMaxDamage() / 8;
  while (i)
  {
    obj = CreateObject(SFRX, Random(GetObjWidth() * 2/3) - GetObjWidth() / 3,
                             Random(GetObjHeight() * 2/3) - GetObjHeight() / 3, GetOwner());
    obj->Settings(i*3/2, 25, 1);
    --i;
  }
  // Explosion
  obj = CreateObject(SFRX, 0,0,GetOwner());
  obj->Settings(GetMaxDamage()/10, 50);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(25); else FakeExplode(25,0,0,25);
  return(1);
}

/* EMP-Einwirkung */

public EMP_Reaction:
  // nur auf Start-Aufruf reagieren
  if (Par(0)) return(0);
  // abstürzen lassen, wenn im Flug
  if ((GetAction() eq "Hover") || (GetAction() eq "Flight"))
    SetAction("Swim");
  return(1);

private HasEMP: return(FindObject(SEMP, 0,0,0,0, 0,"Active",this()));


protected func Hit()
{
  Sound("ShuttleHit");
  return(1);
}


/* Bewaffnung */

public func SetWeapon(num, wea)
{
  if (num == 1)  weapon = wea;
}

public func GetWeapon(num)
{
  if (num == 1)  return(weapon);
  return(0);
}

public GetSlotCount: return(1);

public func GetSlotInfo(slot, index)
{
  // es gibt nur Slot 1
  if (slot != 1) return(0);
  // alle Waffensysteme auflisten
  if (index==0) return(CWA1);
  if (index==1) return(CWA2);
  return(0);
}


/* Eigenschaften */

public InfoBarPos:  return(-16);
public InfoBarSize: return(5);
public GetMaxDamage: return(300);
public Repairable: return(1);
public IsVehicleProduct: return(1);

