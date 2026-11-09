/*-- Bordkanone --*/

#strict 3

#include WEPN
public func IsWeaponSystem()  { return(true); }

// Anzeige in der Hand
public func HandSize() { return(900);  }
public func HandX()    { return(11000); }
public func HandY()    { return(-3000); }
public func BarrelYOffset(){return(-1000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("Gatling");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(50);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(5);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("LangstreckenBallisticRakete");
  if(Equal(data, FM_Icon))     return(HNIS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(3);

  if(Equal(data, FM_Reload))   return(120);
  if(Equal(data, FM_Recharge))   return(1);


  if(Equal(data, FM_Damage)) return(35);

  return(Default(data));
}


public func FMData3(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("nur Gebäude, Gatling");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(50);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(1);

  return(Default(data));
}

public func FMData4(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))   return("WärmesuchRakete");
  if(Equal(data, FM_Icon))     return(HMIS);
  if(Equal(data, FM_AmmoID))   return(MIAM);
  if(Equal(data, FM_AmmoLoad)) return(3);

  if(Equal(data, FM_Reload))   return(160);
  if(Equal(data, FM_Recharge))   return(1);


  if(Equal(data, FM_Damage)) return(35);

  return(Default(data));
}



// Jäger da?
public func Predator()
{
  local jaeger;
  if(jaeger=FindObject(CCF_, -20,-20,40,40, nil,nil,nil,nil)) return(1);
  return(jaeger);
}


// Feuern im ersten Feuermodus
public func Fire1()		// ungenaues aber heftiges Minigunfeuer
{ 
  if(!Predator()) return(nil);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (GetR(jaeger) ?? 0)+(RandomX(-2,+2) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Cos( (GetR(jaeger) ?? 0) + 275 ,nil);
  y=Sin( (GetR(jaeger) ?? 0) + 275 ,nil);
  var ammo = CreateObject(SHT_,(x ?? 0)*5,(y ?? 0)*5,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
}

public func Fire2()
{   
Predator();
LaunchRocket(HNIS,GetR(jaeger), GetFMData(FM_Damage, 2));
}


public func Fire3()		// ungenaues aber heftiges Minigunfeuer
{ 
  if(!Predator()) return(nil);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (GetR(jaeger) ?? 0)+(RandomX(-2,+2) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Cos( (GetR(jaeger) ?? 0) + 275 ,nil);
  y=Sin( (GetR(jaeger) ?? 0) + 275 ,nil);
  var ammo = CreateObject(SHTG,(x ?? 0)*5,(y ?? 0)*5,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 3));

  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->WeaponBegin(x,y);
  var dir = (GetDir(user) ?? 0)*2-1;
}

public func Fire4()
{   
Predator();
LaunchRocket(HMIS,GetR(jaeger), GetFMData(FM_Damage, 4));
}


public func LaunchRocket(rid, angle, dmg) {

  var user = Contained();
  var x,y;


  x=Sin( GetR(jaeger) ,9);
  y=Cos( GetR(jaeger) ,9);
  
  // not too near to clonk
  x = (x ?? 0)*3/2;
  y = (y ?? 0)*3/2+15;

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, dmg);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(Equal(user, GetCursor(GetOwner())))
    SetPlrView(GetOwner(), rocket);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = (ax ?? 0)-(x ?? 0);
  ydir = (ay ?? 0)-(y ?? 0);

  CreateParticle("Thrust",ax,ay,(xdir ?? 0)/2,(ydir ?? 0)/2,80,RGBa(255,200,200,nil),nil,nil);

  for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",(ax ?? 0)+(Sin(angle,rand) ?? 0),(ay ?? 0)-(Cos(angle,rand) ?? 0),
                   RandomX(nil,2*(xdir ?? 0)),RandomX(nil,2*(ydir ?? 0)),
                   RandomX(80,140),RGBa(220,200,180,nil),nil,nil);
  }

  // Sound
  Sound("Rocket");

}


// Soundeffekte

public func OnReload()
{
  Sound("RocketLoad");
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


/* Waffenfix für neue Hazard-Version */
private func Shoot(object caller)
{
  //User da? :S
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(And(And(!(GetUser()->~IsAiming()), GetFMData(FM_Aim)), !(GetUser()->~AimOverride())))
    return(stopauto=true); //abbrechen
    
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto))
    shooting = true;

  // Munition abziehen
  if(Equal(ratecount, 1)) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-(muni ?? 0));
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { S42MigrationStep(ratecount, -1); }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = nil;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
      ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}