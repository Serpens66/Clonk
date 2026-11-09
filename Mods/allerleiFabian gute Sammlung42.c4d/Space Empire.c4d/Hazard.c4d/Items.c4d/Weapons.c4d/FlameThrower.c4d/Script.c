/*-- Flammenwefer --*/

#strict

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(500);  }

public func FMData1(int data)
{
  if(data == FM_Name)     return("$FlameThrower$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(50);
  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Damage)   return(22);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(150);
  if(data == BOT_DmgType)  return(DMG_Fire);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)     return("$FlameBall$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(25);
  if(data == FM_AmmoUsage)  return(25);
  
  if(data == FM_Reload)   return(60); 
  if(data == FM_Recharge) return(25);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage)   return(15);

  return(Default(data));
}

public func BotData3(int data)
{
  if(data == BOT_Range)    return(350);
  if(data == BOT_DmgType)  return(DMG_Fire);
  if(data == BOT_Ballistic)  return(true);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)     return("$SlimeThrower$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(50);

  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Condition) return(GetUpgrade(KSLM));

  if(data == FM_Damage)   return(30);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(150);
  if(data == BOT_DmgType)  return(DMG_Bio);
  if(data == BOT_Power)    return(BOT_Power_3);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)     return("$FlameThrowerStructure$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(50);
  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true); 
  if(data == FM_AmmoUsage)  return(2);
  if(data == FM_AmmoRate)  return(1); 

  if(data == FM_Damage)   return(11);

  return(Default(data));
}

public func FMData6(int data)
{
  if(data == FM_Name)     return("$FlameBallStructure$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(25);
  if(data == FM_AmmoUsage)  return(25);
  
  if(data == FM_Reload)   return(60); 
  if(data == FM_Recharge) return(25);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage)   return(7);

  return(Default(data));
}

public func FMData5(int data)
{
  if(data == FM_Name)     return("$SlimeThrowerStructure$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(50);

  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Condition) return(GetUpgrade(KSLM));

  if(data == FM_Damage)   return(15);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 1));
}

public func Fire3()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(10) + RandomX(-2,+2);
  var flame = CreateObject(FLMB, x,y, GetController(user));
  var xdir = +Sin(angle,RandomX(80,100))+GetXDir(user)/2;
  var ydir = -Cos(angle,RandomX(80,100))+GetYDir(user)/2;

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 3));

  Sound("Inflame");
}

public func Fire2()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);

  flame -> Launch(xdir,ydir,true, GetFMData(FM_Damage, 2));
}

public func Fire4()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(_FLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 4));
}

public func Fire6()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(10) + RandomX(-2,+2);
  var flame = CreateObject(_LMB, x,y, GetController(user));
  var xdir = +Sin(angle,RandomX(80,100))+GetXDir(user)/2;
  var ydir = -Cos(angle,RandomX(80,100))+GetYDir(user)/2;

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 6));

  Sound("Inflame");
}

public func Fire5()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(_FLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);

  flame -> Launch(xdir,ydir,true, GetFMData(FM_Damage, 5));
}


/* Sounds */

public func OnAutoStart(int i)
{
  if(i == 1) Sound("Flamer",0,0,0,0,1);
  if(i == 2) Sound("BioFlamer",0,0,0,0,1);
  if(i == 4) Sound("Flamer",0,0,0,0,1);
  if(i == 5) Sound("BioFlamer",0,0,0,0,1);
}

public func OnAutoStop(int i)
{
  if(i == 1) Sound("Flamer",0,0,0,0,-1);
  if(i == 2) Sound("BioFlamer",0,0,0,0,-1);
  if(i == 4) Sound("Flamer",0,0,0,0,-1);
  if(i == 5) Sound("BioFlamer",0,0,0,0,-1);
}

public func OnReload()
{
  Sound("FlamerLoad");
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KSLM) return("$KSLMUpgradeDesc$");
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(uid == KSLM) {
    SetFireMode(2);
    if(Contained()) HelpMessage(own,"$KSLMUpgraded$",Contained());
    return(true);
  }
  if(uid = KRFL) {
    SetFireMode(3);
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
  if(uid == KSLM) {
      SetUpgradeGraphic();
    SetFireMode(1);
    return(true);
  }
  if(uid == KRFL) {
    SetFireMode(1);
    return(true);
  }
}
