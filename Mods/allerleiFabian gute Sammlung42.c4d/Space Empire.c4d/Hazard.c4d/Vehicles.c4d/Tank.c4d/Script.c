/*-- Tank --*/

#strict 3

public func IsHazardShip()  { return(true); }
#include L_TK	// tank physics


/* Initialization */
/* ------------------------------------------------------------------------- */

public func Initialize() {			// Initialization

	// Action
	SetAction("Drive");
	SetDir(DIR_Left);

	// Create turret
	NewTurret(TurretID());

	// Create chassis
	NewChassis(ChassisID());
	
	shieldupgrade = nil;
	aiming = nil;
	
	// Damage effect
	AddDamageEffect(this);
	
	return(_inherited());
}

/* Turret and Chassis */
/* ------------------------------------------------------------------------- */

local turret, chassis;				// turret, chassis

public func TurretID() { return(TRRT); }
public func ChassisID() { return(CHSS); }

public func NewTurret(id idTurret) {		// Set new turret
	// delete old turret if there were one
	if(turret) RemoveObject(turret);

	// create the new one
	turret = CreateObject(idTurret,nil,5,GetOwner());
	turret -> AttachTo(this());
	SetObjectOrder(this(), turret);
}

public func NewChassis(id idChassis) {		// set new chassis
	// delete old chassis if there were one
	if(chassis) RemoveObject(chassis);

	// create the new one
	chassis = CreateObject(idChassis,nil,18,GetOwner());
	chassis -> AttachTo(this());
	SetObjectOrder(this(), chassis);
}

// the turret has the lights

public func ContextLight(pCaller) {
  [$SwitchLight$|Image=FLSH]
  turret->~SwitchLights();
  return(1);
}

/* Aiming */
/* ------------------------------------------------------------------------- */

local aiming;
local crosshair;
local aimmode;

public func ReadyToAim() { return(!IsJumping()); }
public func AimOverride() { return(IsJumping()); }
public func IsAiming() { return(aiming); }

public func MaxAimUp() {
	return(turret->~MaxAimUp());
}

public func MaxAimDown() {
	return(Min(110,turret->~MaxAimDown()));
}

// almost copied from hazard...:

public func StartAiming() {     // Anfangen zu zielen
	Stop();
	
	crosshair = CreateObject(HCRH,nil,nil,-1);
	crosshair->Init(this());
	crosshair->SetAngle(80*((GetDir() ?? 0)*2-1));
	
	if(IsTurning()) crosshair->SetVisibility(VIS_None);
	//ScheduleCall(this(),"UpdateAiming",1);

	// Callback
	aiming = true;
	turret->~AimStart();
}

public func StopAiming() {      // Zielen beenden
  
	if(crosshair) RemoveObject(crosshair);
 
	// Callback
	aiming = nil;
	turret->~AimStop();
}

// copied from hazard

public func ContainedConf(int conf)
{
  // Feineres Zielen mit JnR-Steuerung möglich
  conf ??= 0;
  if(GetPlrCoreJumpAndRunControl(GetController()))
    DoAiming((conf ?? 0) * (AIM_Step ?? 0) / 5);
  else
    DoAiming((conf ?? 0) * (AIM_Step ?? 0));
}

protected func ContainedUpdate(object clonk, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  if(IsAiming())
  {
    AimUpdate(this(), comdir, 1, "ContainedConf");
    return(1);
	}
	if(IsTurning())
		if(!Equal(comdir, COMD_Stop))
		{
			Go();
			return(1);
		}
	if(IsDriving())
		if(Equal(comdir, COMD_Stop))
		{
			Stop();
			return(1);
		}
		
  return(_inherited(clonk, comdir, dig, throw));
}

public func DoAiming(int iChange) {
  //zielen wir überhaupt?
  iChange ??= 0;
  if(!IsAiming())
    return;

  var angle = (Abs(crosshair->GetAngle()) ?? 0) + (iChange ?? 0);

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = (angle ?? 0)-(angle ?? 0)%(AIM_Step ?? 0);

  // Winkel wird zu groß?
  if(Or((angle ?? 0) > (MaxAimDown() ?? 0), (angle ?? 0) < (MaxAimUp() ?? 0)))
    return;

  if(Equal(GetDir(), DIR_Left))
    angle = 360-(angle ?? 0);
  crosshair->SetAngle(angle);

  UpdateAiming();
}

