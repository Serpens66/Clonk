/*-- Flammenwefer --*/

#strict 3

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(500);  }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$FlameThrower$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(50);
  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(5);
  if(Equal(data, FM_Auto))     return(true);

  if(Equal(data, FM_Damage))   return(22);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(150);
  if(Equal(data, BOT_DmgType))  return(DMG_Fire);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$FlameBall$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(25);
  if(Equal(data, FM_AmmoUsage))  return(25);
  
  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(25);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage))   return(15);

  return(Default(data));
}

public func BotData3(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(350);
  if(Equal(data, BOT_DmgType))  return(DMG_Fire);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$SlimeThrower$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(50);

  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(5);
  if(Equal(data, FM_Auto))     return(true);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))   return(30);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(150);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$FlameThrowerStructure$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(50);
  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(5);
  if(Equal(data, FM_Auto))     return(true);
  if(Equal(data, FM_AmmoUsage))  return(2);
  if(Equal(data, FM_AmmoRate))  return(1);

  if(Equal(data, FM_Damage))   return(11);

  return(Default(data));
}

public func FMData6(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$FlameBallStructure$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(25);
  if(Equal(data, FM_AmmoUsage))  return(25);
  
  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(25);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage))   return(7);

  return(Default(data));
}

public func FMData5(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$SlimeThrowerStructure$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(50);

  if(Equal(data, FM_Reload))   return(60);
  if(Equal(data, FM_Recharge)) return(5);
  if(Equal(data, FM_Auto))     return(true);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))   return(15);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = (+(Sin(angle,35) ?? 0) ?? 0)+(GetXDir(user) ?? 0);
  var ydir = -(Cos(angle,35) ?? 0)+(GetYDir(user) ?? 0);

  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 1));
}

public func Fire3()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-2,+2) ?? 0);
  var flame = CreateObject(FLMB, x,y, GetController(user));
  var xdir = (+(Sin(angle,RandomX(80,100)) ?? 0) ?? 0)+(GetXDir(user) ?? 0)/2;
  var ydir = -(Cos(angle,RandomX(80,100)) ?? 0)+(GetYDir(user) ?? 0)/2;

  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 3));

  Sound("Inflame");
}

public func Fire2()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = (+(Sin(angle,35) ?? 0) ?? 0)+(GetXDir(user) ?? 0);
  var ydir = -(Cos(angle,35) ?? 0)+(GetYDir(user) ?? 0);

  flame -> Launch(xdir,ydir,true, GetFMData(FM_Damage, 2));
}

public func Fire4()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(_FLM, x,y, GetController(user));
  var xdir = (+(Sin(angle,35) ?? 0) ?? 0)+(GetXDir(user) ?? 0);
  var ydir = -(Cos(angle,35) ?? 0)+(GetYDir(user) ?? 0);

  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 4));
}

public func Fire6()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-2,+2) ?? 0);
  var flame = CreateObject(_LMB, x,y, GetController(user));
  var xdir = (+(Sin(angle,RandomX(80,100)) ?? 0) ?? 0)+(GetXDir(user) ?? 0)/2;
  var ydir = -(Cos(angle,RandomX(80,100)) ?? 0)+(GetYDir(user) ?? 0)/2;

  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 6));

  Sound("Inflame");
}

public func Fire5()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(_FLM, x,y, GetController(user));
  var xdir = (+(Sin(angle,35) ?? 0) ?? 0)+(GetXDir(user) ?? 0);
  var ydir = -(Cos(angle,35) ?? 0)+(GetYDir(user) ?? 0);

  flame -> Launch(xdir,ydir,true, GetFMData(FM_Damage, 5));
}


/* Sounds */

public func OnAutoStart(int i)
{
  i ??= 0;
  if(Equal(i, 1)) Sound("Flamer",nil,nil,nil,nil,1);
  if(Equal(i, 2)) Sound("BioFlamer",nil,nil,nil,nil,1);
  if(Equal(i, 4)) Sound("Flamer",nil,nil,nil,nil,1);
  if(Equal(i, 5)) Sound("BioFlamer",nil,nil,nil,nil,1);
}

public func OnAutoStop(int i)
{
  i ??= 0;
  if(Equal(i, 1)) Sound("Flamer",nil,nil,nil,nil,-1);
  if(Equal(i, 2)) Sound("BioFlamer",nil,nil,nil,nil,-1);
  if(Equal(i, 4)) Sound("Flamer",nil,nil,nil,nil,-1);
  if(Equal(i, 5)) Sound("BioFlamer",nil,nil,nil,nil,-1);
}

public func OnReload()
{
  Sound("FlamerLoad");
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KSLM)) return("$KSLMUpgradeDesc$");
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(Equal(uid, KSLM)) {
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
  if(Equal(uid, KSLM)) {
      SetUpgradeGraphic();
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    return(true);
  }
}
