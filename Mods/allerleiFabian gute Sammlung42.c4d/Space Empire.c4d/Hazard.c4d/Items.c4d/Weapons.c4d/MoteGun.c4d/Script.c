/*-- Partikelkanone --*/

#strict 3

#include WEPN

local laser; //der Laserstrahl

// Anzeige in der Hand
public func HandX() { return(5000); }
public func HandY() { return(500); }

//Toller EMPschuss
func FMData1(int data) {
  data ??= 0;
  if(Equal(data, FM_Name))       return("$Shockwave$");

  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(30);
  if(Equal(data, FM_AmmoUsage))  return(30);

  if(Equal(data, FM_Reload))     return(75);

  if(Equal(data, FM_Damage))     return(450);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(400);
  if(Equal(data, BOT_DmgType))  return(nil);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);
  if(Equal(data, BOT_EMP))      return(true);

  return(Default(data));
}

//Railgun
func FMData2(int data) {
  data ??= 0;
  if(Equal(data, FM_Name))       return("$Railgun$");

  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(100);
  if(Equal(data, FM_AmmoUsage))  return(50);

  if(Equal(data, FM_Reload))     return(110);
  if(Equal(data, FM_Recharge))   return(120);
  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))     return(165);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
  if(Equal(data, BOT_DmgType))  return(DMG_Energy);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);
  return(Default(data));
}

//Railgun gebäude
func FMData3(int data) {
  data ??= 0;
  if(Equal(data, FM_Name))       return("$RailgunStructure$");

  if(Equal(data, FM_AmmoID))     return(ENAM);
  if(Equal(data, FM_AmmoLoad))   return(100);
  if(Equal(data, FM_AmmoUsage))  return(50);

  if(Equal(data, FM_Reload))     return(110);
  if(Equal(data, FM_Recharge))   return(120);
  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))     return(30);

  return(Default(data));
}


//Railgun ist nur Lasergehaxxe.
public func Fire2() {
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(5);
  laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGBa(255,255,255,64));
  laser->Set(angle,8,700,60,this(),user);
  laser->SetContinuous(5);

  MuzzleFlash(300,user,x,y,angle,RGBa(64,64,255,nil));
  Sound("RailGunShot");
}

//EMP!
public func Fire1() {
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(15);
  var dir = (GetDir(user) ?? 0)*2-1;
  var ammo = CreateObject(SHKR,x,y,GetController(user));
  ammo->Launch(angle,50, GetFMData(FM_Damage, 2), 100, 60, nil);

  MuzzleFlash(50,user,x,y,angle,RGBa(40,40,255,nil));
  Sound("EMPShot");
}


//Railgun ist nur Lasergehaxxe.
public func Fire3() {
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(5);
  laser = CreateObject(LASG,x,y,GetController(user));
  laser->SetClrModulation(RGBa(255,255,255,64));
  laser->Set(angle,8,700,60,this(),user);
  laser->SetContinuous(5);

  MuzzleFlash(300,user,x,y,angle,RGBa(64,64,255,nil));
  Sound("RailGunShot");
}

public func LaserStrike(object pObj, int iTime) {
  //Schaden machen, natürlich tollen Energiebrazzelschaden!
  iTime ??= 0;
  var schwaecher = Max((iTime ?? 0)-25,nil);
  DoDmg(Max(nil,(GetFMData(FM_Damage, 2) ?? 0)/8-(schwaecher ?? 0)), DMG_Energy, pObj);

	// durch 8 weil er effektiv (nach Abzug des schwächer-werdens) 8mal trifft innerhalb dieser 60 Frames
  return(nil);
}

public func LaserStrikeStructure(object pObj, int iTime) {
  //Schaden machen, natürlich tollen Energiebrazzelschaden!
  iTime ??= 0;
  var schwaecher = Max((iTime ?? 0)-25,nil);
  DoDmg(Max(nil,(GetFMData(FM_Damage, 3) ?? 0)/8-(schwaecher ?? 0)), DMG_Energy, pObj);

	// durch 8 weil er effektiv (nach Abzug des schwächer-werdens) 8mal trifft innerhalb dieser 60 Frames
  return(nil);
}

public func IsFiring() { return(laser); }

// Soundeffekte

public func OnReload()
{
  if(Equal(firemode, 2))
    Sound("MoteGunLoad");
  if(Equal(firemode, 1))
    Sound("EMPLoad");
  if(Equal(firemode, 3))
    Sound("MoteGunLoad");
}

public func OnDeselect() {
  if(laser) {
    RemoveObject(laser);
  }
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