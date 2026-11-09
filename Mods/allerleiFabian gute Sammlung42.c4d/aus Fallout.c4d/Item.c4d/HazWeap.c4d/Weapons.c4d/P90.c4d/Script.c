/*-- P90 --*/

#strict

#include WEPN

local rifleupgrade;

// Anzeige in der Hand
public func HandSize() { return(900); }
public func HandX()    { return(5000); }
public func HandY()    { return(0); }
public func BarrelYOffset() { return(-1000); }

public func Selection()
{
  Sound("p90_select");
  return(1);
}

public func OnReload(int i)
{
  Sound("p90_reload");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(50);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(data == FM_Reload)    return(150);
  if(data == FM_Recharge)  return(2);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(3);

  return(Default(data));
}


public func BotData1(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData2(int data)
{
  if(data == FM_Name)      return("$Bettershot$");
  if(data == FM_AmmoID)    return(SSTM);
  if(data == FM_AmmoLoad)  return(50);
  if(data == FM_Condition) return(rifleupgrade);

  if(data == FM_Reload)    return(150);
  if(data == FM_Recharge)  return(2);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(3);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(500);
  if(data == BOT_Power)    return(BOT_Power_2);
  if(data == BOT_DmgType)  return(DMG_BetterProjectile);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)      return("nur Gebäude, Schnellfeuer");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(50);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(data == FM_Reload)    return(150);
  if(data == FM_Recharge)  return(2);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(1);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20) + RandomX(-6,+6);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1),0,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("p90",0,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20) + RandomX(-6,+6);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 2),0,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("p90",0,ammo);
}

public func Fire3()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20) + RandomX(-6,+6);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 3),0,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("p90",0,ammo);
}


/* Upgrade */

public func IsUpgradeable(id uid) {
  if(uid == KRFL && !rifleupgrade) return("$KRFLUpgradeDesc$");
}

public func Upgrade(id uid) {
  var own = GetOwner(Contained());
  if(uid == KRFL) {
    SetFireMode(2);
    rifleupgrade=true;
    if(Contained()) HelpMessage(own, "$KRFLUpgraded$", Contained());
    return(true);
  }
}


public func IsMupfWeapon()
{ return(true); }
