/*-- Energiegewehr --*/

#strict 3

#include WEPN

local lasupgrade;

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(60);

  if(Equal(data, FM_Reload))    return(50);
  if(Equal(data, FM_Recharge))  return(20);

  if(Equal(data, FM_AmmoUsage)) return(10);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Auto))      return(true);

 // if(data == FM_Condition) return(lasupgrade);

  if(Equal(data, FM_Damage))    return(14);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  //if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Laser$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(50);

  if(Equal(data, FM_AmmoUsage)) return(10);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Reload))    return(100);
  if(Equal(data, FM_Recharge))  return(150);

  if(Equal(data, FM_Condition)) return(lasupgrade);

  if(Equal(data, FM_Damage))    return(66);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PART,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 1),nil,60);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire2()    // Laserschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-3,+3) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(PART,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch(angle,180,400,8,400, GetFMData(FM_Damage, 2),nil,60);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,nil));

  // Sound
  Sound("LaserShot",nil,ammo);
}

public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(And(Equal(uid, KLAS), !lasupgrade)) return("$KLASUpgradeDesc$");
}

public func Upgrade(id uid) {
  if(Equal(uid, KLAS)) {
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