public func UpdateAiming() {
  if(!IsAiming())
    return;

  var a = crosshair->GetAngle();

  turret->~SetAimAngle(a);
}

/* Callbacks */
/* ------------------------------------------------------------------------- */

public func Damage() {							// Damage
	if((GetDamage() ?? 0) < (MaxDamage() ?? 0)) return;

	Destroyed();
}

protected func Destruction() {					// Destruction
	if(turret) RemoveObject(turret);
	if(chassis) RemoveObject(chassis);
}

protected func ActivateEntrance(object by) {	// Entrance
	if (And(((by->GetOCF() ?? 0) & (OCF_CrewMember ?? 0)), !Contents())) {
		if(Equal(by, Contents())) { Exit(by); }
		else if(!ContentsCount()) {
			Enter(this, by);
		}
		FinishCommand(by, true);
	}
	else { FinishCommand(by, nil); }
}

protected func Ejection(object by) {
	if(!EMPShocked()) {
		Sound("tank_stop",nil,nil,SLvl(nil));
		Sound("tank",nil,nil,SLvl(nil),nil,-1);
		Sound("tank2",nil,nil,SLvl(1),nil,-1);
	}
	//DoScore(GetOwner(by),-GetValue());
	SetController(-1);
}

protected func Collection2(object by) {			// Collection
	// only one object, that is a clonk may be in the vehicle
	SetEntrance(nil);
	// change owner of the vehicle 
	DriverIsOwner(by);
	// resume turning if possible
	if(!EMPShocked()) {
		Sound("tank_start",nil,nil,SLvl(nil));
		Sound("tank",nil,nil,SLvl(nil),nil,1);
		ResumeTurn();
	}
}

// nur CrewMember dürfen hier rein
protected func RejectCollect(id idObj, object pObj) {
	if((GetOCF(pObj) ?? 0) & (OCF_CrewMember ?? 0)) return(nil);
	return(1);
} 

private func DriverIsOwner(object driver) {
	DoScore(GetOwner(),+(GetValue() ?? 0));
	SetOwner(GetOwner(driver));
	DoScore(GetOwner(driver),-(GetValue() ?? 0));
}

/* Sound  */
/* ------------------------------------------------------------------------- */

private func SLvl(bool drive) {
	drive ??= false;
	if(!drive) return(50);
	if(drive) return(100);
} 

public func OnGo() {
	if(!IsTurning()) EngineSoundStart();
}

public func OnStop() {
	if(!IsTurning()) EngineSoundStop();
}

public func OnJumpStart() {
	EngineSoundStop();
	Sound("Flamer",nil,nil,80);
}

public func OnJumpEnd() {
	if(!stop) EngineSoundStart();
}

public func OnTurnStart() {
	if(stop) EngineSoundStart();
}

public func OnTurnEnd() {
	if(stop) EngineSoundStop();
}

private func EngineSoundStart()	{ Sound("tank2",nil,nil,SLvl(1),nil,+1); }
private func EngineSoundStop()	{ Sound("tank2",nil,nil,SLvl(1),nil,-1); }

/* Status  */
/* ------------------------------------------------------------------------- */

public func IsBulletTarget() { return(1); }

public func MaxDamage()		{ return(600); }

public func IsThreat()		{ return(!!ContentsCount()); }

/* Control */
/* ------------------------------------------------------------------------- */

public func ContainedLeft(object driver) {	// Left
    [$TxtLeft$|Image=TAN1:3]
	if(EMPShocked()) return(true);
	if(!Equal(GetDir(), DIR_Left)) {
		if(!IsTurning()) {
			if(BottomContact())
				Turn();
		}
	}
	else {
		if(!IsDriving())
			if(!IsAiming())
				Go();
	}
	return(true);
}

public func ContainedRight(object driver) {	// Right
    [$TxtRight$|Image=TAN1:4]
	if(EMPShocked()) return(true);
	if(!Equal(GetDir(), DIR_Right)) {
		if(!IsTurning())
			if(BottomContact())
				Turn();
	}
	else {
		if(!IsDriving())
			if(!IsAiming())
				Go();
	}
	return(true);
}

