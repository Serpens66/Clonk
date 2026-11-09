/*-- Scorpion --*/

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
  if(Equal(data, FM_Name))       return("Plasmakanone");
  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(30);

  if(Equal(data, FM_Reload))     return(140);
  if(Equal(data, FM_Recharge))   return(8);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(1);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(20);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("Stachel");
  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(30);

  if(Equal(data, FM_Reload))     return(140);
  if(Equal(data, FM_Recharge))   return(8);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(1);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(10);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("nur Gebäude, Plasmakanone");
  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(30);

  if(Equal(data, FM_Reload))     return(140);
  if(Equal(data, FM_Recharge))   return(8);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(1);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(4);

  return(Default(data));
}
  
public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("nur Gebäude, Stachel");
  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(30);

  if(Equal(data, FM_Reload))     return(140);
  if(Equal(data, FM_Recharge))   return(8);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(1);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(3);

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
  var angle = (GetR(jaeger) ?? 0)+(RandomX(-3,+3) ?? 0);
  var x,y;
  x=Cos( (GetR(jaeger) ?? 0) + 275 ,20);
  y=Sin( (GetR(jaeger) ?? 0) + 275 ,20);
  var ammo = CreateObject(SHT2,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 1),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire2()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = (GetR(stachel) ?? 0)+(RandomX(-3,+3) ?? 0);
  var x,y;
  x=-(Cos( (GetR(jaeger) ?? 0) + 275 ,nil) ?? 0);
  y=-(Sin( (GetR(jaeger) ?? 0) + 275 ,nil) ?? 0);
  var ammo = CreateObject(SHT2,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 2),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}


public func Fire3()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = (GetR(jaeger) ?? 0)+(RandomX(-3,+3) ?? 0);
  var x,y;
  x=Cos( (GetR(jaeger) ?? 0) + 275 ,20);
  y=Sin( (GetR(jaeger) ?? 0) + 275 ,20);
  var ammo = CreateObject(SH2_,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 3),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire4()    // Projektil-Dauerfeuer
{ 
  var user = Contained();
  var angle = (GetR(stachel) ?? 0)+(RandomX(-3,+3) ?? 0);
  var x,y;
  x=-(Cos( (GetR(jaeger) ?? 0) + 275 ,nil) ?? 0);
  y=-(Sin( (GetR(jaeger) ?? 0) + 275 ,nil) ?? 0);
  var ammo = CreateObject(SH2_,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  var speed =250;
  ammo->Launch(angle,speed,600,8,400, GetFMData(FM_Damage, 4),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
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