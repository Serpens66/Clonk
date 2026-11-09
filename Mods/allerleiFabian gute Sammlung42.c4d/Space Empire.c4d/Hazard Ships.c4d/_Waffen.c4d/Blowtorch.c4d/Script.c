/*-- Schweißgerät --*/

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
  if(rifleupgrade) Sound("EnergyRifleLoad");
  else Sound("FlamerLoad");
  }

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(GSAM);
  if(data == FM_AmmoLoad)  return(30);
  if(data == FM_Condition) return(!rifleupgrade);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(50);
  if(data == FM_AmmoUsage) return(5);

  if(data == FM_Damage)    return(20);

  return(Default(data));
}


public func FMData2(int data)
{
  if(data == FM_Name)      return("$Lasershot$");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(30);
  if(data == FM_Condition) return(rifleupgrade);

  if(data == FM_Reload)    return(80);
  if(data == FM_Recharge)  return(50);
  if(data == FM_AmmoUsage) return(5);
  
  if(data == FM_Damage)    return(40);

  return(Default(data));
}

public func BOTData1(int data)
{
  if(data == BOT_Range)    return(480);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-4,+4);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(BTS_,x,y,GetController(user));
  ammo->Launch(angle,250,50,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;

  // Sound
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-6,+6);
  
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(BTS_,x,y,GetController(user));
  ammo->Launch(angle,250,50,5,300,GetFMData(FM_Damage, 1));
  if(!Random(5)) user->AimUp(user,1,"ControlConf");
  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;

  // Sound
  if(!Random(2)) Sound("PRReparatur1");
  if(!Random(1)) Sound("PRReparatur2");
  Sound("PRReparatur3");
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(uid == KLAS && !rifleupgrade) return("$KLASUpgradeDesc$");
}

public func Upgrade(id uid) {
  var own = GetOwner(Contained());
  if(uid == KLAS) {
    SetFireMode(2);
    rifleupgrade=true;
    if(Contained()) HelpMessage(own, "$KLASUpgraded$", Contained());
    return(true);
  }
}
