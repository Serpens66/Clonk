/*-- Ultra Rifle --*/

#strict 3

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
  i ??= 0;
  Sound("scout_reload");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(SPBT);
  if(Equal(data, FM_AmmoLoad))  return(2);

  if(Equal(data, FM_AmmoRate)) return(1);
  if(Equal(data, FM_AmmoUsage)) return(2);
  if(Equal(data, FM_Reload))    return(250);
  if(Equal(data, FM_Recharge))  return(30);
  if(Equal(data, FM_Aim))    return(2);
  if(Equal(data, FM_Auto)) 	   return(nil);

  if(Equal(data, FM_Damage))    return(180);

  return(Default(data));
}

public func BOTData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(1500);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Erweitert$");
  if(Equal(data, FM_AmmoID))    return(SPBT);
  if(Equal(data, FM_AmmoLoad))  return(4);
  if(Equal(data, FM_Condition)) return(rifleupgrade);

  if(Equal(data, FM_AmmoRate)) return(1);
  if(Equal(data, FM_AmmoUsage)) return(2);
  if(Equal(data, FM_Reload))    return(350);
  if(Equal(data, FM_Recharge))  return(50);
  if(Equal(data, FM_Aim))    return(2);
  if(Equal(data, FM_Auto)) 	   return(nil);

  if(Equal(data, FM_Damage))    return(180);

  return(Default(data));
}

public func BOTData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(1500);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

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
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(60,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);
  // Sound
  Sound("gauss*",nil,ammo);
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
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(60,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);
  // Sound
  Sound("gauss*",nil,ammo);
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

public func IsImbaWeapon()
{ return(true); }
