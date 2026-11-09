/*-- Pistole --*/

#strict 3

#include WEPN


// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(500); }

public func OnReload(int i)
{
  i ??= 0;
  Sound("PistolLoad");
}

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Standard$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(60);

  if(Equal(data, FM_Reload))    return(200);
  if(Equal(data, FM_Recharge))  return(100);

  if(Equal(data, FM_AmmoUsage)) return(10);
  if(Equal(data, FM_Damage))    return(nil);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("$Schock$");
  if(Equal(data, FM_AmmoID))    return(ENAM);
  if(Equal(data, FM_AmmoLoad))  return(120);

  if(Equal(data, FM_Reload))    return(250);
  if(Equal(data, FM_Recharge))  return(150);

  if(Equal(data, FM_AmmoUsage)) return(60);
  if(Equal(data, FM_Damage))    return(nil);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(600);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-1,+1) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(WAVE,x,(y ?? 0)-5,GetController(user));
  ammo->Launch(angle,250,600,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  // Sound
  Sound("EnergyRifle");
}

public func Fire2()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = (user->AimAngle(20) ?? 0) + (RandomX(-1,+1) ?? 0);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SKWV,x,(y ?? 0)-5,GetController(user));
  ammo->Launch(angle,70,600,5,300,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  // Sound
  Sound("EnergyRifle");
}

/* Treffer-Callbacks */

