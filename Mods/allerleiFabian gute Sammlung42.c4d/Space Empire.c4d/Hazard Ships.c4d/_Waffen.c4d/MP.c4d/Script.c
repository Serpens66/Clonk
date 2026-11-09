/*-- MP --*/

#strict

#include WEPN

local rifleupgrade;

// Anzeige in der Hand
public func HandSize() { return(900); }
public func HandX()    { return(5000); }
public func HandY()    { return(0); }
public func BarrelYOffset() { return(-1000); }

public func OnReload(int i)
{
  Sound("PistolLoad");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(32);
  //if(data == FM_Condition) return(!rifleupgrade);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(5);
  if(data == FM_Auto) 	   return(1);


  if(data == FM_Damage)    return(6);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(480);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData2(int data)
{
  if(data == FM_Name)      return("$Quickshot$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(32);
  if(data == FM_Condition) return(rifleupgrade);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(3);
  if(data == FM_Auto) 	   return(1);


  if(data == FM_Damage)    return(3);

  return(Default(data));
}

public func BOTData2(int data)
{
  if(data == BOT_Range)    return(480);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func FMData3(int data)
{
  if(data == FM_Name)      return("$StandardStructure$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(32);
 // if(data == FM_Condition) return(!rifleupgrade);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(5);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(6);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)      return("$QuickshotStructure$");
  if(data == FM_AmmoID)    return(STAM);
  if(data == FM_AmmoLoad)  return(32);
  if(data == FM_Condition) return(rifleupgrade);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(3);
  if(data == FM_Auto) 	   return(1);

  if(data == FM_Damage)    return(3);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-4,+4);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("MPShot",0,ammo);
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-6,+6);
  
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));
  if(!Random(5)) user->AimUp(user,1,"ControlConf");
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("MPShot",0,ammo);
}

public func Fire3()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-4,+4);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("MPShot",0,ammo);
}

public func Fire4()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-6,+6);
  
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,500,5,300,GetFMData(FM_Damage, 1));
  if(!Random(5)) user->AimUp(user,1,"ControlConf");
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("MPShot",0,ammo);
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
