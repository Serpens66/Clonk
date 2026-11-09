/*-- Laserwaffe --*/

#strict

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
       else energyneed = 0;
  SetVertex(0,0, GetVertex(0,0)+vx);
  SetVertex(0,1, GetVertex(0,1)+vy);
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
  return(0);
}

private func GetTargetZap(Range)     // Zaps auch angreifen
{
  var obj;
  while (1)
  {
    obj = FindObject(ZAP1, 0,0,-1,-1, 0,0,0,NoContainer(),obj);
    if (!obj) break;
    if (ObjectDistance(obj) > Range) return(0);
    if (PathFree(GetX(),GetY(), GetX(obj),GetY(obj)))
      return(obj);
  }
  return(0);
}


/* Feuern */

public func GetReadyToFire(par_target)
{
  // genug Energie?
  if (GetEnergy(GetActionTarget()) < energyneed)
    return(0);
  target = par_target;
  CreateObject(SLSG)->Init(this());
  SetAction("GetReady");
}

public func Fire()
{
  // Ziel noch da und in Reichweite?
  if (!target || Contained(target))
    return(0);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,0,0,0,0,0,0,0,buildingg);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
if(munitionspack) 
 munitionspack->~RestAmmoreduzieren(Munitionsverbrauch()); // restmunition im Paket wird um 1 pro Salve reduziert

  // Laserstrahlen erzeugen
  DoEnergy(-10);
  var color = RGB(255,192+Random(64),0);
  DrawLaser(GetX()-12, GetY()-23, GetX(target), GetY(target), color);
  DrawLaser(GetX()+7, GetY()-23, GetX(target), GetY(target), color);
  DrawLaser(GetX()+8, GetY()-12, GetX()+7, GetY()-22, color);
  Sound("LaserShot3");
  // Geschosse werden gleich zerstört
  if (target->~IsProjectile())
    return(target->~Destroy());
  // Zaps sofort killen
  if (GetID(target) == ZAP1)
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
    if (dist < 50) break;
    // nächstes Stück zeichnen
    laserline = CreateObject(SLSR);
    SetPosition(x0, y0, laserline);
    SetColorDw(color, laserline);
    SetR(angle, laserline);
    // zum nächsten Punkt
    x0 += Sin(angle, dist);
    y0 -= Cos(angle, dist);
  }
  if (!dist) return(0);
  // Endstück
  laserline = CreateObject(SLSR);
  var mx = (x0 + x1 + (50 * (x0-x1) + dist/2) / dist + 1) / 2;
  var my = (y0 + y1 + (50 * (y0-y1) + dist/2) / dist + 1) / 2;
  SetPosition(mx, my, laserline);
  SetColorDw(color, laserline);
  SetR(angle, laserline);
  SetCon(50+dist, laserline);
}


/* Energieverbrauch */

public EnergyNeed: return(energyneed);

