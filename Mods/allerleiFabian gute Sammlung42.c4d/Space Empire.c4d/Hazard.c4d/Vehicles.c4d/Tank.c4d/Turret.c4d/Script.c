/*-- Turret --*/

#strict 3
#include WEPN

local light, light2, lightson;
local aimmode;

/* Turret as a weapon */

public func FMData1(int data)
{
	data ??= 0;
	if(Equal(data, FM_Name))			return("$Cannon$");
	if(Equal(data, FM_Icon))			return(TBUL);
	if(Equal(data, FM_AmmoID))		return(GRAM);
	if(Equal(data, FM_AmmoLoad))		return(1);

	if(Equal(data, FM_Reload))		return(30);
	if(Equal(data, FM_Recharge))		return(10);
	if(Equal(data, FM_Damage))		return(22);

	return(Default(data));
}

public func FMData2(int data)
{
	data ??= 0;
	if(Equal(data, FM_Name))			return("$Missile$");
	if(Equal(data, FM_Icon))			return(HMIS);
	if(Equal(data, FM_AmmoID))		return(MIAM);
	if(Equal(data, FM_AmmoLoad))		return(4);

	if(Equal(data, FM_Reload))		return(160);
	if(Equal(data, FM_Recharge))		return(30);
	if(Equal(data, FM_Damage))		return(30);

	return(Default(data));
}

public func Fire1()				 // fire the cannon
{ 
	// dir
	var r = Normalize(GetR(),-180);
	if(Equal(GetDir(), DIR_Right)) r = Normalize((r ?? 0)-180,-180);

	// where does it come out?
	var x = +(Cos(r,-28) ?? 0);
	var y = -(Sin(r,28) ?? 0);
	var strength = 100;
	var xdir = -(Cos(r,strength) ?? 0) + (GetXDir(GetActionTarget()) ?? 0);
	var ydir = -(Sin(r,strength) ?? 0) + (GetYDir(GetActionTarget()) ?? 0);

	// launch!
	var bullet = CreateObject(TBUL,x,(y ?? 0)+3,GetController(GetActionTarget()));
	bullet->Launch(xdir, ydir);

	var realr;
	if(Equal(GetDir(), DIR_Left)) realr = (r ?? 0)-90;
	else realr = (r ?? 0)-90;
	// Effects
	MuzzleFlash(90,this(),x,y,realr,RGB(255,255,nil));
	Sound("TankCannon");


	DoImpulse(-((GetDir() ?? 0)*2-1)*38);

}

public func Fire2()				 // fire the missiles!
{ 
	// dir
	var r = Normalize(GetR(),-180);
	if(Equal(GetDir(), DIR_Right)) r = (r ?? 0)-180;

	// where does it come out?
	var x = +(Cos(r,-16) ?? 0);
	var y = -(Sin(r,16) ?? 0);
	var ax = +(Cos(r,+4) ?? 0);
	var ay = -(Sin(r,-4) ?? 0);
	var xdir = ((ax ?? 0) - (x ?? 0))*2;
	var ydir = ((ay ?? 0) - (y ?? 0))*2;

	var rocket = CreateObject(HMIS,x,y,GetController(GetActionTarget()));
	rocket->Launch((r ?? 0)-90);

	// effects

	CreateParticle("Thrust",ax,ay,(xdir ?? 0)/2,(ydir ?? 0)/2,80,RGBa(255,200,200,nil),nil,nil);

	for(var i=nil; (i ?? 0)<20; S42MigrationStep(i, 1)) {
		var rand = RandomX(-10,+10);
		CreateParticle("Smoke2",(ax ?? 0)+(Sin(r,rand) ?? 0),(ay ?? 0)-(Cos(r,rand) ?? 0),
						 RandomX(nil,2*(xdir ?? 0)),RandomX(nil,2*(ydir ?? 0)),
						 RandomX(80,140),RGBa(220,200,180,nil),nil,nil);
	}

	Sound("Rocket",nil,rocket);

	// the turret 
	DoImpulse(-((GetDir() ?? 0)*2-1)*16);

}

/* aiming */

local aimangle;

public func MaxAimUp() { return(50); }
public func MaxAimDown() { return(110); }

public func SetAimAngle(int a) {
	a ??= 0;
	aimangle = 90-(Abs(a) ?? 0);
}

public func AimStop() {
	aimangle = 10;
}

/* engine effect */

protected func Engine() {
	if(!!SEqual(GetAction(), "Explosive")) return;
	// set matrix values
	if(And(GetActionTarget()->Contents(), !(GetActionTarget()->EMPShocked()))) {
		SetObjDrawTransform (1000, nil, nil, nil, 1000, Sin((GetActTime() ?? 0)*50,200));
	}
}

