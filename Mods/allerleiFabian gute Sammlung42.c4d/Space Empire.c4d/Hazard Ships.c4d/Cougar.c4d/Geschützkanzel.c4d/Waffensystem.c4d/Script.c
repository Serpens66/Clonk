/*-- Maschinenkanone --*/

#strict 3

#include WEPN
public func IsWeaponSystem()  { return(true); }

// Anzeige in der Hand
public func HandSize() { return(1000);  }
public func HandX()    { return(5500); }
public func HandY()    { return(-1000); }
public func BarrelXOffset(){return(-10000); }
public func BarrelYOffset(){return(-10000); }

// Daten für ersten Feuermodus
public func FMData1(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("Gatling");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(100);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(10);

  return(Default(data));
}

public func FMData2(int data)
{
  data ??= 0;
  if(Equal(data, FM_Name))       return("nur Gebäude, Gatling");
  if(Equal(data, FM_AmmoID))     return(STAM);
  if(Equal(data, FM_AmmoLoad))   return(100);

  if(Equal(data, FM_Reload))     return(160);
  if(Equal(data, FM_Recharge))   return(3);

  if(Equal(data, FM_AmmoUsage))  return(1);
  if(Equal(data, FM_AmmoRate))   return(2);
  if(Equal(data, FM_Auto))       return(true);

  if(Equal(data, FM_Damage))     return(2);

  return(Default(data));
}

// Jäger da?
public func Predator()
{
  local jaeger;
  if(jaeger=FindObject(CGG_, -20,-20,40,40, nil,nil,nil,nil)) return(1);
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
  x=Cos( (GetR(jaeger) ?? 0) + 270 ,1);
  y=Sin( (GetR(jaeger) ?? 0) + 270 ,1);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,300,600,5,400,GetFMData(FM_Damage, 1));

  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
}

public func Fire2()		// ungenaues aber heftiges Minigunfeuer
{ 
  if(!Predator()) return(nil);
  var user = Contained();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (GetR(jaeger) ?? 0)+(RandomX(-2,+2) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Cos( (GetR(jaeger) ?? 0) + 270 ,1);
  y=Sin( (GetR(jaeger) ?? 0) + 270 ,1);
  var ammo = CreateObject(SHTG,x,y,GetController(user));
  ammo->Launch(angle,300,600,5,400,GetFMData(FM_Damage, 2));

  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
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