/*-- Energiegewehr --*/

#strict

#include WEPN

local wepupgrade, konv, lasupgrd;

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(30);

  if(data == FM_Reload)    return(140);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(5);

  if(data == FM_Damage)    return(14);

  return(Default(data));
}


public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData2(int data)
{
  if(data == FM_Name)      return("$Super$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(60);

  if(data == FM_Reload)    return(200);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(60);
  if(data == FM_Condition) return(wepupgrade);

  if(data == FM_Damage)    return(35);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData3(int data)
{
  if(data == FM_Name)      return("$Tripple$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(60);

  if(data == FM_Reload)    return(200);
  if(data == FM_Recharge)  return(30);
  if(data == FM_AmmoUsage) return(10);
  if(data == FM_Condition) return(lasupgrd);

  if(data == FM_Damage)    return(12);

  return(Default(data));
}

public func BotData3(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData4(int data)
{
  if(data == FM_Name)      return("$Doubleblast$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(60);

  if(data == FM_Reload)    return(200);
  if(data == FM_Recharge)  return(50);
  if(data == FM_AmmoUsage) return(20);
  if(data == FM_Condition) return(konv);

  if(data == FM_Damage)    return(18);

  return(Default(data));
}

public func BotData4(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData5(int data)
{
  if(data == FM_Name)      return("$MEGA$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(150);

  if(data == FM_Reload)    return(300);
  if(data == FM_Recharge)  return(50);
  if(data == FM_AmmoUsage) return(120);
  if(data == FM_Condition) return(konv && wepupgrade && lasupgrd );

  if(data == FM_Damage)    return(25);

  return(Default(data));
}

public func BotData5(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData6(int data)
{
  if(data == FM_Name)      return("Blaster nur Gebäude");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(30);

  if(data == FM_Reload)    return(140);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(5);

  if(data == FM_Damage)    return(10);

  return(Default(data));
}

public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 1),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func Fire2()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 2),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func Fire3()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle-3,80,400,8,400, GetFMData(FM_Damage, 3),0,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 3),0,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle+3,80,400,8,400, GetFMData(FM_Damage, 3),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}


public func Fire4()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle-2,80,400,8,400, GetFMData(FM_Damage, 4),0,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle+2,80,400,8,400, GetFMData(FM_Damage, 4),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}


public func Fire5()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle-2,80,400,8,400, GetFMData(FM_Damage, 5),0,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle+2,80,400,8,400, GetFMData(FM_Damage, 5),0,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle-5,80,400,8,400, GetFMData(FM_Damage, 5),0,60);
  var ammo = CreateObject(PTBL,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle+5,80,400,8,400, GetFMData(FM_Damage, 5),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}

public func Fire6()    // Projektil-Dauerfeuer-Gebäude
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PT__,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,80,400,8,400, GetFMData(FM_Damage, 6),0,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,0));
  // Sound
  Sound("EnergyRifle",0,ammo);
}


public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(uid == KRFL && !wepupgrade) return("$KLASUpgradeDesc$");
  if(uid == STRH && !konv) return("$KLASUpgradeDesc$");
  if(uid == KLAS && !lasupgrd) return("$KLASUpgradeDesc$");
}

public func Upgrade(id uid) {
  //konv, lasupgrd
  if(uid == KRFL) {
    SetFireMode(1);
    wepupgrade=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(uid == STRH) {
    SetFireMode(1);
    konv=true;
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(uid == KLAS) {
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
