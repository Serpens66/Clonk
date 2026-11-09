/*-- M249 --*/

#strict

#include WEPN

local rifleupgrade;


// Anzeige in der Hand
public func HandSize() { return(980); }
public func HandX()    { return(7200); }
public func HandY()    { return(1000);  }

public func OnReload(int i)
{
  Sound("M249reload");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(100);

  if(data == FM_Reload)    return(300);
  if(data == FM_Recharge)  return(2);
  if(data == FM_Aim)       return(2);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(3);

  return(Default(data));
}

public func BOTData1(int data)
{
  if(data == BOT_Range)    return(480);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Bettershot$");
  if(data == FM_AmmoID)    return(SSTM);
  if(data == FM_AmmoLoad)  return(100);
  if(data == FM_Condition) return(rifleupgrade);

  if(data == FM_Reload)    return(300);
  if(data == FM_Recharge)  return(3);
  if(data == FM_Aim)       return(2);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(3);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20) + RandomX(-5,+5);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1),0,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("M249",0,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->~AimAngle(20) + RandomX(-5,+5);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 2),0,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("M249",0,ammo);
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
