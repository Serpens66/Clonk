/*-- Pistole --*/

#strict 3

local partikelupgrade;

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelYOffset() { return(-2700); }

public func OnReload(int i)
{
  i ??= 0;
  if(Equal(i, 1)) Sound("PistolLoad");
  if(Equal(i, 2)) Sound("PistolLoad");
  if(Equal(i, 3)) Sound("GrenadeLoad");
  if(Equal(i, 4)) Sound("SlimeLoad");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(12);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(20);
 // if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(12);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);

  return(Default(data));
}

//Laser
public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$Laser$");
  if(Equal(data, FM_AmmoID))   return(ENAM);
  if(Equal(data, FM_AmmoLoad)) return(10);

  if(Equal(data, FM_Reload))   return(100);
  if(Equal(data, FM_Recharge)) return(16);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(26);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(DMG_Energy);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);

  return(Default(data));
}

//Granate
public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$GrenadeLauncher$");
  if(Equal(data, FM_AmmoID))   return(GRAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(80);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage))    return(30);

  return(Default(data));
}

public func BotData3(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(50);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$Slime$");
  if(Equal(data, FM_AmmoID))   return(STAM);
  if(Equal(data, FM_AmmoLoad)) return(10);

  if(Equal(data, FM_Reload))   return(80);
  if(Equal(data, FM_Recharge))   return(20);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))    return(20);

  return(Default(data));
}

public func BotData4(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(50);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);

  return(Default(data));
}


public func FMData5(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$StandardStructure$");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoLoad))  return(12);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(20);
 // if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(1);

  return(Default(data));
}


//Laser Gebäude
public func FMData6(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$LaserStructure$");
  if(Equal(data, FM_AmmoID))   return(ENAM);
  if(Equal(data, FM_AmmoLoad)) return(10);

  if(Equal(data, FM_Reload))   return(100);
  if(Equal(data, FM_Recharge)) return(16);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))    return(2);

  return(Default(data));
}

public func FMData7(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$Teleport$");
  if(Equal(data, FM_AmmoID))   return(ENAM);
  if(Equal(data, FM_AmmoLoad)) return(50);
  if(Equal(data, FM_AmmoUsage)) return(10);
  if(Equal(data, FM_Reload))   return(50);
  if(Equal(data, FM_Recharge))   return(50);

  if(Equal(data, FM_Condition)) return(partikelupgrade);

  //if(data == FM_Damage)    return(20);

  return(Default(data));
}

public func BotData7(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(nil);
  if(Equal(data, BOT_DmgType))  return;
  if(Equal(data, BOT_Power))  return;

  return(Default(data));
}



public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-1,+1) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,250,600,4,300,GetFMData(FM_Damage, 1),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("Pistol",nil,ammo);
}

public func Fire2()    // Laserschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  var laser = CreateObject(LASR,x,y,GetController(user));
  laser->Set(angle,8,350,15,this());
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGB(255,32,nil));

  // Sound
  Sound("LaserShot",nil,laser);
}

public func Fire3()    // Granatenwerfer
{ 
  var user = GetUser();
  var dir = (GetDir(user) ?? 0)*2-1;
  var angle = (user->AimAngle(5) ?? 0) + (RandomX(-5,5) ?? 0);
  var x,y; user->WeaponEnd(x,y);

  var xdir = Sin(angle,50);
  var ydir = -(Cos(angle,50) ?? 0);

  // Create & launch
  var grenade=CreateObject(GREN, (x ?? 0)+(xdir ?? 0)/10,(y ?? 0)+(ydir ?? 0)/10, GetController(user));
  grenade->Launch((xdir ?? 0)+(GetXDir(user) ?? 0)/2,(ydir ?? 0)+(GetYDir(user) ?? 0)/2, GetFMData(FM_Damage, 3));

  Sound("GrenadeFire");
}

public func Fire4()    // Schleimschuss
{ 
  var user = GetUser();
  var dir = (GetDir(user) ?? 0)*2-1;
  var angle = (user->AimAngle(5) ?? 0) + (RandomX(-5,5) ?? 0);
  var x,y; user->WeaponEnd(x,y);

  var xdir = Sin(angle,50);
  var ydir = -(Cos(angle,50) ?? 0);

  // Create & launch
  var slime=CreateObject(SLST, (x ?? 0)+(xdir ?? 0)/10,(y ?? 0)+(ydir ?? 0)/10, GetController(user));
  slime->Launch((xdir ?? 0)+(GetXDir(user) ?? 0)/2,(ydir ?? 0)+(GetYDir(user) ?? 0)/2,GetFMData(FM_Damage, 4));

  // Sound
  Sound("SlimeShot*");
}


