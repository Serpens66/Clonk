/*-- Flak --*/

#strict 3
#include WEPN

local Crosshair;
local Rifle; // aus welchem Rohr wir gerade ballern

func Initialize()
{
  SetAction("Aim");
  SetPhase((GetActMapVal("Length",GetAction()) ?? 0)/3); // Nach oben Schaun
  
  // Schadenseffekt
  AddDamageEffect(this);
  
  return(_inherited());
}


/** Anfasszeugs **/

public func Grabbed(object byObj, bool grabbed)
{
	// Abfassen! :(
	grabbed ??= false;
	if(!grabbed)
	{
		EndAim();
		return;
	}
	
	if(GetUser())
	{
		return(AddCommand(byObj,"Ungrab"));
	}
	else
	{
		SetUser(byObj);
		SetOwner(GetController(byObj));
	}
	
	InitAim();
}

public func GrabLost()
{
	Grabbed(nil,true);
}


/* Zielzeug */

private func InitAim()
{
	if(Crosshair)
		RemoveObject(Crosshair);
	
	Crosshair = CreateObject(HCRH); // Owner wird in Init gesetzt
	Crosshair->Init(this());
	Crosshair->SetAngle(PtoR());
} 

private func EndAim()
{
	PauseReload();
	AimCancel(GetUser());
	SetUser();
	if(Crosshair)
		RemoveObject(Crosshair);
}

public func IsAiming() { return(true); }

private func StartAngle() { return(90); }

// Winkel -> Phase
private func RtoP(int angle)
{
	angle ??= 0;
	var phases = GetActMapVal("Length",GetAction());

	// -180 bis +180
	angle = Normalize(angle,-180);
	// Maximale Drehung nach unten
	angle = BoundBy(angle,-(StartAngle() ?? 0),+(StartAngle() ?? 0));
	// 0 bis 2*StartAngle()
	S42MigrationAssign(angle, "+=", (StartAngle() ?? 0));
	
	// umrechnen von 0..2*StartAngle() nach 0..phases-1
	var p = (angle ?? 0)*((phases ?? 0)-1)/2/(StartAngle() ?? 0);
	
	return(p);
}

// Phase -> Winkel
private func PtoR()
{
	var phases = GetActMapVal("Length",GetAction());
	
	var a = (GetPhase() ?? 0)*2*(StartAngle() ?? 0)/((phases ?? 0)-1);
	
	S42MigrationAssign(a, "-=", (StartAngle() ?? 0));
	
	return(a);
}

/** Zielen **/

public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj)
{
	iTx ??= 0;
	iTy ??= 0;
	iData ??= 0;
	if(!!SEqual(strCommand, "MoveTo")) {
		var angle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);
		// max angle
		angle = BoundBy(angle,-(StartAngle() ?? 0),+(StartAngle() ?? 0));
		
		Crosshair->SetAngle(angle);
		SetPhase(RtoP(angle));
		
		// doof: In controlCommand erfahren wir nicht wer das verursacht hat... aber
		var user = GetCursor(GetController());
		ControlThrow(user);
		return(1);
	}
}

public func ControlLeft()
{
    [$TxtLeft$|Image=TAN3:1]
	AimUp(GetUser(), 1, "ControlConf");
	return(true);
}

public func ControlRight()
{
    [$TxtRight$|Image=TAN3:2]
	AimDown(GetUser(), 1, "ControlConf");
	return(true);
}  

public func ControlThrow()
{
    [$TxtFire$|Image=TAN3:0]
	_inherited(...);
}

public func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
	comdir ??= 0;
	dig ??= false;
	throw ??= false;
	if(Equal(comdir, COMD_Left))
		AimUpdate(GetUser(), COMD_Up, 1, "ControlConf");
	else if(Equal(comdir, COMD_Right))
		AimUpdate(GetUser(), COMD_Down, 1, "ControlConf");
	else
		AimUpdate(GetUser(), nil, 1, "ControlConf");
	
	inherited(clonk, comdir, dig, throw);
	
	return(true);
}

public func ControlConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  conf ??= 0;
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming((conf ?? 0) * (AIM_Step ?? 0) / 5);
  else
    DoAiming((conf ?? 0) * (AIM_Step ?? 0));
}

public func DoAiming(int change)
{
	change ??= 0;
	var angle = (Crosshair->GetAngle() ?? 0) + (change ?? 0);

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = (angle ?? 0)-(angle ?? 0)%(AIM_Step ?? 0);

  // Winkel wird zu groß?
  if((Abs(angle) ?? 0) > 90)
    return;

  Crosshair->SetAngle(angle);

	//UpdatePhase();
	SetPhase(RtoP(angle));
}


/** Waffenzeugs **/

public func FMData1(int data)
{
	data ??= 0;
	if(Equal(data, FM_Name))			return("$Auto$");
	if(Equal(data, FM_AmmoID))		return(STAM);

	if(Equal(data, FM_AmmoLoad))		return(36);
	if(Equal(data, FM_AmmoUsage))	return(3);

	if(Equal(data, FM_Reload))		return(80);
	if(Equal(data, FM_Recharge))		return(12);

	if(Equal(data, FM_Damage))		return(20);
	if(Equal(data, FM_Auto))			return(true);

	return(Default(data));
}

