/*-- Flak --*/

#strict
#include WEPN

local Crosshair;
local Rifle; // aus welchem Rohr wir gerade ballern

func Initialize()
{
  SetAction("Aim");
  SetPhase(GetActMapVal("Length",GetAction())/3); // Nach oben Schaun
  
  // Schadenseffekt
  AddDamageEffect(this);
  
  return(_inherited());
}


/** Anfasszeugs **/

public func Grabbed(object byObj, bool grabbed)
{
	// Abfassen! :(
	if(!grabbed)
	{
		EndAim();
		return();
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
	Grabbed(0,true);
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
	var phases = GetActMapVal("Length",GetAction());

	// -180 bis +180
	angle = Normalize(angle,-180);
	// Maximale Drehung nach unten
	angle = BoundBy(angle,-StartAngle(),+StartAngle());
	// 0 bis 2*StartAngle()
	angle += StartAngle();
	
	// umrechnen von 0..2*StartAngle() nach 0..phases-1
	var p = angle*(phases-1)/2/StartAngle();
	
	return(p);
}

// Phase -> Winkel
private func PtoR()
{
	var phases = GetActMapVal("Length",GetAction());
	
	var a = GetPhase()*2*StartAngle()/(phases-1);
	
	a -= StartAngle();
	
	return(a);
}

/** Zielen **/

public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj)
{
	if(strCommand eq "MoveTo") {
		var angle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);
		// max angle
		angle = BoundBy(angle,-StartAngle(),+StartAngle());
		
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
	if(comdir == COMD_Left)
		AimUpdate(GetUser(), COMD_Up, 1, "ControlConf");
	else if(comdir == COMD_Right)
		AimUpdate(GetUser(), COMD_Down, 1, "ControlConf");
	else
		AimUpdate(GetUser(), 0, 1, "ControlConf");
	
	inherited(clonk, comdir, dig, throw);
	
	return(true);
}

public func ControlConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming(conf * AIM_Step / 5);
  else
    DoAiming(conf * AIM_Step);
}

public func DoAiming(int change)
{
	var angle = Crosshair->GetAngle() + change;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;

  // Winkel wird zu groß?
  if(Abs(angle) > 90)
    return();

  Crosshair->SetAngle(angle);

	//UpdatePhase();
	SetPhase(RtoP(angle));
}


/** Waffenzeugs **/

public func FMData1(int data)
{
	if(data == FM_Name)			return("$Auto$");
	if(data == FM_AmmoID)		return(STAM);

	if(data == FM_AmmoLoad)		return(36);
	if(data == FM_AmmoUsage)	return(3);

	if(data == FM_Reload)		return(80);
	if(data == FM_Recharge)		return(12);

	if(data == FM_Damage)		return(20);
	if(data == FM_Auto)			return(true);

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
	ammo->Launch(Sin(angle,160), -Cos(angle,160), GetFMData(FM_Damage,1));
	
	angle = Normalize(angle,-180);
	var bangle = angle-90;
	if(angle<0) bangle = angle+90;
	var str = Abs(angle/3);
	BulletCasing(x/4,y/4, Sin(bangle,str), -Cos(bangle,str), 8);
	
	MuzzleFlash(100,this(),x,y,angle);
}


public func FMData2(int data)
{
        if(data == FM_Name)                     return("$Raketen$");
        if(data == FM_AmmoID)           return(MIAM);

        if(data == FM_AmmoLoad)         return(4);
//        if(data == FM_AmmoRate)         return(1);
        if(data == FM_AmmoUsage)        return(4);

        if(data == FM_Reload)           return(300);
        if(data == FM_Recharge)         return(0);

        if(data == FM_Damage)           return(100);
        if(data == FM_Auto)                     return(false);

        return(Default(data));
}

public func Fire2()
{
        Sound("ParticleShot");
        ScheduleCall(0,"FireLaser",6,4, 100); // vorletzter Parameter: anzahl der Schüsse, Letzer Parameter: schaden der schüsse
}

public func FireLaser(int iDmg) {
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
  return 0;
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
	if(p > 17)
		p = 35-p;
	
	// weirder Korrekturfaktor damit wir immer ~den Mittelpunkt haben
	var correction;
	correction = (100*p)/17;
	return(correction);
}

private func CalculateRifle(&oX, &oY)
{
	var angle = Crosshair->GetAngle() + GetR();

	// Korrektur für Mitte (speziell für diese Grafik)
	var correction = CenterFix();
	
	var x = +Sin(angle, 40-(correction/5));
	var y = -Cos(angle, 20+correction/20);
	
	// circle x,y
	// min->max breite: 0->6
	// min->max höhe: 3->5
	var cx, cy;
	cx = correction/16;
	cy = 5 - correction/50;
	
	x += Sin(30+90*Rifle, cx);
	y += Cos(60+90*Rifle, cy);
	
	if(++Rifle > 3)
		Rifle = 0;
	
	oX = x;
	oY = y+4;
}

public func NoWeaponChoice() { return(1); }

// Schaden

public func MaxDamage() { return (350); }

public func OnDmg(int iDmg, int iType) {

	if(iType == DMG_Bio)		return(100);
	if(iType == DMG_Melee)		return(80);
	if(iType == DMG_Fire)		return(80);
	if(iType == DMG_Explosion)	return(0);

	return(30);
}

public func Damage() {
	if(GetDamage() < MaxDamage()) return;

	// boom
	Explode(30);
}

public func IsBulletTarget() { return (true);}
public func IsBulletExtraTarget() { return (true);}
public func IsRepa() { return (true);}