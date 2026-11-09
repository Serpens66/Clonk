/*-- Granatenwerfer --*/

#strict 3

#include WEPN // Das Objekt ist eine Waffe  
local explodedelay;

func IsGrenadeLauncher() {return(1);} // ist ein Granatwerfer. Man kann im Kontextmenü die Zündzeit der Granate wählen.

func EditExplodedelay(iwert)
{
if(Equal(iwert, nil))
{explodedelay = nil;
PlayerMessage(GetOwner(),"Standardwerte wieder hergestellt (für jede Granate unterschiedlich)",Contained());
return;}

if(Equal(iwert, 1))
{S42MigrationAssign(explodedelay, "+=", 4);
explodedelay = BoundBy(explodedelay,4,400);
PlayerMessage(GetOwner(),"Zünder auf %v ms hochgestellt",Contained(),(explodedelay ?? 0)*1000/40);
return;}

if(Equal(iwert, 2))
{S42MigrationAssign(explodedelay, "-=", 4);
explodedelay = BoundBy(explodedelay,4,400);
PlayerMessage(GetOwner(),"Zünder auf %v ms runtergestellt",Contained(),(explodedelay ?? 0)*1000/40);
return;}
}

// Anzeige in der Hand
public func HandSize() { return(1050); }
public func HandX() { return(5000); }
public func HandY() { return(1000); }

public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$Grenade$");
  if(Equal(data, FM_Icon))     return(GREN);
  if(Equal(data, FM_AmmoID))   return(GRAM);
  if(Equal(data, FM_AmmoLoad)) return(6);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge)) return(90);

  if(Equal(data, FM_Damage))   return(30);
//  if(data == FM_Condition) return(!GetUpgrade(KRFL));

  return(Default(data));
}

public func BotData1(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$FragGrenade$");
  if(Equal(data, FM_Icon))     return(CGRN);
  if(Equal(data, FM_AmmoID))   return(GRAM);
  if(Equal(data, FM_AmmoLoad)) return(8);
  if(Equal(data, FM_AmmoUsage))return(2);
  if(Equal(data, FM_Reload))   return(170);
  if(Equal(data, FM_Recharge)) return(90);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage))   return(14);

  return(Default(data));
}

public func BotData2(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(DMG_Explosion);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$Napalm$");
  if(Equal(data, FM_Icon))     return(NGRN);
  if(Equal(data, FM_AmmoID))   return(GSAM);
  if(Equal(data, FM_AmmoLoad)) return(60);
  if(Equal(data, FM_AmmoUsage))return(20);
  if(Equal(data, FM_Reload))   return(180);
  if(Equal(data, FM_Recharge)) return(60);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KRFL));

  if(Equal(data, FM_Damage))   return(30);

  return(Default(data));
}

public func BotData3(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(DMG_Fire);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$Slime$");
  if(Equal(data, FM_Icon))     return(SGRN);
  if(Equal(data, FM_AmmoID))   return(GRAM);
  if(Equal(data, FM_AmmoLoad)) return(2);

  if(Equal(data, FM_Reload))   return(180);
  if(Equal(data, FM_Recharge)) return(90);

  if(Equal(data, FM_Damage))   return(8);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KSLM));
  return(Default(data));
}

public func BotData4(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(DMG_Bio);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_3);

  return(Default(data));
}

public func FMData5(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$EMP$");
  if(Equal(data, FM_Icon))     return(EGRN);
  if(Equal(data, FM_AmmoID))   return(ENAM);
  if(Equal(data, FM_AmmoLoad)) return(30);
  if(Equal(data, FM_AmmoUsage))return(30);
  if(Equal(data, FM_Reload))   return(60);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))   return(400);

  return(Default(data));
}

public func BotData5(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(nil);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);
  if(Equal(data, BOT_EMP))      return(true);

  return(Default(data));
}

public func FMData6(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))     return("$Laser$");
  if(Equal(data, FM_Icon))     return(LGRN);
  if(Equal(data, FM_AmmoID))   return(ENAM);
  if(Equal(data, FM_AmmoLoad)) return(45);
  if(Equal(data, FM_AmmoUsage))return(15);
  if(Equal(data, FM_Reload))   return(180);
  if(Equal(data, FM_Recharge)) return(100);

  if(Equal(data, FM_Condition)) return(GetUpgrade(KLAS));

  if(Equal(data, FM_Damage))   return(50);

  return(Default(data));
}

public func BotData6(int data)
{
  data ??= 0;
  if(Equal(data, BOT_Range))    return(300);
  if(Equal(data, BOT_DmgType))  return(DMG_Energy);
  if(Equal(data, BOT_Ballistic))  return(true);
  if(Equal(data, BOT_Power))    return(BOT_Power_2);

  return(Default(data));
}

