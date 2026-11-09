/*-- SCOUT --*/

#strict

#include WEPN



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
  if(data == FM_Name)   return("$Standard$");
  if(data == FM_Icon)     return(GetID());	
  if(data == FM_AmmoID)   return(SPBT);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(250);
  if(data == FM_Recharge)   return(30);

  if(data == FM_Aim)    return(2);

  if(data == FM_Damage) return(170);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(1000);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)   return("$Erweitert$");
  if(data == FM_Icon)     return(GetID());	
  if(data == FM_AmmoID)   return(SPBT);
  if(data == FM_AmmoLoad) return(2);

  if(data == FM_Reload)   return(250);
  if(data == FM_Recharge)   return(30);

  if(data == FM_Aim)    return(2);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage) return(170);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(1000);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,1000,5,500,GetFMData(FM_Damage, 1),0,1);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(user==GetCursor(GetOwner()))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("scout",0,ammo);
}


public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,1000,5,500,GetFMData(FM_Damage, 2),0,1);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(user==GetCursor(GetOwner()))
    SetPlrView(GetOwner(), ammo);
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("scout",0,ammo);
}



/* Upgrades */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
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
  if(uid == KRFL) {
    SetFireMode(1);
    return(true);
  }
}


public func IsMupfWeapon()
{ return(true); }

public func IsImbaWeapon()
{ return(true); }