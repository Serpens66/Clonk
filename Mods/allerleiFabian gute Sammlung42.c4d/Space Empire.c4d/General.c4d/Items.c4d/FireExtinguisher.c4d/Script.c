/*-- Feuerlöscher --*/

#strict

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(500);  }

public func FMData1(int data)
{
  if(data == FM_Name)     return("$FlameThrower$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(25);
  if(data == FM_Reload)   return(100);
  if(data == FM_AmmoUsage)   return(1);
  if(data == FM_AmmoRate)   return(5);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Damage)   return(0);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(SFE2, x,y, GetController(user));
  var flame2 = CreateObject(SFE2, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);
  var xdir2 = +Sin(angle,45)+GetXDir(user);
  var ydir2 = -Cos(angle,45)+GetYDir(user);

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 1));
  flame2 -> Launch(xdir2,ydir2,0, GetFMData(FM_Damage, 1));
}