public func Fire1()
{
	var angle = Crosshair->GetAngle();
	var x,y;
	var ammo;
	
	CalculateRifle(x,y);
	
	ammo = CreateObject(SHTF, x, y, GetController(GetUser()));
	ammo->Sound("FlakCannon");
	ammo->Launch(Sin(angle,160), -(Cos(angle,160) ?? 0), GetFMData(FM_Damage,1));
	
	angle = Normalize(angle,-180);
	var bangle = (angle ?? 0)-90;
	if((angle ?? 0)<0) bangle = (angle ?? 0)+90;
	var str = Abs((angle ?? 0)/3);
	BulletCasing((x ?? 0)/4,(y ?? 0)/4, Sin(bangle,str), -(Cos(bangle,str) ?? 0), 8);
	
	MuzzleFlash(100,this(),x,y,angle);
}


public func FMData2(int data)
{
        data ??= 0;
        if(Equal(data, FM_Name))                     return("$Raketen$");
        if(Equal(data, FM_AmmoID))           return(MIAM);

        if(Equal(data, FM_AmmoLoad))         return(4);
//        if(data == FM_AmmoRate)         return(1);
        if(Equal(data, FM_AmmoUsage))        return(4);

        if(Equal(data, FM_Reload))           return(300);
        if(Equal(data, FM_Recharge))         return(nil);

        if(Equal(data, FM_Damage))           return(100);
        if(Equal(data, FM_Auto))                     return(nil);

        return(Default(data));
}

public func Fire2()
{
        Sound("ParticleShot");
        ScheduleCall(nil,"FireLaser",6,4, 100); // vorletzter Parameter: anzahl der Schüsse, Letzer Parameter: schaden der schüsse
}

public func FireLaser(int iDmg) {
  iDmg ??= 0;
  var user = GetUser();
  if(!user) return;
  var x,y;
  CalculateRifle(x,y);
  
  var angle = Crosshair->GetAngle();
  var rocket = CreateObject(MISS,x,y,GetController(user));
  rocket->Launch(angle, iDmg);

  // Effekte
  MuzzleFlash(100,this(),x,y,angle);

  // Sound
  Sound("Rocket");
}

public func LaserStrike(object pTarget) {       // Laser trifft
  DoDmg(GetFMData(FM_Damage, 1), DMG_Energy, pTarget);
  
  // geht durch...
  return nil;
}



/*
public func FMData3(int data)
{
	if(data == FM_Name)			return("$Single$");
	if(data == FM_AmmoID)		return(STAM);

	if(data == FM_AmmoLoad)		return(25);
	if(data == FM_AmmoRate)		return(1);

	if(data == FM_Reload)		return(80);
	if(data == FM_Recharge)		return(30);

	if(data == FM_Damage)		return(20);
	// if(data == FM_Auto) return(true);

	return(Default(data));
}

public func Fire3()
{
	var angle = Crosshair->GetAngle();
	var x,y;
	var ammo;
	
	for(var i; i < 4; i++)
	{
		CalculateRifle(x,y);
		ammo = CreateObject(SHTF, x, y, GetController(GetUser()));
		ammo->Launch(Sin(angle,100), -Cos(angle,100), 20);
		MuzzleFlash(100,this(),x,y,angle);
	}
	
	//DrawParticleLine("PSpark", x,y+6, x*2, y*2+6, 5, 25, RGB(255), RGB(0,0,255)); 
}
*/

private func CenterFix() {
	var p = GetPhase();
	if((p ?? 0) > 17)
		p = 35-(p ?? 0);
	
	// weirder Korrekturfaktor damit wir immer ~den Mittelpunkt haben
	var correction;
	correction = (100*(p ?? 0))/17;
	return(correction);
}

private func CalculateRifle(&oX, &oY)
{
	var angle = (Crosshair->GetAngle() ?? 0) + (GetR() ?? 0);

	// Korrektur für Mitte (speziell für diese Grafik)
	var correction = CenterFix();
	
	var x = +(Sin(angle, 40-((correction ?? 0)/5)) ?? 0);
	var y = -(Cos(angle, 20+(correction ?? 0)/20) ?? 0);
	
	// circle x,y
	// min->max breite: 0->6
	// min->max höhe: 3->5
	var cx, cy;
	cx = (correction ?? 0)/16;
	cy = 5 - (correction ?? 0)/50;
	
	S42MigrationAssign(x, "+=", (Sin(30+90*(Rifle ?? 0), cx) ?? 0));
	S42MigrationAssign(y, "+=", (Cos(60+90*(Rifle ?? 0), cy) ?? 0));
	
	if(S42MigrationStep(Rifle, 1) > 3)
		Rifle = nil;
	
	oX = x;
	oY = (y ?? 0)+4;
}

public func NoWeaponChoice() { return(1); }

// Schaden

public func MaxDamage() { return (350); }

public func OnDmg(int iDmg, int iType) {

	iDmg ??= 0;
	iType ??= 0;
	if(Equal(iType, DMG_Bio))		return(100);
	if(Equal(iType, DMG_Melee))		return(80);
	if(Equal(iType, DMG_Fire))		return(80);
	if(Equal(iType, DMG_Explosion))	return(nil);

	return(30);
}

public func Damage() {
	if((GetDamage() ?? 0) < (MaxDamage() ?? 0)) return;

	// boom
	Explode(30);
}

public func IsBulletTarget() { return (true);}
public func IsBulletExtraTarget() { return (true);}
public func IsRepa() { return (true);}