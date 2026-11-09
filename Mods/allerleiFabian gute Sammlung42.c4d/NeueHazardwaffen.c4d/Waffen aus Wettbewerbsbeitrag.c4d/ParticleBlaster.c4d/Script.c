/*-- Energiegewehr --*/

#strict 3

#include WEPN

local wepupgrade, konv, lasupgrd;

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(30);

  if(Equal(data, FM_Reload))    return(140);
  if(Equal(data, FM_Recharge))  return(20);

  if(Equal(data, FM_AmmoUsage)) return(5);

  if(Equal(data, FM_Damage))    return(14);

  return(Default(data));
}


public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Super$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(60);

  if(Equal(data, FM_Reload))    return(200);
  if(Equal(data, FM_Recharge))  return(20);

  if(Equal(data, FM_AmmoUsage)) return(60);
  if(Equal(data, FM_Condition)) return(wepupgrade);

  if(Equal(data, FM_Damage))    return(35);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Tripple$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(60);

  if(Equal(data, FM_Reload))    return(200);
  if(Equal(data, FM_Recharge))  return(30);
  if(Equal(data, FM_AmmoUsage)) return(10);
  if(Equal(data, FM_Condition)) return(lasupgrd);

  if(Equal(data, FM_Damage))    return(12);

  return(Default(data));
}

public func BotData3(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Doubleblast$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(60);

  if(Equal(data, FM_Reload))    return(200);
  if(Equal(data, FM_Recharge))  return(50);
  if(Equal(data, FM_AmmoUsage)) return(20);
  if(Equal(data, FM_Condition)) return(konv);

  if(Equal(data, FM_Damage))    return(18);

  return(Default(data));
}

public func BotData4(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData5(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$MEGA$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(150);

  if(Equal(data, FM_Reload))    return(300);
  if(Equal(data, FM_Recharge))  return(50);
  if(Equal(data, FM_AmmoUsage)) return(120);
  if(Equal(data, FM_Condition)) return(And(And(konv, wepupgrade), lasupgrd) );

  if(Equal(data, FM_Damage))    return(25);

  return(Default(data));
}

public func BotData5(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData6(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("Blaster nur Gebäude");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(30);

  if(Equal(data, FM_Reload))    return(140);
  if(Equal(data, FM_Recharge))  return(20);

  if(Equal(data, FM_AmmoUsage)) return(5);

  if(Equal(data, FM_Damage))    return(10);

  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 1),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire2()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 2),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire3()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)-3,80,400,8,400, GetFMData(FM_Damage, 3),nil,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 3),nil,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)+3,80,400,8,400, GetFMData(FM_Damage, 3),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}


public func Fire4()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)-2,80,400,8,400, GetFMData(FM_Damage, 4),nil,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)+2,80,400,8,400, GetFMData(FM_Damage, 4),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}


public func Fire5()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)-2,80,400,8,400, GetFMData(FM_Damage, 5),nil,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)+2,80,400,8,400, GetFMData(FM_Damage, 5),nil,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)-5,80,400,8,400, GetFMData(FM_Damage, 5),nil,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)+5,80,400,8,400, GetFMData(FM_Damage, 5),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire6()    // Projektil-Dauerfeuer-Gebäude
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PT__,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 6),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}


public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(And(Equal(uid, KRFL), !wepupgrade)) return("$KLASUpgradeDesc$");
  if(And(Equal(uid, STRH), !konv)) return("$KLASUpgradeDesc$");
  if(And(Equal(uid, KLAS), !lasupgrd)) return("$KLASUpgradeDesc$");
}

public func Upgrade(id uid) {
  //konv, lasupgrd
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    wepupgrade=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(Equal(uid, STRH)) {
    SetFireMode(1);
    konv=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(Equal(uid, KLAS)) {
    SetFireMode(1);
    lasupgrd=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
}

// Soundeffekte

public func OnReload()
{
  Sound("EnergyRifleLoad");
}
