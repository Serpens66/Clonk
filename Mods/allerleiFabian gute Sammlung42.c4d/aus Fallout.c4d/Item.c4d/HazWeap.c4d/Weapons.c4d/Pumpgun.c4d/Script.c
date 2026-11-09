
/*-- Pumpgun --*/

#strict

#include WEPN

local rifleupgrade;

public func HandSize() { return(800);  }
public func HandX()    { return(6000); }
public func HandY()    { return(500);  }
public func BarrelYOffset(){return(-1000);}

public func Selection()
{
  Sound("PumpgunSelect");
  return(1);
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(SHTM);
  if(data == FM_AmmoLoad)  return(8);

  if(data == FM_Reload)    return(100);
  if(data == FM_Recharge)  return(60);

  //if(data == FM_Condition) return(!GetUpgrade(KRFL));

  if(data == FM_Damage)    return(20);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(200);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return("$Automatik$");
  if(data == FM_AmmoID)    return(SHTM);
  if(data == FM_AmmoLoad)  return(8);

  if(data == FM_Reload)    return(100);
  if(data == FM_Recharge)  return(30);

  if(data == FM_Condition) return(GetUpgrade(KRFL));

  if(data == FM_Damage)    return(10);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(200);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var x,y; user->~WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var ammo;

  var dir = GetDir(user)*2-1;
  
  for(var i=0; i<15; i++)
  {
    ammo = CreateObject(SHT1, x, y, GetController(user));
    ammo -> Launch(angle+RandomX(-7, 7), 250+Random(100), 80+Random(200), 3, 400, GetFMData(FM_Damage, 1), 1);

	if(!(i%3))
		BulletCasing(dir*6, 3, -dir*(Random(15)+5), -Random(15)-15, 5);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("PumpgunFire");
}

public func Fire2()    // Automatikschuss
{ 
  var user = GetUser();
  var x,y; user->~WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var ammo;

  var dir = GetDir(user)*2-1;
  
  for(var i=0; i<15; i++)
  {
    ammo = CreateObject(SHT1, x, y, GetController(user));
    ammo -> Launch(angle+RandomX(-7, 7), 250+Random(100), 80+Random(200), 3, 400, GetFMData(FM_Damage, 2), 1);

	if(!(i%3))
		BulletCasing(dir*6, 3, -dir*(Random(15)+5), -Random(15)-15, 5);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("Pumpgun");
}

/* Upgrades */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(false);
  
  if(uid == KRFL) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
  if(uid = KRFL) {
    SetFireMode(2);
    var own = GetOwner(Contained());
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
  if(uid == KRFL) {
    SetFireMode(1);
    return(true);
  }
}


// Soundeffekte

public func OnReload()
{
  Sound("PumpgunLoad");
}

public func IsMupfWeapon()
{ return(true); }
