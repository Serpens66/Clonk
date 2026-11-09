/*-- SOCOM --*/

#strict 3

#include WEPN

local rifleupgrade;

// Anzeige in der Hand
public func HandSize() { return(900); }
public func HandX()    { return(5000); }
public func HandY()    { return(nil); }
public func BarrelYOffset() { return(-1000); }

public func OnReload(int i)
{
  i ??= 0;
  Sound("socom_reload");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(17);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(Equal(data, FM_Reload))    return(60);
  if(Equal(data, FM_Recharge))  return(12);
  if(Equal(data, FM_Auto)) 	   return(nil);

  if(Equal(data, FM_Damage))    return(5);

  return(Default(data));
}


public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standart²$");
  if(Equal(data, FM_AmmoID))    return(SSTM);
  if(Equal(data, FM_AmmoLoad))  return(17);
 // if(data == FM_Condition) return(!rifleupgrade);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(15);
  if(Equal(data, FM_Auto)) 	   return(nil);

  if(Equal(data, FM_Damage))    return(8);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);
  if(Equal(data, BOT_DmgType))  return(DMG_BetterProjectile);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-4,+4) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT1,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  (MuzzleFlash(40,user,x,y,angle) ?? 0) + (RandomX(-2,+2) ?? 0);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("socom",nil,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-4,+4) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 2));

  // Effekte
  (MuzzleFlash(40,user,x,y,angle) ?? 0) + (RandomX(-2,+2) ?? 0);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("socom",nil,ammo);
}

public func IsMupfWeapon()
{ return(true); }


public func IsAnvilProduct() { return(100); }