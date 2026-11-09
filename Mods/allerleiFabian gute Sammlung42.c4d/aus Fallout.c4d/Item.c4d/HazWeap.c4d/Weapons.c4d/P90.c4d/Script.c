/*-- P90 --*/

#strict 3

#include WEPN

local rifleupgrade;

// Anzeige in der Hand
public func HandSize() { return(900); }
public func HandX()    { return(5000); }
public func HandY()    { return(nil); }
public func BarrelYOffset() { return(-1000); }

public func Selection()
{
  Sound("p90_select");
  return(1);
}

public func OnReload(int i)
{
  i ??= 0;
  Sound("p90_reload");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(50);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(Equal(data, FM_Reload))    return(150);
  if(Equal(data, FM_Recharge))  return(2);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(3);

  return(Default(data));
}


public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}


public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Bettershot$");
  if(Equal(data, FM_AmmoID))    return(SSTM);
  if(Equal(data, FM_AmmoLoad))  return(50);
  if(Equal(data, FM_Condition)) return(rifleupgrade);

  if(Equal(data, FM_Reload))    return(150);
  if(Equal(data, FM_Recharge))  return(2);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(3);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);
  if(Equal(data, BOT_DmgType))  return(DMG_BetterProjectile);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("nur Gebäude, Schnellfeuer");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(50);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(Equal(data, FM_Reload))    return(150);
  if(Equal(data, FM_Recharge))  return(2);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(1);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-6,+6) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("p90",nil,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-6,+6) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 2),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("p90",nil,ammo);
}

public func Fire3()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-6,+6) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 3),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("p90",nil,ammo);
}


/* Upgrade */

public func IsUpgradeable(id uid) {
  if(And(Equal(uid, KRFL), !rifleupgrade)) return("$KRFLUpgradeDesc$");
}

public func Upgrade(id uid) {
  var own = GetOwner(Contained());
  if(Equal(uid, KRFL)) {
    SetFireMode(2);
    rifleupgrade=true;
    if(Contained()) HelpMessage(own, "$KRFLUpgraded$", Contained());
    return(true);
  }
}


public func IsMupfWeapon()
{ return(true); }
