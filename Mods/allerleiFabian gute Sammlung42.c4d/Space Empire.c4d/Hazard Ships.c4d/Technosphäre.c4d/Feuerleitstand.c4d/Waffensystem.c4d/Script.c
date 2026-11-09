
#strict

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
  if(data == FM_Name)      return("Laser");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(25);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(12);

  if(data == FM_AmmoUsage) return(2);
  if(data == FM_AmmoRate)  return(2);
  if(data == FM_Auto)      return(true);

  if(data == FM_Damage)    return(10);

  return(Default(data));
}

func FMData2(int data) {
  if(data == FM_Name)       return("Railgun");

  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(100);
  if(data == FM_AmmoUsage)  return(50);

  if(data == FM_Reload)     return(110);
  if(data == FM_Recharge)   return(120);

  if(data == FM_Damage)     return(60);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)   return("EMP-SuchRakete");
  if(data == FM_Icon)     return(EHMI);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge)   return(1);


  if(data == FM_Damage) return(15);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)      return("nur Gebäude, Laser");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(25);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(12);

  if(data == FM_AmmoUsage) return(2);
  if(data == FM_AmmoRate)  return(2);
  if(data == FM_Auto)      return(true);

  if(data == FM_Damage)    return(2);

  return(Default(data));
}


// Sphäre da?
public func Sphere()
{
  local sphere;
  if(sphere=FindObject(TSG_, -20,-20,40,40, 0,0,0,0)) return(1);
  return(0);
}

public func Fire1()    // Laserschuss
{ 
  Sphere();
  if(!Sphere()) return(1);
  var user = Contained();
  var x,y;
  x=Cos( GetR(sphere) + 270 ,8);
  y=Sin( GetR(sphere) + 270 ,8);
  var angle = GetR(sphere);
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGB(50,100,255));
  laser->Set(angle,7,350,15,this(),sphere);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,0));

  // Sound
  Sound("LaserShot",0,laser);
}


public func Fire2() {
  Sphere();
  if(!Sphere()) return(1);
  var user = Contained();
  var x,y; 
  x=Cos( GetR(sphere) + 270 ,8);
  y=Sin( GetR(sphere) + 270 ,8);
  var angle = GetR(sphere);
  laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGBa(255,255,255,64));
  laser->Set(angle,26,700,60,this(),sphere);
  laser->SetContinuous(3);
  sphere->Laden();
  MuzzleFlash(300,user,x,y,angle,RGBa(64,64,255,0));
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
  x=Cos( GetR(sphere) + 270, 5);
  y=Sin( GetR(sphere) + 270, 5);
  }

  // not too near to clonk
  x = x*3/2;
  y = y*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(0,50,200,0),0,0);

  for(var i=0; i<20; ++i) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
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
  x=Cos( GetR(sphere) + 270 ,8);
  y=Sin( GetR(sphere) + 270 ,8);
  var angle = GetR(sphere);
  var laser = CreateObject(LASG,x,y,GetController(user));
  laser->SetClrModulation(RGB(50,100,255));
  laser->Set(angle,7,350,15,this(),sphere);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,0));

  // Sound
  Sound("LaserShot",0,laser);
}


public func LaserStrike(object pObj) {
  //Schaden machen, natürlich tollen Energiebrazzelschaden!
  if(firemode == 1)
  {
  DoDmg(GetFMData(FM_Damage, 1), DMG_Energy, pObj, 10);
  return(1);
  }
  if(firemode == 2)
{
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pObj, 10);
  return(1);
}
  if(firemode == 4)
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
  if(firemode == 1)
    Sound("EMPLoad");
  if(firemode == 2)
    Sound("MoteGunLoad");
  if(firemode == 3)
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
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim) && !(GetUser()->~AimOverride()))
    return(stopauto=true); //abbrechen
    
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto))
    shooting = true;

  // Munition abziehen
  if(ratecount == 1) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { --ratecount; }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = false;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
      ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}
