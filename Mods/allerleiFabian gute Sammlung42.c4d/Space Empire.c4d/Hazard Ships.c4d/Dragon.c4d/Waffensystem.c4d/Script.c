/*-- Dragonpower--*/

#strict

#include WEPN
public func IsWeaponSystem()  { return(true); }

public func FMData1(int data)
{
  if(data == FM_Name)       return("Gatling");
  if(data == FM_Icon)       return(MIWP);
  if(data == FM_AmmoID)     return(STAM);
  if(data == FM_AmmoLoad)   return(100);

  if(data == FM_Reload)     return(160);
  if(data == FM_Recharge)   return(3);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(2);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(12);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)     return("Flammenwerfer");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(100);

  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(4);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Damage)   return(20);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)     return("FragGrenade");
  if(data == FM_Icon)     return(CGRN);
  if(data == FM_AmmoID)   return(GRAM);
  if(data == FM_AmmoLoad) return(12);
  if(data == FM_AmmoUsage)return(3);
  if(data == FM_Reload)   return(170);
  if(data == FM_Recharge) return(40);

  if(data == FM_Damage)   return(5);

  return(Default(data));
}


public func FMData4(int data)
{
  if(data == FM_Name)       return("nur Gebäude, Gatling");
  if(data == FM_Icon)       return(MIWP);
  if(data == FM_AmmoID)     return(STAM);
  if(data == FM_AmmoLoad)   return(100);

  if(data == FM_Reload)     return(160);
  if(data == FM_Recharge)   return(3);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(2);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(12);

  return(Default(data));
}

public func FMData5(int data)
{
  if(data == FM_Name)     return("Flammenw. Gebäude und Pflanzen");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(100);

  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(4);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Damage)   return(5);

  return(Default(data));
}


// Turm
public func Turret(_turret)
{
  local turret;
  turret=_turret;
  return(turret);
}


public func Fire1()
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = GetR(turret)+RandomX(-2,+2);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Sin(GetR(turret),10);
  y=-Cos( GetR(turret),10);
  var ammo = turret->CreateObject(SHT_,x/2,y/2,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 1));

  // Effekte
  turret->MuzzleFlash(RandomX(20,50),user,x/2,y/2,angle);
  
  user->WeaponBegin(x,y);
  var dir = GetDir(user)*2-1;
  BulletCasing(x,y,-dir*Cos(angle-35*dir,40+Random(20)),-dir*Sin(angle-35*dir,40+Random(20)),5);
}

public func Fire2()
{
  var user = Contained();
  var x,y;
  x=Sin(GetR(turret),10);
  y=-Cos( GetR(turret),10);
  var angle = GetR(turret)+RandomX(-2,+2);
  var xdir = +Sin(angle,RandomX(80,100))+GetXDir(Contained());
  var ydir = -Cos(angle,RandomX(80,100))+GetYDir(Contained());
  var flame = turret->CreateObject(FFLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 2));
  flame = turret->CreateObject(FFLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 2));
  if(!Random(3))
    Smoke(x,y,Random(10));
}

public func Fire3()
{
  var user = Contained();
  var x,y;
  x=Sin(GetR(turret),10);
  y=-Cos( GetR(turret),10);
  var angle = GetR(turret)+RandomX(-2,+2);
  var xdir = +Sin(angle,RandomX(120,130))+GetXDir(Contained());
  var ydir = -Cos(angle,RandomX(120,130))+GetYDir(Contained());
  // create and launch
  var grenade=turret->CreateObject(CGRN, x+xdir/10, y+ydir/10, GetController(user));
  grenade->Launch(xdir+GetXDir(user)/2, ydir+GetYDir(user)/2, GetFMData(FM_Damage,3));

  // effect
  Sound("GrenadeFire");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    CreateParticle("Smoke2",x+RandomX(-5,+5),y+RandomX(-5,+5),
                   GetXDir(user)+RandomX(0,xdir/4),GetYDir(user)+RandomX(0,ydir/4),
                   RandomX(80,140),RGBa(200,200,200,0),0,0);
  }
}

public func Fire4()
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = GetR(turret)+RandomX(-2,+2);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Sin(GetR(turret),10);
  y=-Cos( GetR(turret),10);
  var ammo = turret->CreateObject(SHTG,x/2,y/2,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 4));

  // Effekte
  turret->MuzzleFlash(RandomX(20,50),user,x/2,y/2,angle);
  
  user->WeaponBegin(x,y);
  var dir = GetDir(user)*2-1;
  BulletCasing(x,y,-dir*Cos(angle-35*dir,40+Random(20)),-dir*Sin(angle-35*dir,40+Random(20)),5);
}


public func Fire5()
{
  var user = Contained();
  var x,y;
  x=Sin(GetR(turret),10);
  y=-Cos( GetR(turret),10);
  var angle = GetR(turret)+RandomX(-2,+2);
  var xdir = +Sin(angle,RandomX(80,100))+GetXDir(Contained());
  var ydir = -Cos(angle,RandomX(80,100))+GetYDir(Contained());
  var flame = turret->CreateObject(_FLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 5));
  flame = turret->CreateObject(_FLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 5));
  if(!Random(3))
    Smoke(x,y,Random(10));
}


// Soundeffekte

public func OnReload(int i)
{
  if(i == 1) Sound("MiniLoad");
  if(i == 2) Sound("FlamerLoad");
  if(i == 3) Sound("GrenadeLoad");
}

public func OnAutoStart(int i)
{
  if(i == 1)
  {
  Sound("MiniGun",0,0,0,0,1);
  Sound("MiniTurn",0,0,0,0,1);
  }
  if(i == 2) Sound("Flamer",0,0,0,0,1);
}

public func OnAutoStop(int i)
{
  if(i == 1)
  {
  Sound("MiniGun",0,0,0,0,-1);
  Sound("MiniTurn",0,0,0,0,-1);
  }
  if(i == 2) Sound("Flamer",0,0,0,0,-1);
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