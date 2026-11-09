/*-- Pistole --*/

#strict 3

#include WEPN

local lasupgrade, partk;

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(500); }

public func OnReload(int i)
{
  i ??= 0;
  Sound("PistolLoad");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(12);

  if(Equal(data, FM_Reload))    return(40);
  if(Equal(data, FM_Recharge))  return(12);

  if(Equal(data, FM_Damage))    return(19);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Super$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(30);

  if(Equal(data, FM_Reload))    return(60);
  if(Equal(data, FM_Recharge))  return(20);
  if(Equal(data, FM_AmmoUsage)) return(10);
  if(Equal(data, FM_Condition)) return(lasupgrade);

  if(Equal(data, FM_Damage))    return(8);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Laser$");
  if(Equal(data, FM_AmmoID))    return(PTAM);
  if(Equal(data, FM_AmmoLoad))  return(30);

  if(Equal(data, FM_Reload))    return(90);
  if(Equal(data, FM_Recharge))  return(20);
  if(Equal(data, FM_AmmoUsage)) return(30);
  if(Equal(data, FM_Condition)) return(partk);

  if(Equal(data, FM_Damage))    return(30);

  return(Default(data));
}

public func BotData3(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  //if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-1,+1) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ELC3,x,(y ?? 0)-5,GetController(user));
  ammo->Launch(angle,250,600,5,300,GetFMData(FM_Damage, 1),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  // Sound
  Sound("EnergyRifle");
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-1,+1) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ELC4,x,(y ?? 0)-5,GetController(user));
  ammo->Launch((angle ?? 0)-7,250,600,5,300,GetFMData(FM_Damage, 2),nil,1);
  var ammo = CreateObject(ELC4,x,(y ?? 0)-5,GetController(user));
  ammo->Launch((angle ?? 0)-3,250,600,5,300,GetFMData(FM_Damage, 2),nil,1);
  var ammo = CreateObject(ELC4,x,(y ?? 0)-5,GetController(user));
  ammo->Launch(angle,250,600,5,300,GetFMData(FM_Damage, 2),nil,1);
  var ammo = CreateObject(ELC4,x,(y ?? 0)-5,GetController(user));
  ammo->Launch((angle ?? 0)+3,250,600,5,300,GetFMData(FM_Damage, 2),nil,1);
  var ammo = CreateObject(ELC4,x,(y ?? 0)-5,GetController(user));
  ammo->Launch((angle ?? 0)+7,250,600,5,300,GetFMData(FM_Damage, 2),nil,1);
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  // Sound
  Sound("EnergyRifle");
}


public func Fire3()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(BONC,x,(y ?? 0)-5,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iDmg, int iTrail, int iDmgType
  ammo->Launch((angle ?? 0)+(RandomX(-2,2) ?? 0),180,400,8,400, GetFMData(FM_Damage, 3),nil,60);
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,nil));

  // Sound
  Sound("LaserShot",nil,ammo);
}

public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 3), DMG_Energy, pTarget);
  return(1);
}
/* Treffer-Callbacks */


/* Upgrade */

public func IsUpgradeable(id uid) {
  if(And(Equal(uid, KRFL), !lasupgrade)) return("$KLASUpgradeDesc$");
  if(And(Equal(uid, STRH), !partk)) return("$STRHUpgradeDesc$");
}

public func Upgrade(id uid) {
  var own = GetOwner(Contained());
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    lasupgrade=true;
    if(Contained()) HelpMessage(own, "$KLASUpgraded$", Contained());
    return(true);
  }
  if(Equal(uid, STRH)) {
    SetFireMode(1);
    partk=true;
    if(Contained()) HelpMessage(own, "$STRHUpgraded$", Contained());
    return(true);
  }

}
