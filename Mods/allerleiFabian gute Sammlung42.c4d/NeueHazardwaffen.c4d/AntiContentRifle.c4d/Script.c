/*-- Pistole --*/

#strict

#include WEPN


// Anzeige in der Hand
public func HandSize() { return(800); }
public func HandX()    { return(5500); }
public func HandY()    { return(500); }

public func OnReload(int i)
{
  Sound("PistolLoad");
}

public func FMData1(int data)
{
  if(data == FM_Name)      return("$Standard$");
  if(data == FM_AmmoID)    return(ENAM);
  if(data == FM_AmmoLoad)  return(60);

  if(data == FM_Reload)    return(200);
  if(data == FM_Recharge)  return(100);

  if(data == FM_AmmoUsage) return(15);
  if(data == FM_Damage)    return(0);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
 // if(data == BOT_DmgType)  return(DMG_Bio);
 // if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}


public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-1,+1);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(ACST,x,y-5,GetController(user));
  ammo->Launch(angle,250,600,5,300,GetFMData(FM_Damage, 1),0,1);

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  // Sound
  Sound("EnergyRifle");
}



/* Treffer-Callbacks */

