/*-- MP --*/

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
  Sound("PistolLoad");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(32);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(5);
  if(Equal(data, FM_Auto)) 	   return(1);


  if(Equal(data, FM_Damage))    return(6);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(480);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}


public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Quickshot$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(32);
  if(Equal(data, FM_Condition)) return(rifleupgrade);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(3);
  if(Equal(data, FM_Auto)) 	   return(1);


  if(Equal(data, FM_Damage))    return(3);

  return(Default(data));
}

public func BOTData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(480);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}


public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$StandardStructure$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(32);
 // if(data == FM_Condition) return(!rifleupgrade);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(5);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(6);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$QuickshotStructure$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(32);
  if(Equal(data, FM_Condition)) return(rifleupgrade);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(3);
  if(Equal(data, FM_Auto)) 	   return(1);

  if(Equal(data, FM_Damage))    return(3);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-4,+4) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("MPShot",nil,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-6,+6) ?? 0);
  
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));
  if(!Random(5)) user->AimUp(user,1,"ControlConf");
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("MPShot",nil,ammo);
}

public func Fire3()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-4,+4) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("MPShot",nil,ammo);
}

public func Fire4()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-6,+6) ?? 0);
  
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));
  if(!Random(5)) user->AimUp(user,1,"ControlConf");
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("MPShot",nil,ammo);
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
