/*-- SCOUT --*/

#strict 3

#include WEPN



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
  if(Equal(data, FM_Name))   return("$Standard$");
  if(Equal(data, FM_Icon))     return(GetID());
  if(Equal(data, FM_AmmoID))   return(SPBT);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(250);
  if(Equal(data, FM_Recharge))   return(30);

  if(Equal(data, FM_Aim))    return(2);

  if(Equal(data, FM_Damage)) return(170);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(1000);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$Erweitert$");
  if(Equal(data, FM_Icon))     return(GetID());
  if(Equal(data, FM_AmmoID))   return(SPBT);
  if(Equal(data, FM_AmmoLoad)) return(2);

  if(Equal(data, FM_Reload))   return(250);
  if(Equal(data, FM_Recharge))   return(30);

  if(Equal(data, FM_Aim))    return(2);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage)) return(170);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(1000);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,1000,5,500,GetFMData(FM_Damage, 1),nil,1);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("scout",nil,ammo);
}


public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,1000,5,500,GetFMData(FM_Damage, 2),nil,1);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("scout",nil,ammo);
}



/* Upgrades */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
  if(uid = KRFL) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    return(true);
  }
}


public func IsMupfWeapon()
{ return(true); }

public func IsImbaWeapon()
{ return(true); }