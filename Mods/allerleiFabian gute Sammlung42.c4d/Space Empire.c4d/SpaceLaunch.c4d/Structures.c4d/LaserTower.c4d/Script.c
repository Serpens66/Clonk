/*-- Laserturm --*/

#strict
#include BOB2

local basement, manager;
local target, mode, cannon,buildingg, aus,fertig1,fertig2;

// außer Energiezelle nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=ENAP && def!=CLNK && def!=CLNK) return(1); }

func Zumachen() 
{
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);
return(0);
}


func Munitionsverbrauch(zahl)
{
if(zahl==1 && !(Random(2)) )return(1);
if(zahl==2)return(2);
return(0);
} // regelt den Munitionsverbrauch pro Salve


/* Fundament */

protected func Initialize()
{
  buildingg = this();
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(1,1);
  SetAction("Wait0");
  // Animation
  CreateObject(SLTS,0,+9,GetOwner())->Init(this());
SetEntrance(1);
}

protected func Construction()
{
  basement = CreateObject(BS15,0,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  var scanner = FindObject(SLTS,0,0,0,0,0,"Active",this());
  if (!scanner)
      scanner = FindObject(SLTS,0,0,0,0,0,"Wait",this());
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
  for (i = 0; i < 36; i++)
  {
    marker = CreateObject(SMK1, Cos(i*10,280), -28+Sin(i*10,280), GetOwner(clonk));
    marker->SetTimer(5);
  }
  PlayerMessage(GetOwner(),"Reichweite wird angezeigt",this());
  return(1);
}

func ContainedDig(clonk)
{
  CreateMenu(SLTW, clonk, this(),0,0,0,1);
AddMenuItem("Feuermodus wechseln", "ControlDigDouble", SLTW, clonk, 0, 0,"Nicht möglich, während der Turm gerade schießt. Zwei einzelne Laser, oder ein gebündelter Laser, der Ziele in Brand stecken kann.");
AddMenuItem("Ein-/Ausschalten", "ControlUpDouble", SLTW, clonk, 0, 0,"Ein-/Ausschalten um eventuell Munition zu sparen");

}

protected func ControlDigDouble(idImage)
{
  [Modus wechseln]
  if (GetAction() eq "Wait1")  return(DoChange(1));
  if (GetAction() eq "Wait2")  return(DoChange(2));
  return(1);
}

protected func ControlUpDouble(idImage,zahl) //zahl nur für sensoren wichtig
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




private func DoChange(num)
{
  if (GetEnergy() < 25)
    return(Sound("Error"));
  DoEnergy(-25);
  SetAction(Format("Change%d", num));
  mode = 2-num;
}


/* Action-Call */

protected func CheckOpen()
{
if(aus==1) return(0);  // eingeschaltet?
  // Arme aus Startposition ausfahren, wenn genügend Energie
  if (EnoughEnergy())
    SetAction("Open");
}


/* Ziel suchen */

protected func FindTarget()
{
if(aus==1) {SetAction("Wait0");return(0);}  // eingeschaltet?
  // Position der aktiven Kanone
  var wx = 0;
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
  if (!obj)  obj = GetTargetHostileDetonation(300, 0, wx, wy);
  //if (!obj)  obj = GetTargetAnimal(300, wx, wy);
  if (!obj)  obj = GetTargetZap(300, wx, wy);
  if (!obj)  obj = GetOtherThreats(300, wx, wy);
  // schiessen
  if (obj)
    return(GetReadyToFire(obj));
  // oder auch nicht
  cannon = !cannon;
  return(0);
}

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


/* Feuern */

public func GetReadyToFire(par_target)
{
  // Energie da?
  if (!EnoughEnergy())
    return(0);
  target = par_target;
  if (GetAction() eq "Wait1")
  {
    CreateObject(SLTG)->Init(this(),cannon);
    return(SetAction("GetReady1"));
  }
  if (GetAction() eq "Wait2")
  {
    CreateObject(SLTG)->Init(this(),2);
    return(SetAction("GetReady2"));
  }
  return(1);
}

public func Fire1()
{
  // Ziel noch da und in Reichweite?
  if (!target || Contained(target))
    return(0);


// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,0,0,0,0,0,0,0,buildingg);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
 if(munitionspack) munitionspack->~RestAmmoreduzieren(Munitionsverbrauch(1)); // restmunition im Paket wird um 1 pro Salve reduziert

  // Laserstrahl erzeugen
  DoEnergy(-10);
  var color = RGB(255,192+Random(64),0);
  if (cannon)
  {
    DrawLaser(GetX()+31, GetY()-28, GetX(target), GetY(target), color);
    DrawLaser(GetX()+32, GetY()-15, GetX()+31, GetY()-28, color);
  }
  else
  {
    DrawLaser(GetX()-32, GetY()-28, GetX(target), GetY(target), color);
    DrawLaser(GetX()-33, GetY()-15, GetX()-32, GetY()-28, color);
  }
  cannon = !cannon;
  Sound("LaserShot3");
  // Geschosse werden gleich zerstört
  if (target->~IsProjectile())
    return(target->~Destroy());
  // Zaps sofort killen
  if (GetID(target) == ZAP1)
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
  if (!target || Contained(target))
    return(0);

// Munitionskontrolle
var munitionspack;
    munitionspack = FindObject(ENAP,0,0,0,0,0,0,0,buildingg);
if(!munitionspack && !FindObject(NOAM)) {return(PlayerMessage(GetOwner(),"keine Munition",this()));}
 if(munitionspack) munitionspack->~RestAmmoreduzieren(Munitionsverbrauch(2)); // restmunition im Paket wird um 1 pro Salve reduziert

  // Laserstrahl erzeugen
  DoEnergy(-25);
  DrawLaser(GetX(), GetY()-23, GetX(target), GetY(target), RGB(255,0,0));
  DrawLaser(GetX()+14, GetY()-18, GetX(),   GetY()-23, RGB(255,0,0));
  DrawLaser(GetX()-15, GetY()-18, GetX()-1, GetY()-23, RGB(255,0,0));
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
  if (GetID(target) == ZAP1)
    return(RemoveObject(target));
  // alles andere beschädigen
  //DoDamage( 15+Random(4), target);
  //DoEnergy(-7-Random(4), target);     // Schaden: 16.5 HP
    DoDmg(5,DMG_Energy,target);
  if (!Random(6))
    if (GetOCF(target) & OCF_Inflammable())
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


/* Energie */

public EnoughEnergy: return(GetEnergy() >= 25);
public IsEnergyConsumer: return(1);


/* Eigenschaften */

public InfoBarSize: return(3);
public GetMaxDamage: return(150);

