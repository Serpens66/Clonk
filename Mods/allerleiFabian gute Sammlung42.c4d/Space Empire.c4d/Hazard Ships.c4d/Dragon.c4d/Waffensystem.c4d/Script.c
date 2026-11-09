/*-- Dragonpower--*/

#strict 3

#include WEPN
public func IsWeaponSystem()  { return(true); }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("Gatling");
  if(Equal(data, FM_Icon))       return(MIWP);
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(100);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(12);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("Flammenwerfer");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(100);

  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(4);
  if(Equal(data, FM_Auto))     return(true);

  if(Equal(data, FM_Damage))   return(20);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("FragGrenade");
  if(Equal(data, FM_Icon))     return(CGRN);
  if(Equal(data, FM_AmmoID))   return(GRAM);
  if(Equal(data, FM_AmmoLoad)) return(12);
  if(Equal(data, FM_AmmoUsage))return(3);
  if(Equal(data, FM_Reload))   return(170);
  if(Equal(data, FM_Recharge)) return(40);

  if(Equal(data, FM_Damage))   return(5);

  return(Default(data));
}


public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("nur Gebäude, Gatling");
  if(Equal(data, FM_Icon))       return(MIWP);
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(100);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(12);

  return(Default(data));
}

public func FMData5(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("Flammenw. Gebäude und Pflanzen");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(100);

  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(4);
  if(Equal(data, FM_Auto))     return(true);

  if(Equal(data, FM_Damage))   return(5);

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
  var angle = (GetR(turret) ?? 0)+(RandomX(-2,+2) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Sin(GetR(turret),10);
  y=-(Cos( GetR(turret),10) ?? 0);
  var ammo = turret->CreateObject(SHT_,(x ?? 0)/2,(y ?? 0)/2,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 1));

  // Effekte
  turret->MuzzleFlash(RandomX(20,50),user,(x ?? 0)/2,(y ?? 0)/2,angle);
  
  user->WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}

public func Fire2()
{
  var user = Contained();
  var x,y;
  x=Sin(GetR(turret),10);
  y=-(Cos( GetR(turret),10) ?? 0);
  var angle = (GetR(turret) ?? 0)+(RandomX(-2,+2) ?? 0);
  var xdir = (+(Sin(angle,RandomX(80,100)) ?? 0) ?? 0)+(GetXDir(Contained()) ?? 0);
  var ydir = -(Cos(angle,RandomX(80,100)) ?? 0)+(GetYDir(Contained()) ?? 0);
  var flame = turret->CreateObject(FFLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 2));
  flame = turret->CreateObject(FFLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 2));
  if(!Random(3))
    Smoke(x,y,Random(10));
}

public func Fire3()
{
  var user = Contained();
  var x,y;
  x=Sin(GetR(turret),10);
  y=-(Cos( GetR(turret),10) ?? 0);
  var angle = (GetR(turret) ?? 0)+(RandomX(-2,+2) ?? 0);
  var xdir = (+(Sin(angle,RandomX(120,130)) ?? 0) ?? 0)+(GetXDir(Contained()) ?? 0);
  var ydir = -(Cos(angle,RandomX(120,130)) ?? 0)+(GetYDir(Contained()) ?? 0);
  // create and launch
  var grenade=turret->CreateObject(CGRN, (x ?? 0)+(xdir ?? 0)/10, (y ?? 0)+(ydir ?? 0)/10, GetController(user));
  grenade->Launch((xdir ?? 0)+(GetXDir(user) ?? 0)/2, (ydir ?? 0)+(GetYDir(user) ?? 0)/2, GetFMData(FM_Damage,3));

  // effect
  Sound("GrenadeFire");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,nil),nil,nil);

  for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
    CreateParticle("Smoke2",(x ?? 0)+(RandomX(-5,+5) ?? 0),(y ?? 0)+(RandomX(-5,+5) ?? 0),
                   (GetXDir(user) ?? 0)+(RandomX(nil,(xdir ?? 0)/4) ?? 0),(GetYDir(user) ?? 0)+(RandomX(nil,(ydir ?? 0)/4) ?? 0),
                   RandomX(80,140),RGBa(200,200,200,nil),nil,nil);
  }
}

public func Fire4()
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (GetR(turret) ?? 0)+(RandomX(-2,+2) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Sin(GetR(turret),10);
  y=-(Cos( GetR(turret),10) ?? 0);
  var ammo = turret->CreateObject(SHTG,(x ?? 0)/2,(y ?? 0)/2,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 4));

  // Effekte
  turret->MuzzleFlash(RandomX(20,50),user,(x ?? 0)/2,(y ?? 0)/2,angle);
  
  user->WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}


public func Fire5()
{
  var user = Contained();
  var x,y;
  x=Sin(GetR(turret),10);
  y=-(Cos( GetR(turret),10) ?? 0);
  var angle = (GetR(turret) ?? 0)+(RandomX(-2,+2) ?? 0);
  var xdir = (+(Sin(angle,RandomX(80,100)) ?? 0) ?? 0)+(GetXDir(Contained()) ?? 0);
  var ydir = -(Cos(angle,RandomX(80,100)) ?? 0)+(GetYDir(Contained()) ?? 0);
  var flame = turret->CreateObject(_FLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 5));
  flame = turret->CreateObject(_FLM, x,y, GetController(Contained()));
  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 5));
  if(!Random(3))
    Smoke(x,y,Random(10));
}


// Soundeffekte

public func OnReload(int i)
{
  i ??= 0;
  if(Equal(i, 1)) Sound("MiniLoad");
  if(Equal(i, 2)) Sound("FlamerLoad");
  if(Equal(i, 3)) Sound("GrenadeLoad");
}

public func OnAutoStart(int i)
{
  i ??= 0;
  if(Equal(i, 1))
  {
  Sound("MiniGun",nil,nil,nil,nil,1);
  Sound("MiniTurn",nil,nil,nil,nil,1);
  }
  if(Equal(i, 2)) Sound("Flamer",nil,nil,nil,nil,1);
}

public func OnAutoStop(int i)
{
  i ??= 0;
  if(Equal(i, 1))
  {
  Sound("MiniGun",nil,nil,nil,nil,-1);
  Sound("MiniTurn",nil,nil,nil,nil,-1);
  }
  if(Equal(i, 2)) Sound("Flamer",nil,nil,nil,nil,-1);
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