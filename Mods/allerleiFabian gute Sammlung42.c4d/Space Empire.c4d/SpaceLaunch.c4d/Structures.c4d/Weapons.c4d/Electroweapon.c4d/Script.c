/*-- Elektrowaffe --*/

#strict

local target;
local energyneed,buildingg;

func RejectGraphChange() {return(1);}

func Munitionsverbrauch(){return(3);}  // regelt den Munitionsverbrauch pro Salve


/* Initialisierung */

public func Init(building, mode, vx, vy)
{
buildingg= building;
  SetAction("Nothing", building);
  if (mode) energyneed = 60;
       else energyneed = 0;
  SetVertex(0,0, GetVertex(0,0)+vx);
  SetVertex(0,1, GetVertex(0,1)+vy);
}


/* Action-Calls */

protected func Nothing()
{
  // genug Energie?
  if (GetEnergy(GetActionTarget()) >= energyneed)
    SetAction("Wait");
}

protected func Waiting()
{
  // Energie überprüfen
  var building = GetActionTarget();
  if (GetEnergy(building) < energyneed)
    return(SetAction("Nothing"));
  // Ziel suchen
  target = GetTargetHostileDetonation(300);              // feindl Sprengsätze, Minen
  if (!target) target = GetTargetCrew(300);       // verfeindeter Clonk
  if (!target) target = GetTargetVehicle(300);    // gegnerisches Fahrzeug
  //if (!target) target = GetTargetAnimal(300);     // alle Tier
  if (!target) target = GetTargetZap(300);        // und schließlich Zaps
  if (!target) target = GetOtherThreats(300); // feindliche Raumgleiter, gefährliche tiere und aliens
  if (!target) return(1);
  // bereit machen
  SetAction("GetReady");
}

protected func Ready()
{
  // Energie überprüfen
  var building = GetActionTarget();
  if (GetEnergy(building) < energyneed) return(0);
  // Ziel noch in Reichweite?
  if (!CheckTarget()) return(0);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,0,0,0,0,0,0,0,buildingg);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert
  // Feuern!
  SetAction("Fire");
  // Energieverbrauch
  DoEnergy(-energyneed, building);
}

protected func Firing()
{
  // nur jedes zweite Frame feuern
  if (GetActTime() % 2) return(0);
  // Ziel noch in Reichweite?
  if (!CheckTarget()) return(0);
if(!Random(10)) Sound("Electric");
  // Blitze erzeugen
  var angle = GetPhase()*6;
  var i;
  for (i = 0; i < 3; ++i)
    if (Random(2))
      DrawLightning(GetX()-Sin(angle+i*120,15), GetY()-10,
                    GetX(target)+Random(3)-1,   GetY(target)+Random(3)-1);
  // Lebewesen oder nicht?
  if (!GetOCF(target) & OCF_Alive())
  //  DoEnergy(-1-Random(3), target);                              // Schaden: 2.0 HP
  //else
    CreateObject(SEMP,0,0,-1)->SetTarget(target, 7+Random(5));   // EMP der Stärke 9.0
  //DoDamage(1+Random(2), target);                                 // Schaden: 1.5 HP
  DoDmg(3,DMG_Energy,target);
// zusätzlicher Sound
  if (!Random(3))
    if (GetActTime() < 15)
      Sound("Electric");
}


/* Hilfsfunktionen */

private func GetTargetZap(Range, posX, posY)     // Zaps auch angreifen
{
  var obj;
  while (1)
  {
    obj = FindObject(ZAP1, posX, posY,-1,-1, 0,0,0,NoContainer(),obj);
    if (!obj) break;
    if (Distance(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)) > Range) return(0);
    if (PathFree(GetX()+posX,GetY()+posY,GetX(obj),GetY(obj)))
      return(obj);
  }
  return(0);
}

private func CheckTarget()
{
  // Ziel noch in Reichweite?
  if (!target || Contained(target)) return(0);
  if (ObjectDistance(target) > 200) return(0);
  if (!PathFree(GetX(),GetY(),GetX(target),GetY(target))) return(0);
  // ja, isses
  return(1);
}

private func DrawLightning(x0, y0, x1, y1)
{
  var lgh = CreateObject(SLGH,0,0,-1);
  SetCon(Distance(x0,y0,x1,y1)/2, lgh);
  SetR(Angle(x0,y0,x1,y1), lgh);
  SetPosition((x0+x1)/2, (y0+y1)/2, lgh);
}


/* Energieverbrauch */

public EnergyNeed: return(Min(energyneed,25));   // sonst gibt's ständig ein NoEnergy-Symbol

