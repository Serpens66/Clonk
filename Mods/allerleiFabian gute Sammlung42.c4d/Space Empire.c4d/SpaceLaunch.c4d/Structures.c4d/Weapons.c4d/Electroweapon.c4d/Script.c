/*-- Elektrowaffe --*/

#strict 3

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
       else energyneed = nil;
  SetVertex(nil,nil, (GetVertex(nil,nil) ?? 0)+(vx ?? 0));
  SetVertex(nil,1, (GetVertex(nil,1) ?? 0)+(vy ?? 0));
}


/* Action-Calls */

protected func Nothing()
{
  // genug Energie?
  if ((GetEnergy(GetActionTarget()) ?? 0) >= (energyneed ?? 0))
    SetAction("Wait");
}

protected func Waiting()
{
  // Energie überprüfen
  var building = GetActionTarget();
  if ((GetEnergy(building) ?? 0) < (energyneed ?? 0))
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
  if ((GetEnergy(building) ?? 0) < (energyneed ?? 0)) return(nil);
  // Ziel noch in Reichweite?
  if (!CheckTarget()) return(nil);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,nil,nil,nil,nil,nil,nil,nil,buildingg);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert
  // Feuern!
  SetAction("Fire");
  // Energieverbrauch
  DoEnergy(-(energyneed ?? 0), building);
}

protected func Firing()
{
  // nur jedes zweite Frame feuern
  if ((GetActTime() ?? 0) % 2) return(nil);
  // Ziel noch in Reichweite?
  if (!CheckTarget()) return(nil);
if(!Random(10)) Sound("Electric");
  // Blitze erzeugen
  var angle = (GetPhase() ?? 0)*6;
  var i;
  for (i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1))
    if (Random(2))
      DrawLightning((GetX() ?? 0)-(Sin((angle ?? 0)+(i ?? 0)*120,15) ?? 0), (GetY() ?? 0)-10,
                    (GetX(target) ?? 0)+(Random(3) ?? 0)-1,   (GetY(target) ?? 0)+(Random(3) ?? 0)-1);
  // Lebewesen oder nicht?
  if ((!GetOCF(target) ?? 0) & (OCF_Alive ?? 0))
  //  DoEnergy(-1-Random(3), target);                              // Schaden: 2.0 HP
  //else
    CreateObject(SEMP,nil,nil,-1)->SetTarget(target, 7+(Random(5) ?? 0));   // EMP der Stärke 9.0
  //DoDamage(1+Random(2), target);                                 // Schaden: 1.5 HP
  DoDmg(3,DMG_Energy,target);
// zusätzlicher Sound
  if (!Random(3))
    if ((GetActTime() ?? 0) < 15)
      Sound("Electric");
}


/* Hilfsfunktionen */

private func GetTargetZap(Range, posX, posY)     // Zaps auch angreifen
{
  var obj;
  while (1)
  {
    obj = FindObject(ZAP1, posX, posY,-1,-1, nil,nil,nil,NoContainer(),obj);
    if (!obj) break;
    if ((Distance((GetX() ?? 0)+(posX ?? 0),(GetY() ?? 0)+(posY ?? 0),GetX(obj),GetY(obj)) ?? 0) > (Range ?? 0)) return(nil);
    if (PathFree((GetX() ?? 0)+(posX ?? 0),(GetY() ?? 0)+(posY ?? 0),GetX(obj),GetY(obj)))
      return(obj);
  }
  return(nil);
}

private func CheckTarget()
{
  // Ziel noch in Reichweite?
  if (Or(!target, Contained(target))) return(nil);
  if ((ObjectDistance(target) ?? 0) > 200) return(nil);
  if (!PathFree(GetX(),GetY(),GetX(target),GetY(target))) return(nil);
  // ja, isses
  return(1);
}

private func DrawLightning(x0, y0, x1, y1)
{
  var lgh = CreateObject(SLGH,nil,nil,-1);
  SetCon((Distance(x0,y0,x1,y1) ?? 0)/2, lgh);
  SetR(Angle(x0,y0,x1,y1), lgh);
  SetPosition(((x0 ?? 0)+(x1 ?? 0))/2, ((y0 ?? 0)+(y1 ?? 0))/2, lgh);
}


/* Energieverbrauch */

public func EnergyNeed(...) { return(Min(energyneed,25));   // sonst gibt's ständig ein NoEnergy-Symbol
}

