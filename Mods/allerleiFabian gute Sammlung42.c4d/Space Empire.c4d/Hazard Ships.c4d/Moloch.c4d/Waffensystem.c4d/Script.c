/*-- Moloch --*/

#strict 3

#include WEPN
public func IsWeaponSystem()  { return(true); }

// Anzeige in der Hand
public func HandSize() { return(900);  }
public func HandX()    { return(11000); }
public func HandY()    { return(-3000); }
public func BarrelYOffset(){return(-1000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("Stachelkanone");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(32);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(50);

  if(Equal(data, FM_AmmoUsage))  return(4);
  if(Equal(data, FM_AmmoRate))   return(2);

  if(Equal(data, FM_Damage))     return(10);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("Sporen");
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(100);
  if(Equal(data, FM_Recharge))   return(1);


  if(Equal(data, FM_Damage)) return(15);

  return(Default(data));
}

// Jäger da?
public func Predator()
{
  local jaeger;
  if(jaeger=FindObject(MOO_, -20,-20,40,40, nil,nil,nil,nil)) return(1);
  return(nil);
}


// Feuern im ersten Feuermodus
public func Fire1()		// ungenaues aber heftiges Minigunfeuer
{ 
  if(!Predator()) return(nil);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = GetR(jaeger);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Cos( (GetR(jaeger) ?? 0) +270 , 40);
  y=Sin( (GetR(jaeger) ?? 0) +270 , 40);
  var ammo = CreateObject(SH_M,x,y,GetController(user));
  ammo->Launch((angle ?? 0)+(RandomX(-6,+6) ?? 0),250+(RandomX(-50,+50) ?? 0),500+(RandomX(-70,+70) ?? 0),4,600,GetFMData(FM_Damage, 1));
  var ammo = CreateObject(SH_M,x,y,GetController(user));
  ammo->Launch((angle ?? 0)+(RandomX(-6,+6) ?? 0),250+(RandomX(-50,+50) ?? 0),500+(RandomX(-70,+70) ?? 0),4,600,GetFMData(FM_Damage, 1));
  var ammo = CreateObject(SH_M,x,y,GetController(user));
  ammo->Launch((angle ?? 0)+(RandomX(-5,+5) ?? 0),250+(RandomX(-50,+50) ?? 0),500+(RandomX(-70,+70) ?? 0),4,600,GetFMData(FM_Damage, 1));
  var ammo = CreateObject(SH_M,x,y,GetController(user));
  ammo->Launch((angle ?? 0)+(RandomX(-5,+5) ?? 0),250+(RandomX(-50,+50) ?? 0),500+(RandomX(-70,+70) ?? 0),4,600,GetFMData(FM_Damage, 1));
  // Sound
  Sound("Needle");
  
}

public func Fire2()
{   
Predator();
LaunchRocket(nil,GetR(jaeger), GetFMData(FM_Damage, 2));
}

public func LaunchRocket(rid, angle, dmg) {

  var user = Contained();
  var x,y;

  x=Cos( (GetR(jaeger) ?? 0) + 270 ,40);
  y=Sin( (GetR(jaeger) ?? 0) + 270 ,40);

  // not too near to clonk
  x = (x ?? 0)*3/2;
  y = (y ?? 0)*3/2;

  var rocket = CreateObject(SPO_,x,y,GetController(user));
  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = (ax ?? 0)-(x ?? 0);
  ydir = (ay ?? 0)-(y ?? 0);

  // Sound
  Sound("Sporelaunch");

}


// Soundeffekte

public func OnReload()
{
  Sound("Sporeload*");
}


/* Waffenfix für neue Hazard-Version */
private func Shoot(object caller)
{
  //User da? :S
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(And(And(!(GetUser()->~IsAiming()), GetFMData(FM_Aim)), !(GetUser()->~AimOverride())))
    return(stopauto=true); //abbrechen
    
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto))
    shooting = true;

  // Munition abziehen
  if(Equal(ratecount, 1)) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-(muni ?? 0));
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { S42MigrationStep(ratecount, -1); }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = nil;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
      ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}