/*-- Wriath Stunner --*/

#strict

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelYOffset() { return(-2700); }

public func OnReload(int i)
{
//  if(i == 1) Sound("PistolLoad");
//  if(i == 2) Sound("PistolLoad");
}
/*
public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(10);
  if(data == FM_AmmoUsage) return(10);

  if(data == FM_Reload)    return(30);
  if(data == FM_Recharge)  return(30);

  if(data == FM_Damage)    return(0);

  return(Default(data));
}
*/

// test ob eigentlich dasselbe richtig zielt
public func FMData1(int data)
{
  if(data == FM_Name)       return("$Standard$");
  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(10);

  if(data == FM_Reload)     return(30);
  if(data == FM_Recharge)   return(30);

  if(data == FM_AmmoUsage)  return(10);
  if(data == FM_Damage)     return(0);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_Power)    return(BOT_Power_1);

  return(Default(data));
}

public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-5,+5);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(WRSF,x,y,GetController(user));
  ammo->Launch(angle,250,600,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("wr_pistolfire",0,ammo);
  Reload();
}
