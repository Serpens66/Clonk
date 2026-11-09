/*-- Artilleriegeschütz --*/

#strict

local angle, marker, power;       // angle = Ausrichtung in 1/10 Grad
local ammoID;


/* Initialisierung */

public func Init(building)
{
  angle = 0;
  power = 3;
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
  CreateMenu(GetID(),caller);
  var building = GetActionTarget();
  // alle Objekte im Gebäude durchsuchen
  var objID, count;
  var obj = FindObject(0, 0,0,0,0, OCF_Collectible(),"",0, building);
  while (1)
  {
    // alle Objekttypen
    objID = GetID(obj);
    if (!obj || !objID) break;
    count = 1;
    while (1)
    {
      // alle Objekte
      obj = FindObject(0, 0,0,0,0, OCF_Collectible(),"",0, building, obj);
      if (!obj) break;
      if (GetID(obj) == objID) ++count;
                          else break;
    }
    AddMenuItem("Munition: %s", "SetAmmo", objID, caller, count);
  }
  // den Clonk auch hinzufügen
 if(GetID(caller)==HZCK) AddMenuItem(Format("%s verschiessen", GetName(caller)), "SetAmmo", HZCK, caller);
  if(GetID(caller)==CLNK) AddMenuItem(Format("%s verschiessen", GetName(caller)), "SetAmmo", CLNK, caller);
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
  var gunpowder = CreateContents(GUNP,0,1); //FindContents(GUNP);
  if (!gunpowder)
      gunpowder = FindContents(GUNP, GetActionTarget());

  // Feuern!
  if (ammo && gunpowder)
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

private func Fire(ammo, caller)
{
  // Objekt bzw. Clonk abfeuern
  var speed = 60 + power*30 + Random(20);
  var energy = speed * GetMass(ammo) / 20;
  if (!ObjectCall(GetActionTarget(), "UseEnergy", energy))
  {
    Message("nicht genug|Energie!", this());
    Sound("Error");
    return(0);
  }
  // Clonk oder Munition?
  if (GetID(ammo) == HZCK || GetID(ammo) == CLNK)
  {
    ObjectSetAction(ammo, "Tumble");
    if (Contained(ammo) == this())
      Exiting(ammo);
    ammoID = 0;      // Clonks nicht dauerhaft als Munnition verwenden
  }
  else
    SetOwner(GetOwner(caller), ammo);
  var realangle = (angle+5) / 10;
  Exit(ammo, Sin(realangle,30), -Cos(realangle,30), realangle, 0,0, realangle/5 + Random(15)-7);
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
  ammoID = Par(0);
  return(ShowCannonInfo());

private func ShowCannonInfo()
{
  Message("Munition:  %s|Schusskraft:  %d", this(), GetName(0,ammoID), power);
  return(1);
}