public func Fire1() { LaunchGrenade(GREN, 90,(Contained()->~AimAngle( 5) ?? 0)+(RandomX( -5, 5) ?? 0),1); }
public func Fire2() { LaunchGrenade(CGRN, 70,(Contained()->~AimAngle(10) ?? 0)+(RandomX(-10,15) ?? 0),2); }
public func Fire3() { LaunchGrenade(NGRN, 80,(Contained()->~AimAngle( 5) ?? 0)+(RandomX( -5, 5) ?? 0),3); }
public func Fire4() { LaunchGrenade(SGRN,100,(Contained()->~AimAngle( 5) ?? 0)+(RandomX( -5, 5) ?? 0),4); }
public func Fire5() { LaunchGrenade(EGRN, 80,(Contained()->~AimAngle( 5) ?? 0)+(RandomX( -5, 5) ?? 0),5); }
public func Fire6() { LaunchGrenade(LGRN, 80,(Contained()->~AimAngle( 5) ?? 0)+(RandomX( -5, 5) ?? 0),6); }

// verschießt eine Granate
// idg:   ID der Granate
// speed: Geschwindigkeit
// angle: Winkel
public func LaunchGrenade(id idg, int speed, int angle, int mode) {
  speed ??= 0;
  angle ??= 0;
  mode ??= 0;
  var user = Contained();
  var dir = (GetDir(user) ?? 0)*2-1;

  // Adjust angle
  // special on jetpack...
  if(!SEqual(user->GetAction(), "JetpackFlight"))
	angle = BoundBy((angle ?? 0)+(GetYDir(user) ?? 0)*(dir ?? 0),-360,360);
  // calculate speed
  var xdir = Sin(angle,speed);
  var ydir = -(Cos(angle,speed) ?? 0);

  var x,y;
  user->WeaponEnd(x,y);

  // create and launch
  var grenade=CreateObject(idg, (x ?? 0)+(xdir ?? 0)/10, (y ?? 0)+(ydir ?? 0)/10, GetController(user));
  grenade->Launch((xdir ?? 0)+(GetXDir(user) ?? 0)/2, (ydir ?? 0)+(GetYDir(user) ?? 0)/2, GetFMData(FM_Damage, mode));
  grenade->~ChangeExplodeDelay(explodedelay);

  // Sicht verfolgen wenn kein automatischer Schuss
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), grenade);

  // effect
  Sound("GrenadeFire");

  CreateParticle("Thrust",x,y,GetXDir(user),GetYDir(user),80,RGBa(255,200,200,nil),nil,nil);

  for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
    CreateParticle("Smoke2",(x ?? 0)+(RandomX(-5,+5) ?? 0),(y ?? 0)+(RandomX(-5,+5) ?? 0),
                   (GetXDir(user) ?? 0)+(RandomX(nil,(xdir ?? 0)/4) ?? 0),(GetYDir(user) ?? 0)+(RandomX(nil,(ydir ?? 0)/4) ?? 0),
                   RandomX(80,140),RGBa(200,200,200,nil),nil,nil);
  }
}

/* Upgrade */

public func IsUpgradeable(id uid) {
  if(GetUpgrade(uid))
    return(nil);
  
  if(Equal(uid, KSLM)) return("$KSLMUpgradeDesc$");
  if(Equal(uid, KLAS)) return("$KLASUpgradeDesc$");
  if(Equal(uid, KRFL)) return("$KRFLUpgradeDesc$");
}

public func OnUpgrade(id uid) {
    SetUpgradeGraphic();
  var own = GetOwner(Contained());
  if(Equal(uid, KSLM)) {
    // Feuermodus wechseln
    SetFireMode(4);
    if(Contained()) HelpMessage(own,"$KSLMUpgraded$",Contained());
    return(true);
  }
  if(Equal(uid, KLAS)) {
    // Feuermodus wechseln
    SetFireMode(6);
    if(Contained()) HelpMessage(own,"$KLASUpgraded$",Contained());
    return(true);
  }
  if(Equal(uid, KRFL)) {
    // Feuermodus wechseln
    SetFireMode(2);
    if(Contained()) HelpMessage(own,"$KRFLUpgraded$",Contained());
    return(true);
  }
}

public func OnDowngrade(id uid) {
    SetUpgradeGraphic();
  if(Equal(uid, KLAS)) {
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, KSLM)) {
    SetFireMode(1);
    return(true);
  }
  if(Equal(uid, KRFL)) {
    SetFireMode(1);
    return(true);
  }
}


// Soundeffekte

public func OnReload()
{
  Sound("GrenadeLoad");
}
