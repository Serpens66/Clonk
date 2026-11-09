/*-- Laserwaffe --*/

#strict 3

local target;
local energyneed,buildingg;

func RejectGraphChange() {return(1);}

func Munitionsverbrauch(){return(1);} // regelt den Munitionsverbrauch pro Salve

/* Object-Calls */

public func Init(building, mode, vx, vy)
{
buildingg=building;
  SetAction("Wait", building);
  if (mode) energyneed = 20;
       else energyneed = nil;
  SetVertex(nil,nil, (GetVertex(nil,nil) ?? 0)+(vx ?? 0));
  SetVertex(nil,1, (GetVertex(nil,1) ?? 0)+(vy ?? 0));
}


/* Ziel suchen */

protected func FindTarget()
{
  // nach Zielen suchen & feuern
  var obj;
  if (!obj)  obj = GetTargetHostileDetonation(300);              // feindl Sprengsätze, Minen
  if (!obj)  obj = GetTargetProjectile(350);
  if (!obj)  obj = GetTargetCrew(350);
  //if (!obj)  obj = GetTargetAnimal(350);
  if (!obj)  obj = GetTargetZap(350);
  if (!obj) obj = GetOtherThreats(350); // feindliche Raumgleiter, gefährliche tiere und aliens
  // schiessen
  if (obj)
    return(GetReadyToFire(obj));
  // oder auch nicht
  return(nil);
}

private func GetTargetZap(Range)     // Zaps auch angreifen
{
  var obj;
  while (1)
  {
    obj = FindObject(ZAP1, nil,nil,-1,-1, nil,nil,nil,NoContainer(),obj);
    if (!obj) break;
    if ((ObjectDistance(obj) ?? 0) > (Range ?? 0)) return(nil);
    if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
      return(obj);
  }
  return(nil);
}


/* Feuern */

public func GetReadyToFire(par_target)
{
  // genug Energie?
  if ((GetEnergy(GetActionTarget()) ?? 0) < (energyneed ?? 0))
    return(nil);
  target = par_target;
  CreateObject(SLSG)->Init(this());
  SetAction("GetReady");
}

public func Fire()
{
  // Ziel noch da und in Reichweite?
  if (Or(!target, Contained(target)))
    return(nil);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,nil,nil,nil,nil,nil,nil,nil,buildingg);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert

  // Laserstrahlen erzeugen
  DoEnergy(-10);
  var color = RGB(255,192+(Random(64) ?? 0),nil);
  DrawLaser((GetX() ?? 0)-12, (GetY() ?? 0)-23, GetX(target), GetY(target), color);
  DrawLaser((GetX() ?? 0)+7, (GetY() ?? 0)-23, GetX(target), GetY(target), color);
  DrawLaser((GetX() ?? 0)+8, (GetY() ?? 0)-12, (GetX() ?? 0)+7, (GetY() ?? 0)-22, color);
  Sound("LaserShot3");
  // Geschosse werden gleich zerstört
  if (target->~IsProjectile())
    return(target->~Destroy());
  // Zaps sofort killen
  if (Equal(GetID(target), ZAP1))
    return(RemoveObject(target));
  // alles andere beschädigen
  DoDmg(5,DMG_Energy,target);
 // DoDamage( 2+Random(5), target);
 // DoEnergy(-2-Random(5), target);     // Schaden: 4.0 HP
}



/* Laser zeichnen */

public func DrawLaser(x0, y0, x1, y1, color)
{
  var dist, angle;
  var laserline;
  // Endlosschleife
  while (1)
  {
    // Winkel und Entfernung
    angle = Angle(x0,y0,x1,y1);
    dist = Min(50, Distance(x0,y0,x1,y1));
    // geht noch ein ganzes Stück rein?
    if ((dist ?? 0) < 50) break;
    // nächstes Stück zeichnen
    laserline = CreateObject(SLSR);
    SetPosition(x0, y0, laserline);
    SetColorDw(color, laserline);
    SetR(angle, laserline);
    // zum nächsten Punkt
    S42MigrationAssign(x0, "+=", (Sin(angle, dist) ?? 0));
    S42MigrationAssign(y0, "-=", (Cos(angle, dist) ?? 0));
  }
  if (!dist) return(nil);
  // Endstück
  laserline = CreateObject(SLSR);
  var mx = ((x0 ?? 0) + (x1 ?? 0) + (50 * ((x0 ?? 0)-(x1 ?? 0)) + (dist ?? 0)/2) / (dist ?? 0) + 1) / 2;
  var my = ((y0 ?? 0) + (y1 ?? 0) + (50 * ((y0 ?? 0)-(y1 ?? 0)) + (dist ?? 0)/2) / (dist ?? 0) + 1) / 2;
  SetPosition(mx, my, laserline);
  SetColorDw(color, laserline);
  SetR(angle, laserline);
  SetCon(50+(dist ?? 0), laserline);
}


/* Energieverbrauch */

public func EnergyNeed(...) { return(energyneed);
}

