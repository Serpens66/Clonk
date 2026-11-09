/*-- M249 --*/

#strict 3

#include WEPN

local rifleupgrade;


// Anzeige in der Hand
public func HandSize() { return(980); }
public func HandX()    { return(7200); }
public func HandY()    { return(1000);  }

public func OnReload(int i)
{
  i ??= 0;
  Sound("M249reload");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(100);

  if(Equal(data, FM_Reload))    return(300);
  if(Equal(data, FM_Recharge))  return(2);
  if(Equal(data, FM_Aim))       return(2);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(3);

  return(Default(data));
}

public func BOTData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(480);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Bettershot$");
  if(Equal(data, FM_AmmoID))    return(SSTM);
  if(Equal(data, FM_AmmoLoad))  return(100);
  if(Equal(data, FM_Condition)) return(rifleupgrade);

  if(Equal(data, FM_Reload))    return(300);
  if(Equal(data, FM_Recharge))  return(3);
  if(Equal(data, FM_Aim))       return(2);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(3);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-5,+5) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("M249",nil,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(20) ?? 0) + (RandomX(-5,+5) ?? 0);
  var x,y; user->~WeaponEnd(x,y);
  var ammo = CreateObject(SHT9,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 2),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("M249",nil,ammo);
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