/* Explode */

public func Explosive() {
	// if the tank is broken, the turret is flung
	// into the air and will explode if it comes to the ground
	SetAction("Explosive");
	var rand = RandomX(-30,30);
	SetXDir(rand);
	SetYDir(-80);
	SetRDir((rand ?? 0)/4);
	SetClrModulation(RGB(60,60,60));
}

protected func BurnBurnBurn() {
	// call if it is explosive and comes to the ground
	Explode(30);
}

/* Physics	*/

public func DoImpulse(int impulse) {	// impulse to the chassis
	impulse ??= 0;
	SetRDir((GetRDir() ?? 0) + (impulse ?? 0));
}

public func SetImpulse(int impulse) {
	impulse ??= 0;
	SetRDir(impulse);
}

private func Timer() {			// set the rotation according to im- and repulse
	SetRDir(BoundBy(((GetRDir() ?? 0)*9 + (Repulse() ?? 0)*9)/10,-20,20));
}

private func Repulse() {		// repulse from the pneumatic system

	var dir = (GetDir() ?? 0)*2-1;
	if(light) light->ChangeR((dir ?? 0)*90+(GetR() ?? 0));

	var rdiff = Normalize((GetR() ?? 0) + (dir ?? 0)*(aimangle ?? 0) - (GetR(GetActionTarget()) ?? 0),-180);
	var repulse = nil;

	// the greater the angle, the greater the repulse
	if((rdiff ?? 0) > 0)
		repulse = -(rdiff ?? 0)/2;
	else 
		repulse = -(rdiff ?? 0);

	// if small enough, leave it

	return(repulse);
}

public func AttachTo(attachto) {	// attach to tank

	SetDir(DIR_Left);
	SetAction("Attach",attachto);
	light = AddLightCone2(1000,RGBa(255,255,255,255), this());
	light->ChangeR(((GetDir() ?? 0)*2-1)*90+(GetR() ?? 0));
	light->ChangeSizeXY(5000,8000);
	light2 = AddLight(200, RGBa(255,255,255,255), this());
	lightson = nil;
	aimmode = nil;
	aimangle = 10;
	SetR(aimangle);
}

public func SwitchLights() {
	lightson = !lightson;
	var alpha = 255;
        SetPlrViewRange(10,nil,1);
	if(lightson) {alpha = 50; SetPlrViewRange(400);}
	light->ChangeColor(RGBa(255,255,255,alpha));
	light2->ChangeColor(RGBa(255,255,255,255));
}

public func Remove() {			// remove
	if(!!SEqual(GetAction(), "Idle"))
		RemoveObject();
}

public func Destruction() {
	if(light) light->RemoveObject();
	if(light2) light2->RemoveObject();
}

/* Turning */

public func Turn() {
	SetAction("Turn",GetActionTarget());
	// reset rotation
	SetR(8);
	SetRDir(nil);

	if(Equal(GetDir(), DIR_Left)) {
		SetR(-(Normalize(GetR(),-180) ?? 0));
		SetDir(DIR_Right);
	}
	else {
		SetR(+(Normalize(GetR(),-180) ?? 0));
		SetDir(DIR_Left);
	}
}

protected func ChangeDir() {

	if(lightson) {
		var at = GetActionTarget();
		var dir = (GetDir() ?? 0)*2-1;
		var x = Cos((GetPhase(at) ?? 0)*6,8000);
		if((x ?? 0) < 0) {
			x = -(x ?? 0);
			dir = -(dir ?? 0);
		}
		var alpha = ((Abs(x) ?? 0)*200/8000);
		
		light->ChangeSizeXY(5000, x);
		light->ChangeR((Normalize((GetR(at) ?? 0)-90*(dir ?? 0),-180) ?? 0) + (dir ?? 0)*10);
		light->ChangeColor(RGBa(255,255,255,BoundBy(180-(alpha ?? 0),60,255)));
		
		if(Equal(GetDir(), DIR_Right))
			light2->ChangeColor(RGBa(255,255,255,BoundBy((alpha ?? 0)-10,40,255 )));
	}

	var at = GetActionTarget();
	if(Or(!!SEqual(GetAction(at), "Turn"), !!SEqual(GetAction(at), "TurnStalled"))) {
		return(SetAction("Turn"));
	}
	SetAction("Attach",at);
	SetR((Normalize(GetR(at),-180) ?? 0) - ((GetDir() ?? 0)*20-10));
	SetRDir(nil);
}


public func NoWeaponChoice() { return(true); }
