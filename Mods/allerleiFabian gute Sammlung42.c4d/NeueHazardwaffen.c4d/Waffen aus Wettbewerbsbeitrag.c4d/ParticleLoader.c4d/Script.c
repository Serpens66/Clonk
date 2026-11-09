/*-- Energiegewehr --*/

#strict

#include WEPN

local lasupgrade;

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(PTAM);
  if(data == FM_AmmoLoad)  return(60);

  if(data == FM_Reload)    return(50);
  if(data == FM_Recharge)  return(20);

  if(data == FM_AmmoUsage) return(10);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Auto)      return(true);

 // if(data == FM_Condition) return(lasupgrade);

  if(data == FM_Damage)    return(14);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(400);
  //if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Laser$");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(50);

  if(data == FM_AmmoUsage) return(10);
  if(data == FM_AmmoRate)  return(1);
  if(data == FM_Reload)    return(100);
  if(data == FM_Recharge)  return(150);

  if(data == FM_Condition) return(lasupgrade);

  if(data == FM_Damage)    return(66);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PART,x,y,GetController(user));
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
  var angle = user->~AimAngle(10) + RandomX(-3,+3);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PART,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 2),0,60);
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
}

public func Upgrade(id uid) {
  if(uid == KLAS) {
    SetFireMode(2);
    lasupgrade=true;
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
