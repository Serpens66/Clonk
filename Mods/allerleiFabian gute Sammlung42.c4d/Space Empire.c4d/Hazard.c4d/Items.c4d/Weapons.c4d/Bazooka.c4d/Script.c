/*-- Bazooka --*/

#strict

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(850); }
public func HandX()    { return(3000); }
public func HandY()    { return(0);  }

public func FMData1(int data)
{
  if(data == FM_Name)   return("$Missile$");
  if(data == FM_Icon)     return(MISS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(2);

  if(data == FM_Reload)   return(200);
  if(data == FM_Recharge)   return(20);

  if(data == FM_Aim)    return(1);

 // if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(data == FM_Damage) return(40);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)   return("$HMissile$");
  if(data == FM_Icon)     return(HMIS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(200);
  if(data == FM_Recharge)   return(20);

  if(data == FM_Aim)    return(1);

  if(data == FM_Condition) return(GetUpgrade(KLAS));

  if(data == FM_Damage) return(45);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(700);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}


public func FMData3(int data)
{
  if(data == FM_Name)   return("$MissileStructure$");
  if(data == FM_Icon)     return(MISS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge)   return(20);

  if(data == FM_Aim)    return(1);

 // if(data == FM_Condition) return(!GetUpgrade(KLAS));

  if(data == FM_Damage) return(15);

  return(Default(data));
}


// findet militärgebäude... zu mächtig, nur KI darf dies nutzen
public func FMData4(int data)
{
  if(data == FM_Name)   return("$HMissileStructure$");
  if(data == FM_Icon)     return(HMIS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge)   return(20);

  if(data == FM_Aim)    return(1);

  if(data == FM_Condition) {if(GetPlayerType(GetOwner())==C4PT_Script || GetOwner()==-1)return(1);return(0);}

  if(data == FM_Damage) return(12);

  return(Default(data));
}

public func BotData4(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);
  if(data == BOT_EMP) return(1);

  return(Default(data));
}

public func FMData5(int data)
{
  if(data == FM_Name)   return("$Bunker$");
  if(data == FM_Icon)     return(MISS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(10);

  if(data == FM_Auto)      return(true);

  if(data == FM_Reload)   return(1200);
  if(data == FM_Recharge)   return(30);

  // if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Aim)    return(1);


  if(data == FM_Damage) return(40);

  return(Default(data));
}

public func BotData5(int data)
{
  if(data == BOT_Range)    return(100);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData6(int data)
{
  if(data == FM_Name)   return("$RCMissile$");
  if(data == FM_Icon)     return(RMIS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge)   return(1);

  if(data == FM_Aim)    return(1);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage) return(37);

  return(Default(data));
}

public func BotData6(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Explosion);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData7(int data)
{
  if(data == FM_Name)   return("$RCMissileStructure$");
  if(data == FM_Icon)     return(RMI_);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(1);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge)   return(1);

  if(data == FM_Aim)    return(1);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage) return(15);

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
  x = x*3/2;
  y = y*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->SetPosition(user->GetX()+x,user->GetY()+y);
  rocket->Launch(angle, dmg);
  if(rid == RMIS || rid == RMI_)
    rocket->ActivateControl(user);

  // Sicht verfolgen wenn kein automatischer Schuss
    if(user==GetCursor(GetOwner()))
    SetPlrView(GetOwner(), rocket);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
  }

  // Sound
  Sound("Rocket");
}

public func LaunchRocketBunker(rid, angle, dmg) {

  var user = Contained();
  var x,y;
  user->WeaponEnd(x,y);
  // not too near to clonk
  x = x*3/2;
  y = y*3/2;

  var rocket = CreateObject(rid,x,y,GetController(user));

  rocket->Launch(angle, dmg);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    var rand = RandomX(-5,+5);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(200,260),RGBa(40,40,40,0),0,0);
  }

  // Sound
  Sound("Rocket");
}




/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KLAS) return("$KLASUpgradeDesc$");
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
  SetUpgradeGraphic();
  if(uid == KLAS) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(uid == KRFL) {
    SetFireMode(6);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
  SetUpgradeGraphic();
  if(uid == KLAS) {
    SetFireMode(1);
    return(true);
  }
  if(uid == KRFL) {
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