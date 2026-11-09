/*-- Laserturm --*/

#strict 3
#include BOB2

local basement, manager;
local target, mode, cannon,buildingg, aus,fertig1,fertig2;

// außer Energiezelle nichts aufnehmen
protected func RejectCollect(def,pObj) { if(And(And(!Equal(def, ENAP), !Equal(def, CLNK)), !Equal(def, CLNK))) return(1); }

func Zumachen() 
{
  if(Equal(zuu, 1)) SetEntrance(nil);
  if(Equal(zuu, nil)) SetEntrance(1);
return(nil);
}


func Munitionsverbrauch(zahl)
{
if(And(Equal(zahl, 1), !(Random(2))) )return(1);
if(Equal(zahl, 2))return(2);
return(nil);
} // regelt den Munitionsverbrauch pro Salve


/* Fundament */

protected func Initialize()
{
  buildingg = this();
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(1,1);
  SetAction("Wait0");
  // Animation
  CreateObject(SLTS,nil,+9,GetOwner())->Init(this());
SetEntrance(1);
}

protected func Construction()
{
  basement = CreateObject(BS15,nil,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  var scanner = FindObject(SLTS,nil,nil,nil,nil,nil,"Active",this());
  if (!scanner)
      scanner = FindObject(SLTS,nil,nil,nil,nil,nil,"Wait",this());
  if (scanner) RemoveObject(scanner);
  if (manager) RemoveObject(manager);
  inherited();
}


/* Steuerung */

/*
protected func ControlThrow(clonk,a,b,c,d,f)
{
if(zuu==1) {PlayerMessage(GetOwner(clonk),"Zugriff verschlossen",clonk);return(1);}
  return(_inherited(clonk,a,b,c,d,f));
}

protected func ControlUp(clonk)
{
  [Reichweite]
  var i, marker;
  for (i = 0; i < 36; i++)
  {
    marker = CreateObject(SMK1, Cos(i*10,280), -28+Sin(i*10,280), GetOwner(clonk));
    marker->SetTimer(5);
  }
  PlayerMessage(GetOwner(),"Reichweite wird angezeigt",this());
  return(1);
}

protected func ControlDigDouble(clonk)
{
  [Modus wechseln]
  if (GetAction() eq "Wait1")  return(DoChange(1));
  if (GetAction() eq "Wait2")  return(DoChange(2));
  return(1);
}

protected func ControlUpDouble(clonk,zahl) //zahl nur für sensoren wichtig
{
if(zahl==1 && fertig1==1) return(0); // um weitere Prüfung zu ersparen.
if(zahl==2 && fertig2==1) return(0); 

// an-/ausschalten
if(!zahl){
if(aus==0) {aus=1;PlayerMessage(GetOwner(),"Laserturm ausgeschaltet",this());return(1);}
if(aus==1) {aus=0;PlayerMessage(GetOwner(),"Laserturm eingeschaltet",this());return(1);}
  return(1);}

//sensoren:
if(zahl==1 && fertig1==0) {aus=0;fertig1=1;fertig2=0;PlayerMessage(GetOwner(),"Laserturm eingeschaltet",this());return(1);}
if(zahl==2 && fertig2==0) {aus=1;fertig2=1;fertig1=0;PlayerMessage(GetOwner(),"Laserturm ausgeschaltet",this());return(1);}
return(1);
}
*/




protected func ContainedUp(clonk)
{
  [Reichweite]
  var i, marker;
  for (i = nil; (i ?? 0) < 36; S42MigrationStep(i, 1, true))
  {
    marker = CreateObject(SMK1, Cos((i ?? 0)*10,280), -28+(Sin((i ?? 0)*10,280) ?? 0), GetOwner(clonk));
    marker->SetTimer(5);
  }
  PlayerMessage(GetOwner(),"Reichweite wird angezeigt",this());
  return(1);
}

func ContainedDig(clonk)
{
  CreateMenu(SLTW, clonk, this(),nil,nil,nil,1);
AddMenuItem("Feuermodus wechseln", "ControlDigDouble", SLTW, clonk, nil, nil,"Nicht möglich, während der Turm gerade schießt. Zwei einzelne Laser, oder ein gebündelter Laser, der Ziele in Brand stecken kann.");
AddMenuItem("Ein-/Ausschalten", "ControlUpDouble", SLTW, clonk, nil, nil,"Ein-/Ausschalten um eventuell Munition zu sparen");

}

protected func ControlDigDouble(idImage)
{
  [Modus wechseln]
  if (!!SEqual(GetAction(), "Wait1"))  return(DoChange(1));
  if (!!SEqual(GetAction(), "Wait2"))  return(DoChange(2));
  return(1);
}

protected func ControlUpDouble(idImage,zahl) //zahl nur für sensoren wichtig
{
if(And(Equal(zahl, 1), Equal(fertig1, 1))) return(nil); // um weitere Prüfung zu ersparen.
if(And(Equal(zahl, 2), Equal(fertig2, 1))) return(nil);

// an-/ausschalten
if(!zahl){
if(Equal(aus, nil)) {aus=1;PlayerMessage(GetOwner(),"Laserturm ausgeschaltet",this());return(1);}
if(Equal(aus, 1)) {aus=nil;PlayerMessage(GetOwner(),"Laserturm eingeschaltet",this());return(1);}
  return(1);}

//sensoren:
if(And(Equal(zahl, 1), Equal(fertig1, nil))) {aus=nil;fertig1=1;fertig2=nil;PlayerMessage(GetOwner(),"Laserturm eingeschaltet",this());return(1);}
if(And(Equal(zahl, 2), Equal(fertig2, nil))) {aus=1;fertig2=1;fertig1=nil;PlayerMessage(GetOwner(),"Laserturm ausgeschaltet",this());return(1);}
return(1);
}




private func DoChange(num)
{
  if ((GetEnergy() ?? 0) < 25)
    return(Sound("Error"));
  DoEnergy(-25);
  SetAction(Format("Change%d", num));
  mode = 2-(num ?? 0);
}


/* Action-Call */

protected func CheckOpen()
{
if(Equal(aus, 1)) return(nil);  // eingeschaltet?
  // Arme aus Startposition ausfahren, wenn genügend Energie
  if (EnoughEnergy())
    SetAction("Open");
}


/* Ziel suchen */

protected func FindTarget()
{
if(Equal(aus, 1)) {SetAction("Wait0");return(nil);}  // eingeschaltet?
  // Position der aktiven Kanone
  var wx = nil;
  var wy = -23;
  if (!mode)
  {
    if (cannon) wx = +32;
           else wx = -31;
    wy = -28;
  }
  // nach Zielen suchen & feuern
  var obj;
  if (!obj)  obj = GetTargetProjectile(300, wx, wy);
  if (!obj)  obj = GetTargetCrew(300, wx, wy);
  if (!obj)  obj = GetTargetHostileDetonation(300, nil, wx, wy);
  //if (!obj)  obj = GetTargetAnimal(300, wx, wy);
  if (!obj)  obj = GetTargetZap(300, wx, wy);
  if (!obj)  obj = GetOtherThreats(300, wx, wy);
  // schiessen
  if (obj)
    return(GetReadyToFire(obj));
  // oder auch nicht
  cannon = !cannon;
  return(nil);
}

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


/* Feuern */

public func GetReadyToFire(par_target)
{
  // Energie da?
  if (!EnoughEnergy())
    return(nil);
  target = par_target;
  if (!!SEqual(GetAction(), "Wait1"))
  {
    CreateObject(SLTG)->Init(this(),cannon);
    return(SetAction("GetReady1"));
  }
  if (!!SEqual(GetAction(), "Wait2"))
  {
    CreateObject(SLTG)->Init(this(),2);
    return(SetAction("GetReady2"));
  }
  return(1);
}

public func Fire1()
{
  // Ziel noch da und in Reichweite?
  if (Or(!target, Contained(target)))
    return(nil);


// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,nil,nil,nil,nil,nil,nil,nil,buildingg);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
 if(munitionspack) munitionspack->~RestAmmoreduzieren(Munitionsverbrauch(1)); // restmunition im Paket wird um 1 pro Salve reduziert

  // Laserstrahl erzeugen
  DoEnergy(-10);
  var color = RGB(255,192+(Random(64) ?? 0),nil);
  if (cannon)
  {
    DrawLaser((GetX() ?? 0)+31, (GetY() ?? 0)-28, GetX(target), GetY(target), color);
    DrawLaser((GetX() ?? 0)+32, (GetY() ?? 0)-15, (GetX() ?? 0)+31, (GetY() ?? 0)-28, color);
  }
  else
  {
    DrawLaser((GetX() ?? 0)-32, (GetY() ?? 0)-28, GetX(target), GetY(target), color);
    DrawLaser((GetX() ?? 0)-33, (GetY() ?? 0)-15, (GetX() ?? 0)-32, (GetY() ?? 0)-28, color);
  }
  cannon = !cannon;
  Sound("LaserShot3");
  // Geschosse werden gleich zerstört
  if (target->~IsProjectile())
    return(target->~Destroy());
  // Zaps sofort killen
  if (Equal(GetID(target), ZAP1))
    return(RemoveObject(target));
  // alles andere beschädigen
  //DoDamage( 5+Random(3), target);
  //DoEnergy(-1-Random(3), target);     // Schaden: 6.0 HP
  DoDmg(2,DMG_Energy,target);
  return(1);  
}

public func Fire2()
{
  // Ziel noch da und in Reichweite?
  if (Or(!target, Contained(target)))
    return(nil);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,nil,nil,nil,nil,nil,nil,nil,buildingg);
if(And(!munitionspack, !FindObject(NOAM))) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
 if(munitionspack) munitionspack->~RestAmmoreduzieren(Munitionsverbrauch(2)); // restmunition im Paket wird um 1 pro Salve reduziert

  // Laserstrahl erzeugen
  DoEnergy(-25);
  DrawLaser(GetX(), (GetY() ?? 0)-23, GetX(target), GetY(target), RGB(255,nil,nil));
  DrawLaser((GetX() ?? 0)+14, (GetY() ?? 0)-18, GetX(),   (GetY() ?? 0)-23, RGB(255,nil,nil));
  DrawLaser((GetX() ?? 0)-15, (GetY() ?? 0)-18, (GetX() ?? 0)-1, (GetY() ?? 0)-23, RGB(255,nil,nil));
  Sound("LaserShot3");
  // Geschosse werden gleich zerstört
  if (target->~IsProjectile())
  {
    target->~Destroy();
    if (target)
      target->~Destroy();   // gebündelter Strahl wirkt doppelt (z.B. T-Flints)
    return(1);
  }
  // Zaps sofort killen
  if (Equal(GetID(target), ZAP1))
    return(RemoveObject(target));
  // alles andere beschädigen
  //DoDamage( 15+Random(4), target);
  //DoEnergy(-7-Random(4), target);     // Schaden: 16.5 HP
    DoDmg(5,DMG_Energy,target);
  if (!Random(6))
    if ((GetOCF(target) ?? 0) & (OCF_Inflammable ?? 0))
      Incinerate(target);             // gebündelter Strahl kann Ziel anzünden
  return(1);  
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


/* Energie */

public func EnoughEnergy(...) { return((GetEnergy() ?? 0) >= 25);
}
public func IsEnergyConsumer(...) { return(1);
}


/* Eigenschaften */

public func InfoBarSize(...) { return(3);
}
public func GetMaxDamage(...) { return(150);
}

