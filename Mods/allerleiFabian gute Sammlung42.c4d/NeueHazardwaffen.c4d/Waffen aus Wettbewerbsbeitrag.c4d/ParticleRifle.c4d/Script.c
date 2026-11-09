/*-- Energiegewehr --*/

#strict

#include WEPN

local lasupgrade;
local upgradewep;
local upgradekonv;

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(10);

  if(data == FM_Reload)    return(140);
  if(data == FM_Recharge)  return(2);

  if(data == FM_AmmoUsage) return(1);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(true);

  if(data == FM_Condition) return(true);

  if(data == FM_Damage)    return(10);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Laser$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(27);

  if(data == FM_Reload)    return(200);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(3);
  if(data == FM_AmmoRate)  return(1);

  if(data == FM_Condition) return(lasupgrade);

  if(data == FM_Damage)    return(60);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData3(int data)
{
  if(data == FM_Name)      return("$Doppel$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(12);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(30);

  if(data == FM_Condition) return(upgradewep);

  if(data == FM_Damage)    return(50);

  return(Default(data));
}

public func BotData3(int data)
{
  if(data == BOT_Range)    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)      return("$Zufall$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(16);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(8);

  if(data == FM_Condition) return(upgradekonv && upgradewep && lasupgrade);

  if(data == FM_Damage)    return(30);

  return(Default(data));
}

public func BotData4(int data)
{
  if(data == BOT_Range)    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData5(int data)
{
  if(data == FM_Name)      return("$LaserShot$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(20);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(10);

  if(data == FM_AmmoUsage) return(10);
  if(data == FM_AmmoRate)  return(1);

  if(data == FM_Condition) return(upgradekonv);

  if(data == FM_Damage)    return(30);

  return(Default(data));
}

public func BotData5(int data)
{
  if(data == BOT_Range)    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-5,+5);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ELEE,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 1),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func Fire2()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ELEE,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 2),0,60);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,0));

  // Sound
  Sound("LaserShot",0,ammo);
}

public func Fire3()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(5) + RandomX(-2,+1);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ELEE,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage,3),0,60);
  var user = GetUser();
  var angle = user->~AimAngle(5) + RandomX(-1,+2);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ELEE,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 3),0,60);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,0));

  // Sound
  Sound("LaserShot",0,ammo);
}

public func Fire4()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var rand=Random(3);

if(rand==0)
{
  var ammo = CreateObject(ELEE,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 4),0,60);
}


if(rand==1)
{
  var ammo = CreateObject(PART,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 4),0,60);
}

if(rand==2)
{
  var ammo = CreateObject(SHT1,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 4),0,60);
}


if(rand==3)
{
  var ammo = CreateObject(SHT2,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 4),0,60);
}


  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,0));

  // Sound
  Sound("LaserShot",0,ammo);
}


public func Fire5()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(BONC,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle+RandomX(-2,2),180,400,8,400, GetFMData(FM_Damage, 5),0,60);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,0));

  // Sound
  Sound("LaserShot",0,ammo);
}

public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(uid == KLAS && !lasupgrade) return("$KLASUpgradeDesc$");
  if(uid == KRFL && !upgradewep) return("$KRFLUpgradeDesc$");
  if(uid == STRH && !upgradekonv) return("$STRHUpgradeDesc$");
}

public func Upgrade(id uid) {
  if(uid == KLAS) {
    SetFireMode(1);
    lasupgrade=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(uid == KRFL) {
    SetFireMode(1);
    upgradewep=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
  if(uid == STRH) {
    SetFireMode(1);
    upgradekonv=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$STRHUpgraded$",Contained());
    return(true);
  }
}

// Soundeffekte

public func OnReload()
{
  Sound("EnergyRifleLoad");
}
