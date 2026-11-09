/*-- Scorpion --*/

#strict

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
  if(data == FM_Name)       return("Plasmakanone");
  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(30);

  if(data == FM_Reload)     return(140);
  if(data == FM_Recharge)   return(8);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(1);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(20);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)       return("Stachel");
  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(30);

  if(data == FM_Reload)     return(140);
  if(data == FM_Recharge)   return(8);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(1);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(10);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)       return("nur Gebäude, Plasmakanone");
  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(30);

  if(data == FM_Reload)     return(140);
  if(data == FM_Recharge)   return(8);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(1);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(4);

  return(Default(data));
}
  
public func FMData4(int data)
{
  if(data == FM_Name)       return("nur Gebäude, Stachel");
  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(30);

  if(data == FM_Reload)     return(140);
  if(data == FM_Recharge)   return(8);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(1);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(3);

  return(Default(data));
}  


public func Bezug(_jaeger,_stachel)
{
  local jaeger, stachel;
  jaeger=_jaeger;
  stachel=_stachel;
  return(jaeger);
}


public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = GetR(jaeger)+RandomX(-3,+3);
  var x,y;
  x=Cos( GetR(jaeger) + 275 ,20);
  y=Sin( GetR(jaeger) + 275 ,20);
  var ammo = CreateObject(SHT2,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 1),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func Fire2()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = GetR(stachel)+RandomX(-3,+3);
  var x,y;
  x=-Cos( GetR(jaeger) + 275 ,0);
  y=-Sin( GetR(jaeger) + 275 ,0);
  var ammo = CreateObject(SHT2,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 2),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}


public func Fire3()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = GetR(jaeger)+RandomX(-3,+3);
  var x,y;
  x=Cos( GetR(jaeger) + 275 ,20);
  y=Sin( GetR(jaeger) + 275 ,20);
  var ammo = CreateObject(SH2_,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 3),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func Fire4()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = GetR(stachel)+RandomX(-3,+3);
  var x,y;
  x=-Cos( GetR(jaeger) + 275 ,0);
  y=-Sin( GetR(jaeger) + 275 ,0);
  var ammo = CreateObject(SH2_,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 4),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}


// Soundeffekte

public func OnReload()
{
  Sound("EnergyRifleLoad");
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