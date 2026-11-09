/*-- Bordgeschütz --*/

#strict

#include WEPN
public func IsWeaponSystem()  { return(true); }

// Anzeige in der Hand
public func HandSize() { return(900);  }
public func HandX()    { return(22000); }
public func HandY()    { return(-6000); }
public func BarrelYOffset(){return(-1000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  if(data == FM_Name)       return("Gatling");
  if(data == FM_AmmoID)     return(STAM);
  if(data == FM_AmmoLoad)   return(50);

  if(data == FM_Reload)     return(160);
  if(data == FM_Recharge)   return(3);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(2);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(5);

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)   return("LangstreckenBallisticRaketen");
  if(data == FM_Icon)     return(HNIS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(6);

  if(data == FM_Reload)   return(120);
  if(data == FM_Recharge)   return(1);


  if(data == FM_Damage) return(35);

  return(Default(data));
}

public func FMData3(int data)
{
  if(data == FM_Name)       return("nur Gebäude, Gatling");
  if(data == FM_AmmoID)     return(STAM);
  if(data == FM_AmmoLoad)   return(50);

  if(data == FM_Reload)     return(160);
  if(data == FM_Recharge)   return(3);

  if(data == FM_AmmoUsage)  return(1);
  if(data == FM_AmmoRate)   return(2);
  if(data == FM_Auto)       return(true);

  if(data == FM_Damage)     return(1);

  return(Default(data));
}

public func FMData4(int data)
{
  if(data == FM_Name)   return("WärmesuchRakete");
  if(data == FM_Icon)     return(HMIS);	
  if(data == FM_AmmoID)   return(MIAM);
  if(data == FM_AmmoLoad) return(3);

  if(data == FM_Reload)   return(160);
  if(data == FM_Recharge)   return(1);


  if(data == FM_Damage) return(35);

  return(Default(data));
}



// Jäger da?
public func Predator()
{
  local jaeger;
  if(jaeger=FindObject(CCG_, -20,-20,40,40, 0,0,0,0)) return(1);
  return(jaeger);
}

public func Bezug(_rocky)
{
  local rocky;
  rocky=_rocky;
  return(rocky);
}

// Feuern im ersten Feuermodus
public func Fire1()		// ungenaues aber heftiges Minigunfeuer
{ 
  if(!Predator()) return(0);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = GetR(jaeger)+RandomX(-2,+2);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Cos( GetR(jaeger) + 270 ,0);
  y=Sin( GetR(jaeger) + 270 ,0);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->WeaponBegin(x,y);
  var dir = GetDir(user)*2-1;
}

public func Fire2()
{   
LaunchRocket(HNIS,GetR(rocky)+RandomX(-5,+5), GetFMData(FM_Damage, 2));
}


public func Fire3()		// ungenaues aber heftiges Minigunfeuer
{ 
  if(!Predator()) return(0);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = GetR(jaeger)+RandomX(-2,+2);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Cos( GetR(jaeger) + 275 ,0);
  y=Sin( GetR(jaeger) + 275 ,0);
  var ammo = CreateObject(SHTG,x*5,y*5,GetController(user));
  ammo->Launch(angle,300,600,4,600,GetFMData(FM_Damage, 3));

  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  
  user->WeaponBegin(x,y);
  var dir = GetDir(user)*2-1;
}

public func Fire4()
{   
Predator();
LaunchRocket(HMIS,GetR(jaeger), GetFMData(FM_Damage, 4));
}


public func LaunchRocket(rid, angle, dmg) {

  var user = Contained();
  var x,y;


  x=Cos( GetR(rocky) +270,9)+GetX(rocky)-GetX();
  y=Sin( GetR(rocky) +270,9)+25;
  
  // not too near to clonk

  var rocket = CreateObject(rid,x,y,GetController(user));
  rocket->Launch(angle, dmg);
  // Sicht verfolgen wenn kein automatischer Schuss
    if(user==GetCursor(GetOwner()))
    SetPlrView(GetOwner(), rocket);

  // Effekte
  var ax, ay, xdir, ydir;
  user->WeaponBegin(ax,ay);
  xdir = ax-x;
  ydir = ay-y;

  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
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
  Sound("MiniGun",0,0,0,0,1);
  Sound("MiniTurn",0,0,0,0,1);
}

public func OnAutoStop()
{
  Sound("MiniGun",0,0,0,0,-1);
  Sound("MiniTurn",0,0,0,0,-1);
}


/* Waffenfix für neue Hazard-Version */
private func Shoot(object caller)
{
  //User da? :S
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim) && !(GetUser()->~AimOverride()))
    return(stopauto=true); //abbrechen
    
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto))
    shooting = true;

  // Munition abziehen
  if(ratecount == 1) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-muni);
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { --ratecount; }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = false;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
      ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}