public func ContainedDown(object driver) {	// Down
    [$TxtAimDown$|Image=TAN1:2]
	if(IsAiming()) {
		AimDown(this(), 1, "ContainedConf");
		return(1);
	}
	if(IsDriving()) {
		Stop();
		return(1);
	}
	else {
		if(GetPlrDownDouble(GetOwner(driver))) {
			StartAiming();
		}
	}
	//PauseTurn();
	return(1);
}

public func ContainedUpDouble(object driver) {	// UpDouble
    [$TxtOut$|Image=TAN1:6]
	if(IsAiming()) {
		AimUp(this(), 1, "ContainedConf");
		return(1);
	}
	if(!IsDriving()) {
		PauseTurn();
		Exit(driver,nil,20);
	}
}

public func ContainedLeftDouble(object driver) {	// LeftDouble
    [$TxtStopAim$|Image=TAN1:7]
	if(IsAiming()) {
		StopAiming();
		return(1);
	}
}

public func ContainedRightDouble(object driver) {	// RightDouble
    [$TxtStopAim$|Image=TAN1:7]
	if(IsAiming()) {
		StopAiming();
		return(1);
	}
}

public func ContainedUp(object driver) {	// Up
    [$TxtAimUp$|Image=TAN1:1]
	if(IsAiming()) {
		AimUp(this(), 1, "ContainedConf");
		return(1);
	}
	if(Jump()) {
		for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1))
			CreateDustParticle(RandomX(-10,10),RandomX(20,300));

		//var dir = GetDir()*2-1;
		//Schedule(Format("MuzzleFlash(20, Object(%d), %d, -2, %d, %d)",ObjectNumber(this()),-21*dir -1, -90*dir, RGB(255,150,0)),2,5,this());
	}
}

public func ContainedThrow(object driver) {	// Throw (Fire)
    [$TxtFire$|Image=TAN1:0]
	if(EMPShocked()) return(true);
	// forward to turret: select the fire mode
	turret->ControlThrow(driver);
	return(true);
}

public func ContainedDigDouble(object driver) {	// Dig Double (fire mode)
    [$TxtFireMode$|Image=TAN1:5]
	if(EMPShocked()) return(true);
	// forward to turret: select the fire mode
	turret->ControlDigDouble(driver);
	return(true);
}

public func ControlCommand(string strCommand, object pTarget, int iTx, int iTy, object pTarget2, int iData, object pCmdObj) {
	iTx ??= 0;
	iTy ??= 0;
	iData ??= 0;
	if(!!SEqual(strCommand, "MoveTo")) {
	
		if(IsAiming()) {
			//Winkel wird zu groß?
			var iAngle = Normalize((Angle(GetX(),GetY(),iTx,iTy) ?? 0)-(GetR() ?? 0),-180);
			
			// falsche Richtung? (nur drehen)
			if(Or(And((iAngle ?? 0) < 0, Equal(GetDir(), DIR_Right)),
			   And((iAngle ?? 0) > 0, Equal(GetDir(), DIR_Left)))) {
			   
			   Turn();
			   // danach!
			   turret->ControlThrow(pCmdObj);
			   return(1);
			}
			
			iAngle = Abs(iAngle);
			iAngle = BoundBy(iAngle,MaxAimUp(),MaxAimDown());

			if(Equal(GetDir(), DIR_Left))
				iAngle = 360-(iAngle ?? 0);
			
			crosshair->SetAngle(iAngle);
			UpdateAiming();
			
			ScheduleCall(turret,"ControlThrow",5,nil,pCmdObj);
		}
		else
		  turret->ControlThrow(pCmdObj);
		// do not exit
		return(1);
	}
}

/* Turning */
/* ------------------------------------------------------------------------- */

public func Turn() {
	SetAction("Turn");

	OnTurnStart();
	if(crosshair) crosshair->SetVisibility(VIS_None);
	
	chassis -> Turn();
	turret -> Turn();
}

protected func ChangeDir() {

	// turn dir 
	if(Equal(GetPhase(), 15)) {
		if(Equal(GetDir(), DIR_Left))
			SetDir(DIR_Right);
		else
			SetDir(DIR_Left);
			
		if(crosshair) crosshair->ChangeDir();
	}
	
	if(Equal(GetPhase(), 29)) {
	
		if(crosshair) {
			crosshair->SetVisibility(VIS_Owner);
		}
		
		OnTurnEnd();
	}
}

