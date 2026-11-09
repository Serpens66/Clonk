/*-- Ultra Rifle --*/

#strict

#include WEPN

local rifleupgrade;

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(10000); }
public func HandY()    { return(1500); }
public func BarrelYOffset() { return(-500); }
public func BarrelXOffset() { return(-14000); }

public func OnReload(int i)
{
  Sound("scout_reload");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(SPBT);
  if(data == FM_AmmoLoad)  return(2);

  if(data == FM_AmmoRate) return(1);
  if(data == FM_AmmoUsage) return(2);
  if(data == FM_Reload)    return(250);
  if(data == FM_Recharge)  return(30);
  if(data == FM_Aim)    return(2);
  if(data == FM_Auto) 	   return(0);

  if(data == FM_Damage)    return(180);

  return(Default(data));
}

public func BOTData1(int data)
{
  if(data == BOT_Range)    return(1500);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Erweitert$");
  if(data == FM_AmmoID)    return(SPBT);
  if(data == FM_AmmoLoad)  return(4);
  if(data == FM_Condition) return(rifleupgrade);

  if(data == FM_AmmoRate) return(1);
  if(data == FM_AmmoUsage) return(2);
  if(data == FM_Reload)    return(350);
  if(data == FM_Recharge)  return(50);
  if(data == FM_Aim)    return(2);
  if(data == FM_Auto) 	   return(0);

  if(data == FM_Damage)    return(180);

  return(Default(data));
}

public func BOTData2(int data)
{
  if(data == BOT_Range)    return(1500);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  CastObjects(SPRK, 15, 15);
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,750,1500,3,900,GetFMData(FM_Damage, 1),50,1);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(user==GetCursor(GetOwner()))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(60,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);
  // Sound
  Sound("gauss*",0,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  CastObjects(SPRK, 15, 15);
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,750,5000,3,900,GetFMData(FM_Damage, 2),50,1);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(user==GetCursor(GetOwner()))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(60,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);
  // Sound
  Sound("gauss*",0,ammo);
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

public func IsImbaWeapon()
{ return(true); }
