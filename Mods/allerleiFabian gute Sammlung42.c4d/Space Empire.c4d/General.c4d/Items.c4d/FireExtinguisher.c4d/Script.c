/*-- Feuerlöscher --*/

#strict 3

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(500);  }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$FlameThrower$");
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(25);
  if(Equal(data, FM_Reload))   return(100);
  if(Equal(data, FM_AmmoUsage))   return(1);
  if(Equal(data, FM_AmmoRate))   return(5);
  if(Equal(data, FM_Recharge)) return(5);
  if(Equal(data, FM_Auto))     return(true);

  if(Equal(data, FM_Damage))   return(nil);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(SFE2, x,y, GetController(user));
  var flame2 = CreateObject(SFE2, x,y, GetController(user));
  var xdir = (+(Sin(angle,35) ?? 0) ?? 0)+(GetXDir(user) ?? 0);
  var ydir = -(Cos(angle,35) ?? 0)+(GetYDir(user) ?? 0);
  var xdir2 = (+(Sin(angle,45) ?? 0) ?? 0)+(GetXDir(user) ?? 0);
  var ydir2 = -(Cos(angle,45) ?? 0)+(GetYDir(user) ?? 0);

  flame -> Launch(xdir,ydir,nil, GetFMData(FM_Damage, 1));
  flame2 -> Launch(xdir2,ydir2,nil, GetFMData(FM_Damage, 1));
}


