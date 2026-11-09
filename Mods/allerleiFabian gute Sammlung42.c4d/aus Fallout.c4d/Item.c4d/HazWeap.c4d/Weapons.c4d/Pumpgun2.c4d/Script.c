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
  if(Equal(data, FM_AmmoLoad))  return(2);

  if(Equal(data, FM_Reload))    return(150);
  if(Equal(data, FM_Recharge))  return(15);

  if(Equal(data, FM_Damage))    return(25);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(200);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var x,y; user->~WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
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
  Sound("Shotgun");
}


// Soundeffekte

public func OnReload()
{
  Sound("Load");
}

public func IsMupfWeapon()
{ return(true); }