public func ResumeTurn() {
	// resume turning only if paused
	if(!SEqual(GetAction(), "TurnStalled")) return;
	var p = GetPhase();
	SetAction("Turn");
	SetPhase(p);
}

public func PauseTurn() {
	// pause turning only if turning
	if(!SEqual(GetAction(), "Turn")) return;
	var p = GetPhase();
	SetAction("TurnStalled");
	SetPhase(p);
}

/* HUD */
/* ------------------------------------------------------------------------- */

public func ReadyToFire() { return(!IsTurning()); }

public func UpdateCharge(object driver) {

	if(!turret) return;

	var hud = driver->GetHUD();
	if(hud) {
		hud->Update(turret, driver->AmmoStoring(),driver);
	}
	return(1);
}


/* Graphical effects */
/* ------------------------------------------------------------------------- */
// mid term: most sbould be general functions

protected func Timer() {
	
	DrivingAnimation();		// Animation beim Fahren
	DustEffect();			// Aufwirbeln von Staub
}

public func GetDmgEffectPos(&x, &y) {

	var turn = 100;
	if(!SEqual(GetAction(), "Drive"))
		turn = 100*(Abs(15 - (GetPhase() ?? 0)) ?? 0)/15;
	x = (Cos(GetR(),-((GetDir() ?? 0)*2-1)*13*(turn ?? 0)/100) ?? 0)+(Sin(GetR(),-4) ?? 0);
	y = (Sin(GetR(),-((GetDir() ?? 0)*2-1)*13*(turn ?? 0)/100) ?? 0)+(Cos(GetR(),-4) ?? 0);
}

private func DustEffect() {

	if(Or(IsDriving(), IsTurning())) {
		CreateDustParticle(((GetDir() ?? 0)*2-1)*10,RandomX(20,300));
	}
}

private func CreateDustParticle(int dir, int size) {
		// Dust effect
		dir ??= 0;
		size ??= 0;
		var groundx = Sin(GetR(),-(YOffsToWheels() ?? 0)-3);
		var groundy = Cos(GetR(),(+(YOffsToWheels() ?? 0) ?? 0)+3);
		var mat = GetMaterial(groundx, groundy);

		if(CheckDust(mat)) {
		
			var rand = Random(3);
			var r = GetMaterialColor(mat,rand,nil);
			var g = GetMaterialColor(mat,rand,1);
			var b = GetMaterialColor(mat,rand,2);
			var alpha = 160;
			var xdir = -(Max(26-(Abs((dir ?? 0)*2) ?? 0),nil) ?? 0)*(dir ?? 0)/(Abs(dir) ?? 0);
			var x = -(Sin(GetR(),YOffsToWheels()) ?? 0) + (Cos(GetR(),-(dir ?? 0)) ?? 0);
			var y = (Cos(GetR(),YOffsToWheels()) ?? 0) + (Sin(GetR(),-(dir ?? 0)) ?? 0);
			CreateParticle("Dust",x,y,Cos(GetR(),(xdir ?? 0)+(RandomX(-2,2) ?? 0)),RandomX(-5,5),size,RGBa(r,g,b,alpha));
		}
}

private func DrivingAnimation() {
	if(IsDriving()) {
	
		if(!IsTurning()) {
		
			// Fling :D
			var dir = (GetDir() ?? 0)*2-1;
			var r = GetR();
			var x = (Cos(r,(dir ?? 0)*20) ?? 0) -(Sin(r,8) ?? 0);
			var y = (Sin(r,(dir ?? 0)*20) ?? 0) +(Cos(r,8) ?? 0);

			for(var obj in FindObjects(Find_Category(C4D_Living),Find_AtPoint(x,y))) {
				if(GetAlive(obj))
					Fling(obj,-(dir ?? 0),-2);
			}
		
			var maxphase = GetActMapVal("Length",GetAction());
			if((maxphase ?? 0) > (GetPhase() ?? 0)+2)
				SetPhase((GetPhase() ?? 0)+2);
			else {
				if(Equal(maxphase, (GetPhase() ?? 0)+1)) SetPhase(1);
				else SetPhase(nil);
			}
		}
	}
}

