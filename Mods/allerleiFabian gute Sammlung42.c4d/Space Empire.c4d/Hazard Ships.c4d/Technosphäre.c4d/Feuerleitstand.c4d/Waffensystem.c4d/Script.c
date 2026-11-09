
#strict 3

#include WEPN
public func IsWeaponSystem()  { return(true); }

local laser; //der Laserstrahl

// Anzeige in der Hand
public func HandSize() { return(900);  }
public func HandX()    { return(11000); }
public func HandY()    { return(-3000); }
public func BarrelYOffset(){return(-1000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("Laser");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(25);

  if(Equal(data, FM_Reload))    return(50);
  if(Equal(data, FM_Recharge))  return(12);

  if(Equal(data, FM_AmmoUsage)) return(2);
  if(Equal(data, FM_AmmoRate))  return(2);
  if(Equal(data, FM_Auto))      return(true);

  if(Equal(data, FM_Damage))    return(10);

  return(Default(data));
}

func FMData2(int data) {
  data ??= 0;
  if(Equal(data, FM_Name))       return("Railgun");

  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(100);
  if(Equal(data, FM_AmmoUsage))  return(50);

  if(Equal(data, FM_Reload))     return(110);
  if(Equal(data, FM_Recharge))   return(120);

  if(Equal(data, FM_Damage))     return(60);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("EMP-SuchRakete");
  if(Equal(data, FM_Icon))     return(EHMI);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge))   return(1);


  if(Equal(data, FM_Damage)) return(15);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("nur Gebäude, Laser");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(25);

  if(Equal(data, FM_Reload))    return(50);
  if(Equal(data, FM_Recharge))  return(12);

  if(Equal(data, FM_AmmoUsage)) return(2);
  if(Equal(data, FM_AmmoRate))  return(2);
  if(Equal(data, FM_Auto))      return(true);

  if(Equal(data, FM_Damage))    return(2);

  return(Default(data));
}


// Sphäre da?
public func Sphere()
{
  local sphere;
  if(sphere=FindObject(TSG_, -20,-20,40,40, nil,nil,nil,nil)) return(1);
  return(nil);
}

public func Fire1()    // Laserschuss
{ 
  Sphere();
  if(!Sphere()) return(1);
  var user = Contained();
  var x,y;
  x=Cos( (GetR(sphere) ?? 0) + 270 ,8);
  y=Sin( (GetR(sphere) ?? 0) + 270 ,8);
  var angle = GetR(sphere);
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGB(50,100,255));
  laser->Set(angle,7,350,15,this(),sphere);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,nil));

  // Sound
  Sound("LaserShot",nil,laser);
}


public func Fire2() {
  Sphere();
  if(!Sphere()) return(1);
  var user = Contained();
  var x,y; 
  x=Cos( (GetR(sphere) ?? 0) + 270 ,8);
  y=Sin( (GetR(sphere) ?? 0) + 270 ,8);
  var angle = GetR(sphere);
  laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGBa(255,255,255,64));
  laser->Set(angle,26,700,60,this(),sphere);
  laser->SetContinuous(3);
  sphere->Laden();
  MuzzleFlash(300,user,x,y,angle,RGBa(64,64,255,nil));
  Sound("RailGunShot");
}

public func Fire3()
{   
Sphere();
LaunchRocket(EHMI,GetR(sphere), GetFMData(FM_Damage, 2));
}

public func LaunchRocket(rid, angle, dmg) {

  var user = Contained();
  var x,y;


  {
  x=Cos( (GetR(sphere) ?? 0) + 270, 5);
  y=Sin( (GetR(sphere) ?? 0) + 270, 5);
  }

  // not too near to clonk
  x = (x ?? 0)*3/2;
  y = (y ?? 0)*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = (ax ?? 0)-(x ?? 0);
  ydir = (ay ?? 0)-(y ?? 0);

  CreateParticle("Thrust",ax,ay,(xdir ?? 0)/2,(ydir ?? 0)/2,80,RGBa(nil,50,200,nil),nil,nil);

  for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",(ax ?? 0)+(Sin(angle,rand) ?? 0),(ay ?? 0)-(Cos(angle,rand) ?? 0),
                   RandomX(nil,2*(xdir ?? 0)),RandomX(nil,2*(ydir ?? 0)),
                   RandomX(80,140),RGBa(220,200,180,nil),nil,nil);
  }

  // Sound
  Sound("Rocket");

}


public func Fire4()    // Laserschuss
{ 
  Sphere();
  if(!Sphere()) return(1);
  var user = Contained();
  var x,y;
  x=Cos( (GetR(sphere) ?? 0) + 270 ,8);
  y=Sin( (GetR(sphere) ?? 0) + 270 ,8);
  var angle = GetR(sphere);
  var laser = CreateObject(LASG,x,y,GetController(user));
  laser->SetClrModulation(RGB(50,100,255));
  laser->Set(angle,7,350,15,this(),sphere);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,nil));

  // Sound
  Sound("LaserShot",nil,laser);
}


public func LaserStrike(object pObj) {
  //Schaden machen, natürlich tollen Energiebrazzelschaden!
  if(Equal(firemode, 1))
  {
  DoDmg(GetFMData(FM_Damage, 1), DMG_Energy, pObj, 10);
  return(1);
  }
  if(Equal(firemode, 2))
{
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pObj, 10);
  return(1);
}
  if(Equal(firemode, 4))
{
  DoDmg(GetFMData(FM_Damage, 4), DMG_Energy, pObj, 10);
  return(1);
}
  DoDmg(GetFMData(FM_Damage, 1), DMG_Energy, pObj, 10);
  return(1);
}


// Soundeffekte

public func OnReload()
{
  if(Equal(firemode, 1))
    Sound("EMPLoad");
  if(Equal(firemode, 2))
    Sound("MoteGunLoad");
  if(Equal(firemode, 3))
    Sound("RocketLoad");
}

public func OnDeselect() {
  if(laser) {
    RemoveObject(laser);
  }
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
