/*-- Energiegewehr --*/

#strict 3

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(950); }
public func HandX()    { return(5000); }
public func HandY()    { return(2000); }
public func BarrelXOffset(){return(-2000);}
public func BarrelYOffset(){return(-2000);}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(20);

  if(Equal(data, FM_Reload))    return(140);
  if(Equal(data, FM_Recharge))  return(8);

  if(Equal(data, FM_AmmoUsage)) return(1);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Auto))      return(1);

  //if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(12);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(DMG_Energy);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Laser$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(25);

  if(Equal(data, FM_Reload))    return(100);
  if(Equal(data, FM_Recharge))  return(12);

  if(Equal(data, FM_AmmoUsage)) return(3);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Auto))      return(true);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(20);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(200);
  if(Equal(data, BOT_DmgType))  return(DMG_Energy);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$StandardStructure$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(20);

  if(Equal(data, FM_Reload))    return(140);
  if(Equal(data, FM_Recharge))  return(8);

  if(Equal(data, FM_AmmoUsage)) return(1);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Auto))      return(true);

  //if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(1);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$LaserStructure$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(25);

  if(Equal(data, FM_Reload))    return(100);
  if(Equal(data, FM_Recharge))  return(12);

  if(Equal(data, FM_AmmoUsage)) return(1);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Auto))      return(true);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(2);

  return(Default(data));
}


public func Fire1()    // Projektil-Dauerfeuer
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-2,+2) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT2,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec
  ammo->Launch(angle,180,400,6,80, GetFMData(FM_Damage, 1),nil,80);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire2()    // Laserschuss
{ 
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = (user->AimAngle(8) ?? 0) + (RandomX(-1,+1) ?? 0);
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGB(50,100,255));
  laser->Set(angle,7,GetBotData(BOT_Range,2),15,this());
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,nil));

  // Sound
  Sound("LaserShot",nil,laser);
}

public func Fire3()    // Projektil-Dauerfeuer auf Gebäude
{ 
  var user = GetUser();
  var angle = (user->~AimAngle(10) ?? 0) + (RandomX(-2,+2) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SH2_,x,y,GetController(user));
  // int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec
  ammo->Launch(angle,180,400,6,80, GetFMData(FM_Damage, 3),nil,80);

  // Effekte
  MuzzleFlash(30,user,x,y,angle,RGBa(96,64,255,nil));
  // Sound
  Sound("EnergyRifle",nil,ammo);
}

public func Fire4()    // Laserschuss auf Gebäude
{ 
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = (user->AimAngle(8) ?? 0) + (RandomX(-2,+1) ?? 0);
  var laser = CreateObject(LASG,x,y,GetController(user));
  laser->SetClrModulation(RGB(50,100,255));
  laser->Set(angle,7,GetBotData(BOT_Range,2),15,this());
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGBa(50,100,255,nil));

  // Sound
  Sound("LaserShot",nil,laser);
}


public func LaserStrike(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

public func LaserStrikeStructure(object pTarget) {	// Laser trifft
  DoDmg(GetFMData(FM_Damage, 4), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KLAS)) return("$KLASUpgradeDesc$");
}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic();
  if(Equal(uid, KLAS)) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
    SetUpgradeGraphic();
  if(Equal(uid, KLAS)) {
    SetFireMode(1);
    return(true);
  }
}

// Soundeffekte

public func OnReload()
{
  Sound("EnergyRifleLoad");
}
