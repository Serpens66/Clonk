/*-- Minigun --*/

#strict 3

#include WEPN

// Anzeige in der Hand
public func HandSize() { return(900);  }
public func HandX()    { return(11000); }
public func HandY()    { return(-3000); }
public func BarrelYOffset(){return(-1000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("Standard");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(50);

  if(Equal(data, FM_Reload))     return(260);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);
  if(Equal(data, FM_Aim))        return(-1);

 // if(data == FM_Condition) return(!GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))     return(2);

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

// Daten für zweiten Feuermodus
public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("$Bouncing$");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(50);

  if(Equal(data, FM_Reload))     return(260);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);
  if(Equal(data, FM_Aim))        return(-1);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))     return(3);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(500);
  if(Equal(data, BOT_Power))    return(BOT_Power_LongLoad);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("$StandardStructure$");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(50);

  if(Equal(data, FM_Reload))     return(260);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);
  if(Equal(data, FM_Aim))        return(-1);

 // if(data == FM_Condition) return(!GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))     return(1);

  return(Default(data));
}


// Daten für vierte Feuermodus
public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("$BouncingStructure$");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(50);

  if(Equal(data, FM_Reload))     return(260);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);
  if(Equal(data, FM_Aim))        return(-1);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KSLM));

  if(Equal(data, FM_Damage))     return(1);

  return(Default(data));
}




// Feuern im ersten Feuermodus
public func Fire1()		// ungenaues aber heftiges Minigunfeuer
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (user->AimAngle(5) ?? 0)+(RandomX(-6,+6) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,100,600,4,100,GetFMData(FM_Damage, 1),nil,1);
//public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->~WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}

// Feuern im zweiten Feuermodus
public func Fire2()		// ungenaues aber heftiges Minigunfeuer mit Abpralleffekt
{ 
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (user->AimAngle(5) ?? 0)+(RandomX(-6,+6) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,300,600,2,100,GetFMData(FM_Damage, 2), nil,1,nil, 3);
//public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->~WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}

// Feuern im dritten Feuermodus
public func Fire3()		// ungenaues aber heftiges Minigunfeuer gegen Gebäude
{ 
  var schaden;
  if(!Random(3)) schaden = GetFMData(FM_Damage, 3);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (user->AimAngle(5) ?? 0)+(RandomX(-6,+6) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,100,600,4,100,schaden,nil,1);
//public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->~WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}

// Feuern im vierten Feuermodus
public func Fire4()		// ungenaues aber heftiges Minigunfeuer mit Abpralleffekt gegen Gebäude
{ 
  var schaden;
  if(!Random(3)) schaden = GetFMData(FM_Damage, 4);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (user->AimAngle(5) ?? 0)+(RandomX(-6,+6) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  user->WeaponEnd(x,y);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,300,600,2,100,schaden, nil,nil,nil, 3);
//public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->~WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}




// Soundeffekte

public func OnReload()
{
  Sound("MiniLoad");
}

public func OnAutoStart()
{
  Sound("MiniGun",nil,nil,nil,nil,1);
  Sound("MiniTurn",nil,nil,nil,nil,1);
}

public func OnAutoStop()
{
  Sound("MiniGun",nil,nil,nil,nil,-1);
  Sound("MiniTurn",nil,nil,nil,nil,-1);
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  //wir haben das Upgrade schon, bah.
  if(GetUpgrade(uid))
    return(nil);

  if(Equal(uid, KSLM)) return("$KSLMUpgradeDesc$");
}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic(nil,[KRFL]); // wenn Waffenteil ausgerüstet, gibts dafuer keine grafik, also nicht berücksichtigen
  var own = GetOwner(Contained());
  if(Equal(uid, KSLM)) {
    SetFireMode(2);
    if(Contained()) HelpMessage(own, "$KSLMUpgraded$", Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
    SetUpgradeGraphic(nil,[KRFL]); // wenn Waffenteil ausgerüstet, gibts dafuer keine grafik, also nicht berücksichtigen
  if(Equal(uid, KSLM)) {
    SetFireMode(1);
    return(true);
  }
}