public func Fire5()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-1,+1) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,250,600,4,300,GetFMData(FM_Damage, 5),nil,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing((dir ?? 0)*6,3,-(dir ?? 0)*5,-20,5);

  // Sound
  Sound("Pistol",nil,ammo);
}


public func Fire6()    // Laserschuss Gebäude
{ 
  var user = GetUser();
  var angle = user->AimAngle(5);
  var x,y; user->WeaponEnd(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  var laser = CreateObject(LASG,x,y,GetController(user));
  laser->Set(angle,8,350,15,this());
  // Effekte
  MuzzleFlash(40,user,x,y,angle,RGB(255,32,nil));

  // Sound
  Sound("LaserShot",nil,laser);
}

public func Fire7()    // Teleporter
{ 
  var user = GetUser();
  var dir = (GetDir(user) ?? 0)*2-1;
  var angle = (user->AimAngle(5) ?? 0) + (RandomX(-5,5) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  
  Sound("GrenadeFire");
  if(!PathFree(GetX(),GetY(),(GetX() ?? 0)+(x ?? 0),(GetY() ?? 0)+(y ?? 0)))
    return;//:O

  var xdir = Sin(angle,75);
  var ydir = -(Cos(angle,75) ?? 0);

  // Create & launch
  var teleport=CreateObject(_TE5, (x ?? 0)+(xdir ?? 0)/10,(y ?? 0)+(ydir ?? 0)/10, GetController(user));
  teleport->Launch((xdir ?? 0)+(GetXDir(user) ?? 0)/2,(ydir ?? 0)+(GetYDir(user) ?? 0)/2, GetFMData(FM_Damage, 3),user);
  SetCursor(GetOwner(user),teleport);
  SetPlrView(GetOwner(user),teleport);
  SetComDir(COMD_Down,user);
}




/* Treffer-Callbacks */

public func LaserStrike(object pTarget) {	// Laser trifft
  if(pTarget)
    DoDmg(GetFMData(FM_Damage, 2), DMG_Energy, pTarget);
  return(1);
}

public func LaserStrikeStructure(object pTarget) {	// Laser trifft
  if(pTarget)
    DoDmg(GetFMData(FM_Damage, 6), DMG_Energy, pTarget);
  return(1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  //wir haben das Upgrade schon, bah.
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KLAS)) return("$KLASUpgradeDesc$");
  if(Equal(uid, KSLM)) return("$KSLMUpgradeDesc$");
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
  if(And(Equal(uid, STRH), !partikelupgrade)) return("$PARTUpgradeDesc$");

}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic(nil,[KRFL,STRH]); // grafik update, aber da keine grafik fuer waffen und partikelupgrade vorhanden sind, diese bei grafiksuche ignorieren
  var own = GetOwner(Contained());
  if(Equal(uid, KLAS)) {
    SetFireMode(2);
    if(Contained()) HelpMessage(own, "$KLASUpgraded$", Contained());
    return(true);
  }
  if(Equal(uid, KSLM)) {
    SetFireMode(4);
    if(Contained()) HelpMessage(own, "$KSLMUpgraded$", Contained());
    return(true);
  }
  if(Equal(uid, KRFL)) {
    SetFireMode(3);
    if(Contained()) HelpMessage(own, "$KRFLUpgraded$", Contained());
    return(true);
  }
  if(Equal(uid, STRH)) {
    SetFireMode(7);
    partikelupgrade=true;
    if(Contained()) HelpMessage(own, "$TeleportUpgraded$", Contained());
    return(true);
  }

}

public func OnDowngrade(id uid) {
    SetUpgradeGraphic(nil,[KRFL,STRH]); // grafik update, aber da keine grafik fuer waffen und partikelupgrade vorhanden sind, diese bei grafiksuche ignorieren
  if(Equal(uid, KLAS)) {
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, KSLM)) {
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, STRH)) {
    SetFireMode(1);
    return(true);
  }
}
