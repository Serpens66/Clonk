/*-- Pumpgun --*/

#strict 3

#include WEPN

local rifleupgrade;

public func HandSize() { return(800);  }
public func HandX()    { return(6000); }
public func HandY()    { return(500);  }
public func BarrelYOffset(){return(-1000);}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(SHTM);
  if(Equal(data, FM_AmmoLoad))  return(6);
  if(Equal(data, FM_AmmoUsage)) return(1);

  if(Equal(data, FM_Reload))    return(80);
  if(Equal(data, FM_Recharge))  return(30);

  if(Equal(data, FM_Damage))    return(28);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(200);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Cluster$");
  if(Equal(data, FM_AmmoID))    return(GRAM);
  if(Equal(data, FM_AmmoLoad))  return(4);
  if(Equal(data, FM_AmmoUsage)) return(2);

  if(Equal(data, FM_Reload))    return(70);
  if(Equal(data, FM_Recharge))  return(70);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage))    return(14);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(100);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(20);
  var ammo;

  var dir = (GetDir(user) ?? 0)*2-1;
  
  for(var i=nil; (i ?? 0)<15; S42MigrationStep(i, 1, true))
  {
    ammo = CreateObject(SHT1, x, y, GetController(user));
    ammo -> Launch((angle ?? 0)+(RandomX(-7, 7) ?? 0), 250+(Random(100) ?? 0), 80+(Random(200) ?? 0), 3, 400, GetFMData(FM_Damage, 1), 1);

	if(!((i ?? 0)%3))
		BulletCasing((dir ?? 0)*6, 3, -(dir ?? 0)*((Random(15) ?? 0)+5), -(Random(15) ?? 0)-15, 5);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("PumpgunFire");
}

public func Fire2()    // Clusterschuss
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(20);
  var dir = (GetDir(user) ?? 0)*2-1;
  var ammo, xdir, ydir;

  // Effekte
  for(var i=nil; (i ?? 0)<6; S42MigrationStep(i, 1, true))
  {
    ammo = CreateObject(CGFR, x, y, GetController(user));
    ammo->SetExplodeSize(GetFMData(FM_Damage, 2));
    var rangle = (angle ?? 0)+(RandomX(-7,7) ?? 0);
    xdir = Sin(rangle, RandomX(100, 140));
    ydir = -(Cos(rangle, RandomX(100, 140)) ?? 0);
    SetSpeed(xdir, ydir, ammo);
    BulletCasing((dir ?? 0)*6, 3, -(dir ?? 0)*((Random(15) ?? 0)+5), -(Random(15) ?? 0)-15, 5);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("PumpgunFire");
}

/* Upgrades */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic();
  if(uid = KRFL) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
    SetUpgradeGraphic();
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    return(true);
  }
}


// Soundeffekte

public func OnReload()
{
  Sound("PumpgunLoad");
}