public func ThrustEffect(int iEffectTime) {
	iEffectTime ??= 0;
	if(!BottomContact()) {
		if((iEffectTime ?? 0) < 10) {
			  var angle = GetR();
			  var xoff = -((GetDir() ?? 0)*2-1)*8;
			  var yoff = +(YOffsToWheels() ?? 0);
			  var x = -(Sin(angle,yoff) ?? 0)+(Cos(angle,xoff) ?? 0);
			  var y = (+(Cos(angle,yoff) ?? 0) ?? 0)+(Sin(angle,xoff) ?? 0);

			  CreateParticle("Thrust",x,y,-(GetXDir() ?? 0)/2,-(GetYDir() ?? 0)/2,RandomX(140,200),RGBa(255,200,200,80),nil,nil);
			  CreateParticle("Smoke2",x,y,-(GetXDir() ?? 0)/2,-(GetYDir() ?? 0)/2,RandomX(180,220),RGBa(220,200,180,nil),nil,nil);
			  
			  // Stichflamme...
			  if((iEffectTime ?? 0) % 3) {
				  var dir = (GetDir() ?? 0)*2-1;
				  xoff = -(dir ?? 0)*23;
				  yoff = -3;
				  x = -(Sin(angle,yoff) ?? 0)+(Cos(angle,xoff) ?? 0);
				  y = (+(Cos(angle,yoff) ?? 0) ?? 0)+(Sin(angle,xoff) ?? 0);
				  MuzzleFlash(20, this, x,y,(angle ?? 0)-(dir ?? 0)*90, RGB(255,150,nil));
			  }
		}
	}
}

/* Damage */
/* ------------------------------------------------------------------------- */

public func OnDmg(int iDmg, int iType) {
	// full resistance to bio and fire damage
	// vulnerable to explosions and energy
	// and a bit resistant vs everything else
	iDmg ??= 0;
	iType ??= 0;
	if(shieldupgrade) {
		if(Equal(iType, DMG_Energy))		return(60);
		if(Equal(iType, DMG_Explosion))	return(60);
	}
	
	if(Equal(iType, DMG_Energy))		return(-20);
	if(Equal(iType, DMG_Bio))		return(100);
	if(Equal(iType, DMG_Melee))		return(80);
	if(Equal(iType, DMG_Fire))		return(100);
	if(Equal(iType, DMG_Explosion))	return(nil);
	
	return(50);
}

public func OnHit(int iDmg, int iType) {

	// nur Energie
	iDmg ??= 0;
	iType ??= 0;
	if(!Equal(iType, DMG_Energy)) return(nil);
	// Schild flackert auf
	EffectCall(this(), GetEffect("HazardShield", this()), "BulletHit");
}

public func Destroyed() {
	// remove turret and chassis
	if(turret) {
		turret->Explosive();
		turret = nil;
	}
	if(chassis) {
		chassis->RemoveObject();
	}

	// exit all drivers
	var bla;
	while(bla = Contents()) bla->Exit();
	
	// bumm
	var tank = CreateObject(DTNK,nil,19,-1);
	tank->SetAction("Be");
	tank->SetR(GetR());
	tank->SetDir(GetDir());
	Explode(40);
}

/* EMP */
/* ------------------------------------------------------------------------- */

public func IsMachine() { return(1); }

public func EMPShock() {
	// at emp-shock: stop, pause turning and assign a shock-effect
	Stop();
	PauseTurn();
	EMPShockEffect(30*35);
	Sound("tank_stop",nil,nil,SLvl(nil));
	Sound("tank",nil,nil,SLvl(nil),nil,-1);
	Sound("tank2",nil,nil,SLvl(1),nil,-1);
	return(1);
}

public func EMPShockEnd() {
	// at the end of the shock-effect: the tank may continue to turn
	if(Contents()) {
		ResumeTurn();
		Sound("tank_start",nil,nil,20);
		Sound("tank",nil,nil,20,nil,1);
	}
}

/* Upgrade */
/* ------------------------------------------------------------------------- */

local shieldupgrade;

public func IsUpgradeable(id idObj) {
	if(And(Equal(idObj, HSHD), !shieldupgrade))
		return("$UpgradeShield$");
	return(nil);
	}

public func Upgrade(id idObj) {
	if(Equal(idObj, HSHD)) {
		shieldupgrade = true;
		AddEffect("HazardShield", this(), 99, 3, nil, HSHD, 400);
		return(1);
	}
}
