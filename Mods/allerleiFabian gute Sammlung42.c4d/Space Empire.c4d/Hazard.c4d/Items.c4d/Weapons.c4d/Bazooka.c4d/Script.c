/*-- Bazooka --*/

#strict 3

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(850); }
public func HandX()    { return(3000); }
public func HandY()    { return(nil);  }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$Missile$");
  if(Equal(data, FM_Icon))     return(MISS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(2);

  if(Equal(data, FM_Reload))   return(200);
  if(Equal(data, FM_Recharge))   return(20);

  if(Equal(data, FM_Aim))    return(1);

 // if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(Equal(data, FM_Damage)) return(40);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$HMissile$");
  if(Equal(data, FM_Icon))     return(HMIS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(200);
  if(Equal(data, FM_Recharge))   return(20);

  if(Equal(data, FM_Aim))    return(1);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage)) return(45);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(700);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}


public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$MissileStructure$");
  if(Equal(data, FM_Icon))     return(MISS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge))   return(20);

  if(Equal(data, FM_Aim))    return(1);

 // if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(Equal(data, FM_Damage)) return(15);

  return(Default(data));
}


// findet militärgebäude... zu mächtig, nur KI darf dies nutzen
public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$HMissileStructure$");
  if(Equal(data, FM_Icon))     return(HMIS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge))   return(20);

  if(Equal(data, FM_Aim))    return(1);

  if(Equal(data, FM_Condition)) {if(Or(Equal(GetPlayerType(GetOwner()), C4PT_Script), Equal(GetOwner(), -1)))return(1);return(nil);}

  if(Equal(data, FM_Damage)) return(12);

  return(Default(data));
}

public func BotData4(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);
  if(Equal(data, BOT_EMP)) return(1);

  return(Default(data));
}

public func FMData5(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$Bunker$");
  if(Equal(data, FM_Icon))     return(MISS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(10);

  if(Equal(data, FM_Auto))      return(true);

  if(Equal(data, FM_Reload))   return(1200);
  if(Equal(data, FM_Recharge))   return(30);

  // if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Aim))    return(1);


  if(Equal(data, FM_Damage)) return(40);

  return(Default(data));
}

public func BotData5(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(100);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData6(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$RCMissile$");
  if(Equal(data, FM_Icon))     return(RMIS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge))   return(1);

  if(Equal(data, FM_Aim))    return(1);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage)) return(37);

  return(Default(data));
}

public func BotData6(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData7(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("$RCMissileStructure$");
  if(Equal(data, FM_Icon))     return(RMI_);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(1);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge))   return(1);

  if(Equal(data, FM_Aim))    return(1);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage)) return(15);

  return(Default(data));
}




public func Fire1() { LaunchRocket(MISS,Contained()->~AimAngle(20), GetFMData(FM_Damage, 1)); }
public func Fire2() { LaunchRocket(HMIS,Contained()->~AimAngle(20), GetFMData(FM_Damage, 2)); }
public func Fire3() { LaunchRocket(MIS_,Contained()->~AimAngle(20), GetFMData(FM_Damage, 3)); }
public func Fire4() { LaunchRocket(HMI_,Contained()->~AimAngle(20), GetFMData(FM_Damage, 4)); }
public func Fire5() { LaunchRocketBunker(IIII,Contained()->~AimAngle(2), GetFMData(FM_Damage, 1)); }
public func Fire6() { LaunchRocket(RMIS,Contained()->~AimAngle(20), GetFMData(FM_Damage, 3)); }
public func Fire7() { LaunchRocket(RMI_,Contained()->~AimAngle(20), GetFMData(FM_Damage, 3)); }

public func LaunchRocket(rid, angle, dmg) {

  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);
  // not too near to clonk
  x = (x ?? 0)*3/2;
  y = (y ?? 0)*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->SetPosition((user->GetX() ?? 0)+(x ?? 0),(user->GetY() ?? 0)+(y ?? 0));
  rocket->Launch(angle, dmg);
  if(Or(Equal(rid, RMIS), Equal(rid, RMI_)))
    rocket->ActivateControl(user);

  // Sicht verfolgen wenn kein automatischer Schuss
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), rocket);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = (ax ?? 0)-(x ?? 0);
  ydir = (ay ?? 0)-(y ?? 0);

  CreateParticle("Thrust",ax,ay,(xdir ?? 0)/2,(ydir ?? 0)/2,80,RGBa(255,200,200,nil),nil,nil);

  for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",(ax ?? 0)+(Sin(angle,rand) ?? 0),(ay ?? 0)-(Cos(angle,rand) ?? 0),
                   RandomX(nil,2*(xdir ?? 0)),RandomX(nil,2*(ydir ?? 0)),
                   RandomX(80,140),RGBa(220,200,180,nil),nil,nil);
  }

  // Sound
  Sound("Rocket");
}

public func LaunchRocketBunker(rid, angle, dmg) {

  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);
  // not too near to clonk
  x = (x ?? 0)*3/2;
  y = (y ?? 0)*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));

  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = (ax ?? 0)-(x ?? 0);
  ydir = (ay ?? 0)-(y ?? 0);

  CreateParticle("Thrust",ax,ay,(xdir ?? 0)/2,(ydir ?? 0)/2,80,RGBa(255,200,200,nil),nil,nil);

  for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
    var rand = RandomX(-5,+5);
    CreateParticle("Smoke2",(ax ?? 0)+(Sin(angle,rand) ?? 0),(ay ?? 0)-(Cos(angle,rand) ?? 0),
                   RandomX(nil,2*(xdir ?? 0)),RandomX(nil,2*(ydir ?? 0)),
                   RandomX(200,260),RGBa(40,40,40,nil),nil,nil);
  }

  // Sound
  Sound("Rocket");
}




/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KLAS)) return("$KLASUpgradeDesc$");
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
  SetUpgradeGraphic();
  if(Equal(uid, KLAS)) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(Equal(uid, KRFL)) {
    SetFireMode(6);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
  SetUpgradeGraphic();
  if(Equal(uid, KLAS)) {
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    return(true);
  }
}

// Soundeffekte

public func OnReload()
{
  Sound("RocketLoad");
}


public func IsWeaponProduct() { return(1); }