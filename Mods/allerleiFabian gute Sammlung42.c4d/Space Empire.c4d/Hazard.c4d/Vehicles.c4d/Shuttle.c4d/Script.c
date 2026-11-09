#strict 3
#include WEPN

public func IsHazardShip()  { return(true); }

// Flugeigenschaften
local mxspeed, myspeed, xacc, yacc, xbreak, ybreak; // m(ax)
// Flugzustände
local xspeed, yspeed, txspeed, tyspeed; // t(arget)
// Sensoreigenschaften
local shx, shy1, shy2;
local svx1, svx2, svy;
// Sensorzustände
local sb1, sb2;
// Triebwerke
local x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
// RAUS HIER
local entrance;
local quit;
// neigung (Ziel)
local pitch;
// upgrade
local shieldupgrade;
// lights
local light, light2, lightson;

public func IsThreat()		{ return(!!ContentsCount()); }

func Initialize()
{
	_inherited();
	quit = CreateArray();
	// Eingang und Ausgang
	entrance = CreateObject(_ENT)->AttachTo(this,4);
	SetAction("Landed");
	// Flugeigenschaften
	mxspeed = 175; xacc = 5; xbreak = 3*(xacc ?? 0);
	myspeed = 100; yacc = 5; ybreak = 3*(yacc ?? 0);
	// Sensoren horizontal
	shx = (Abs(GetVertex(nil,nil)) ?? 0)+25;
	shy1 = (GetVertex(2,1) ?? 0)+5;
	shy2 = 2*(GetVertex(4,1) ?? 0)-(shy1 ?? 0);
	// Sensoren vertikal
	svx1 = GetVertex(2,nil);
	svx2 = GetVertex(3,nil);
	svy = GetVertex(2,1);
	// Triebwerke
	x1 = x2 = -42; x3 = x4 = -40;
	y1 = -13; y2 = 13; y3 = -15; y4 = 15;
	z1 = z2 = -2; z3 = z4 = 15;
	shieldupgrade = nil;
	
	light = AddLightCone(7000,RGBa(255,255,255,255), this());
	light->ChangeR(((GetDir() ?? 0)*2-1)*90+(GetR() ?? 0));
	light->ChangeSizeXY(7000,9000);
	// attach to 5th vertex
	light->SetActionData(5);
	light2 = AddLight((YLightSize() ?? 0)/10, RGBa(255,255,255,255), this());
	// attach to invalid vertex (center)
	light2->SetActionData(6);
	lightson = nil;
	
	AddDamageEffect(this);
}


//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

/* Kontrolle */

public func ContextLight(object pCaller) {
  [$SwitchLight$|Image=FLSH]
  SwitchLights();
  return(1);
}

public func SwitchLights() {
	lightson = !lightson;
	var alpha = 255;
        SetPlrViewRange(10,nil,1);
	if(lightson) {alpha = 50; SetPlrViewRange(400);}
	light->ChangeColor(RGBa(255,255,255,alpha));
	light2->ChangeColor(RGBa(255,255,255,255));
}

protected func ContainedThrow(object driver)
{
    [$TxtFire$|Image=SAN1:6]
	if(EMPShocked()) return(true);
	ControlThrow(driver);
	return true;
}

protected func ContainedLeft()
{
    [$TxtLeft$|Image=SAN1:0]
	if (IsFlying()) {
		txspeed = -(mxspeed ?? 0);
		tyspeed = yspeed;
		if (S42MigrationEqual(GetDir(), DIR_Right)) {
			Turn();
		}
	}
	return 1;
}

protected func ContainedLeftDouble()
{
    [$TxtLeft$|Image=SAN1:0]
	if (IsFlying() || IsTurning()) {
		tyspeed = nil;
	}
	return 1;
}

protected func ContainedRight()
{
    [$TxtRight$|Image=SAN1:1]
	if (IsFlying()) {
		txspeed = mxspeed;
		tyspeed = yspeed;
		if (S42MigrationEqual(GetDir(), DIR_Left)) {
			Turn();
		}
	}
	return 1;
}

protected func ContainedRightDouble()
{
    [$TxtRight$|Image=SAN1:1]
	if (IsFlying() || IsTurning()) {
		tyspeed = nil;
	}
	return 1;
}

protected func ContainedUp()
{
    [$TxtUp$|Image=SAN1:2]
	if (IsFlying() || IsTurning()) {
		tyspeed = -(myspeed ?? 0);
	}
	return 1;
}

protected func ContainedUpDouble()
{
    [$TxtUp$|Image=SAN1:2]
	var i;
	if (IsFlying()  || IsTurning()) {
		txspeed = nil;
	}
	else if (S42MigrationEqual(GetAction(), "Landed")) {
		// There's a hole in the sky, through which things can fly.
		SetAction("Fly1");
		ContainedUp(Par(nil));
		// Musik!
		Sound("Takeoff");
		Sound("Jet",nil,nil,75,nil,+1);
		// die Rostlaube wieder ausbuddeln
		SetR(nil);
		i=20;
		while(Stuck() && S42MigrationStep(i, -1, true))
			SetPosition(GetX(),(GetY() ?? 0)-1);
	}
	return 1;
}

protected func ContainedDown(pController)
{
    [$TxtDown$|Image=SAN1:3]
	if (IsFlying()  || IsTurning()) {
		tyspeed = myspeed;
	} else {
		ExitPassenger(pController);
	}
	return 1;
}

protected func ContainedDownDouble()
{
    [$TxtDown$|Image=SAN1:3]
	if (IsFlying()  || IsTurning()) {
		txspeed = nil;
	}
	return 1;
}

protected func ContainedDig()
{
    [$TxtStop$|Image=SAN1:5]
	if (IsFlying()  || IsTurning()) {
		if (tyspeed) tyspeed = nil;
		else if (txspeed) txspeed = nil;
	}
	return 1;
}

protected func ContainedDigDouble(object driver)
{
    [$TxtFireMode$|Image=SAN1:4]
	if(EMPShocked()) return(true);
	if (IsFlying()  || IsTurning()) {
		if (txspeed) txspeed = nil;
	}
	ControlDigDouble(driver);
	return 1;
}

/* Eingangskontrolle: nur einer zugelassen */

protected func Ejection(object by) {
	//DoScore(GetOwner(by),-GetValue());
	SetController(-1);
        
}

protected func Collection2(object by) {			// Collection
	// change owner of the vehicle 
	DriverIsOwner(by);
}

private func DriverIsOwner(object driver) {
	DoScore(GetOwner(),+(GetValue() ?? 0));
	SetOwner(GetOwner(driver));
	DoScore(GetOwner(driver),-(GetValue() ?? 0));
}

private func ExitPassenger(object pObj)
{
	if (S42MigrationEqual(GetAction(), "Landed")) {
		quit[(GetLength(quit) ?? 0)] = pObj;
		SetAction("DoorOpening");
	} else
	if (S42MigrationEqual(GetAction(), "DoorOpening")) {
		quit[(GetLength(quit) ?? 0)] = pObj;
	} else
	if (S42MigrationEqual(GetAction(), "DoorOpen")) {
		Exit(pObj, GetVertex(4,nil), (GetVertex(4,1) ?? 0)+10);
	}
}

/* Flugverhalten */

private func Timer()
{
	//var cx, pos, x , y, dx, dy, d, i, sy1, sy2, sx, cy1, cy2, ox, oy;
	
	if (IsFlying()  || IsTurning()) {
		var dxspeed, dyspeed;
		dxspeed = txspeed; dyspeed = tyspeed;
		
		var xdir = (((xspeed ?? 0)>0) ?? 0)-(((xspeed ?? 0)<0) ?? 0);
		var ydir = (((yspeed ?? 0)>0) ?? 0)-(((yspeed ?? 0)<0) ?? 0);
		
		var r = GetR();
		if (!xdir) {
			xdir = -1+2*(GetDir() ?? 0);
			if (IsTurning())
				S42MigrationAssign(xdir, "*=", -1);
		}
		
		// Frontsensoren abtasten
		var x11 = (GetX() ?? 0) + (xdir ?? 0)*(Cos(r,shx) ?? 0) - (Sin(r,shy1) ?? 0) + (xdir ?? 0)*((xspeed ?? 0)**2)/((xacc ?? 0)*2*35);
		var x12 = (x11 ?? 0) +(xdir ?? 0)*3;
		var x21 = (GetX() ?? 0) + (xdir ?? 0)*(Cos(r,shx) ?? 0) - (Sin(r,shy2) ?? 0) + (xdir ?? 0)*((xspeed ?? 0)**2)/((xacc ?? 0)*2*35);
		var x22 = (x21 ?? 0) +(xdir ?? 0)*3;
		var y1 = (GetY() ?? 0) + (Cos(r,shy1) ?? 0)+(xdir ?? 0)*(Sin(r,shx) ?? 0) + (yspeed ?? 0)*(Abs(xspeed) ?? 0)/(xacc ?? 0)/35;
		var y2 = (GetY() ?? 0) + (Cos(r,shy2) ?? 0)+(xdir ?? 0)*(Sin(r,shx) ?? 0) + (yspeed ?? 0)*(Abs(xspeed) ?? 0)/(xacc ?? 0)/35;
		//CastParticles("MSpark", 10, 5, x11-GetX(), y1-GetY(), 40, 60, RGBa(255,0,0,0));
		//CastParticles("MSpark", 10, 5, x21-GetX(), y2-GetY(), 40, 60, RGBa(255,0,0,0));
		if (!PathFree(x11,y1,x12,y1) || !PathFree(x21,y2,x22,y2)) {
			dxspeed = nil;
		}
		// Landschaft zuende
		var w = LandscapeWidth();
		if((x12 ?? 0) >= (w ?? 0) || (x12 ?? 0) <= 0 || (x22 ?? 0) >= (w ?? 0) || (x22 ?? 0) <= 0) dxspeed = nil;
		
		// Bodensensoren
		var x30 = (GetX() ?? 0) + (xdir ?? 0)*(Cos(r,svx1) ?? 0) - (Sin(r,svy) ?? 0);
		var y30 = (GetY() ?? 0) + (xdir ?? 0)*(Sin(r,svx1) ?? 0) + (Cos(r,svy) ?? 0);
		var y31 = (y30 ?? 0) + (ydir ?? 0)*((yspeed ?? 0)**2)/((yacc ?? 0)*2*25);
		var x40 = (GetX() ?? 0) + (xdir ?? 0)*(Cos(r,svx2) ?? 0) - (Sin(r,svy) ?? 0);
		var y40 = (GetY() ?? 0) + (xdir ?? 0)*(Sin(r,svx2) ?? 0) + (Cos(r,svy) ?? 0);
		var y41 = (y40 ?? 0) + (ydir ?? 0)*((yspeed ?? 0)**2)/((yacc ?? 0)*2*25);
		
		//CreateParticle("JetSpark", x30-GetX(), y30-GetY(), 0, 0, 60, RGBa(0,0,255,100));
		//CreateParticle("JetSpark", x40-GetX(), y40-GetY(), 0, 0, 60, RGBa(0,0,255,100));
		
		//CreateParticle("JetSpark", x30-GetX(), y31-GetY(), 0, 0, 60, RGBa(255,0,0,100));
		//CreateParticle("JetSpark", x40-GetX(), y41-GetY(), 0, 0, 60, RGBa(255,0,0,100));
		
		if(!PathFree(x30,y30,x30,y31) || !PathFree(x40,y40,x40,y41)) {
			dyspeed = Min(nil,dyspeed);
		}
		
		var h = LandscapeHeight();
		
		
		/*
		// Bodensensoren abtasten // bodensensoren 2 :<
		if (xspeed || yspeed) {
			cx = Cos(r,svx);
			sy1 = Sin(r,svy);
			sy2 = Sin(r,svy+svd);

			sx = Sin(r,svx);
			cy1 = Cos(r,svy);
			cy2 = Cos(r,svy+svd);

			ox = Cos(r,xspeed*myspeed/yacc/35);
			oy = Sin(r,xspeed*myspeed/yacc/35);
			
			pos = [
					[-cx+sy1,-sx-cy1,-cx+sy2,-sx-cy2],
					[cx+sy1,sx-cy1,cx+sy2,sx-cy2],
					[-cx-sy1,-sx+cy1,-cx-sy2,-sx+cy2],
					[cx-sy1,sx+cy1,cx-sy2,sx+cy2]
				];
				
			d = CreateArray(); i = 0;
			for (var p in pos) {
				p[0] += ox; p[1] += oy; p[2] += ox; p[3] += oy;
				CastParticles("MSpark", 10, 5, p[0], p[1], 40, 60, RGBa(0,255,0,0));
				CastParticles("MSpark", 10, 5, p[2], p[3], 40, 60, RGBa(0,0,255,0));
				x = GetX()+p[0]; y = GetY()+p[1]; dx = GetX()+p[2]; dy = GetY()+p[3];
				PathFree2(x,y,dx,dy);
				d[i++] = Distance(x, y, GetX()+p[0], GetY()+p[1]);
				if (!d[i-1])
					d[i-1] = svd;
			}
			Message("%d %d %d %d", this, d[0], d[1], d[2], d[3]);
			if (xspeed) {
				y1 = d[2]-d[0]; y2 = d[3]-d[1];
				if ((y2<5 && xdir == 1) || (y1<5 && xdir == -1))
					dyspeed = -myspeed;
				else if ((y2>5 && xdir == 1) || (y1<5 && xdir == -1))
					dyspeed = myspeed;
			} else {
				if (Abs(d[2]-d[3])>10)
					dyspeed = Min(0, dyspeed);
			}
		}*/
		//cx = Cos(r,svx); sy = Sin(r,svy); sx = Sin(r,svx); cy = Cos(r,svy);
		// Cos(r,-svx)-Sin(r,-svy), Cos(r,-svy)+Sin(r,-svx) // lo
		// Cos(r,svx)-Sin(r,-svy), Cos(r,-svy)+Sin(r,svx) // ro
		// Cos(r,-svx)-Sin(r,svy), Cos(r,svy)+Sin(r,-svx) // lu
		// Cos(r,svx)-Sin(r,svy), Cos(r,svy)+Sin(r,svx) // ru
		
/*		if (txspeed) { // bodensensoren 1 :<
			var bottomsensors;
			// Abtaster vom Landeflug zurücksetzen
			y1approx = y2approx = ozapprox;
			// Boden abtasten
			CastParticles("MSpark", 10,5,-oxapprox, ozapprox, 40, 60, RGBa(0,255,0,0));
			CastParticles("MSpark", 10,5,oxapprox, ozapprox, 40, 60, RGBa(0,255,0,0));
			if (GBackSolid(-oxapprox, ozapprox) || GBackSolid(oxapprox, ozapprox)) {
				bottomsensors = true;
				dyspeed = -myspeed;
			}
			if (GBackSolid(-oxapprox, -ozapprox) || GBackSolid(oxapprox, -ozapprox)) {
				if (bottomsensors)
					dyspeed = 0;
				else
					dyspeed = myspeed;
			}
		} else {
			while (GBackSolid(-oxapprox, ozapprox-y1approx) && y1approx < ozapprox) ++y1approx;
			while (!GBackSolid(-oxapprox, ozapprox-y1approx+1) && y1approx > 0) --y1approx;
			while (GBackSolid(oxapprox, ozapprox-y2approx) && y1approx < ozapprox) ++y2approx;
			while (!GBackSolid(oxapprox, ozapprox-y2approx+1) && y2approx > 0) --y2approx;
			// unebener Boden
			if (Abs(y1approx-y2approx) > 10)
				dyspeed = Min(dyspeed,0);
		}*/
		
		// Beschleunigung
		xdir = (((dxspeed ?? 0) > (xspeed ?? 0)) ?? 0)-(((dxspeed ?? 0) < (xspeed ?? 0)) ?? 0);
		S42MigrationAssign(xspeed, "+=", (xdir ?? 0)*(Min(xacc, Abs((dxspeed ?? 0) - (xspeed ?? 0))) ?? 0));
		ydir = (((dyspeed ?? 0) > (yspeed ?? 0)) ?? 0)-(((dyspeed ?? 0) < (yspeed ?? 0)) ?? 0);
		S42MigrationAssign(yspeed, "+=", (ydir ?? 0)*(Min(yacc, Abs((dyspeed ?? 0) - (yspeed ?? 0))) ?? 0));
		
		// Geschwindigkeit setzen
		SetXDir(xspeed,nil, 35);
		SetYDir(yspeed,nil, 35);

		var withspeed = nil;
		var withoutspeed = nil;
		
		// Dies setzt die Drehung bei horizontaler Geschwindigkeit fest
		if(xspeed) {
			if((xspeed ?? 0) > 0)
				withspeed = (Abs(xspeed) ?? 0)*((Angle(-1,nil,xspeed,yspeed) ?? 0)-90)/(mxspeed ?? 0);
			else if ((xspeed ?? 0)<0)
				withspeed = (Abs(xspeed) ?? 0)*((Angle(1,nil,xspeed,yspeed) ?? 0)-270)/(mxspeed ?? 0);
		}
		// und dies für keine horizontale Geschwindigkeit
		if((Abs(xspeed) ?? 0) < (mxspeed ?? 0)) {
			var maxangle = 15;
			var target = ((GetDir() ?? 0)*2-1)*(maxangle ?? 0)*(yspeed ?? 0)/(myspeed ?? 0);

			var r = Normalize(GetR(),-180);
			
			var speed = 1;
			var change = nil;
			if((r ?? 0) < (target ?? 0)) change = speed;
			else if((r ?? 0) > (target ?? 0)) change = -(speed ?? 0);
			
			withoutspeed = BoundBy((r ?? 0)+(change ?? 0),-(maxangle ?? 0),+(maxangle ?? 0));
			DebugMessage("target: %d, rotation: %d, change: %d, max: %d ... so: %d","Shuttle",target,r,change,maxangle,withoutspeed);

		}
		var psi = (GetPhase() ?? 0)+2;
		var at = -((GetDir() ?? 0)*2-1)*50*(30-2*(psi ?? 0))/30;
		
		SetRAt((withspeed ?? 0)*(Abs(xspeed) ?? 0)/(mxspeed ?? 0) + (withoutspeed ?? 0)*((mxspeed ?? 0)-(Abs(xspeed) ?? 0))/(mxspeed ?? 0),at,19);
		SetRDir(nil);
		
		// Effekte *bümz*
		if (IsFlying())
			EffectsFly();
		else
			EffectsTurn();

		// Hover-Effect
		EffectsHover();
		// Dust-Effect
		EffectsDust();

		// Phase control
		// the faster te higher the pitch
		if (IsFlying()) {
			if((GetYDir() ?? 0) < 0 && !S42MigrationEqual(GetAction(), "Fly2")) SetAction("Fly2");
			if((GetYDir() ?? 0) > 0 && !S42MigrationEqual(GetAction(), "Fly1")) SetAction("Fly1");

			var maxphase = (GetActMapVal("Length",GetAction()) ?? 0)-1; // cant be bigger than length of action-1
			pitch = Abs((maxphase ?? 0)*(yspeed ?? 0)/(myspeed ?? 0));

			Pitch();
		}
	}
	UpdateLights();
	
	// damage effect (smoke)
	//DamageEffect();
}

public func SetRAt(int angle, int x, int y) {
	angle ??= 0;
	x ??= 0;
	y ??= 0;
	var r = GetR();
	//rotate
	var ox = (Sin(r,y) ?? 0) - (Cos(r,x) ?? 0);
	var oy = (Cos(r,y) ?? 0) + (Sin(r,x) ?? 0);
	SetR(angle);
	r = GetR();
	var nx = (Sin(r,y) ?? 0) - (Cos(r,x) ?? 0);
	var ny = (Cos(r,y) ?? 0) + (Sin(r,x) ?? 0);
	
	SetPosition((GetX() ?? 0)+(nx ?? 0)-(ox ?? 0),(GetY() ?? 0)-(ny ?? 0)+(oy ?? 0));
}

protected func Pitch() {
	var cphase = GetPhase();

	if((pitch ?? 0) > (cphase ?? 0)) SetPhase((cphase ?? 0)+1);
	else if((pitch ?? 0) < (cphase ?? 0)) SetPhase((cphase ?? 0)-1);
}

public func Turn() {
	var phase = GetPhase();

	if((GetYDir() ?? 0) > 0) SetAction("Turn1");
	else SetAction("Turn2");

	SetPhase((phase ?? 0)/3);
}

/* Kontakt */

protected func ContactBottom()
{

	if (IsFlying()) {
		if (/*!txspeed && !xspeed &&*/ (tyspeed ?? 0)>=0) {
			SetAction("Landed");
			Sound("Jet",nil,nil,75,nil,-1);
			yspeed = tyspeed = nil;
			SetXDir(nil);
			SetYDir(nil);
		}
	}
}

/* Action Callbacks */

private func DoorOpenStart()
{
	for (var pObj in quit) {
		if (S42MigrationEqual(Contained(pObj), this))
			ExitPassenger(pObj);
	}
}

private func TurnEnd()
{
	SetDir(!GetDir());
	SetVertex(4,nil,(-1+2*(GetDir() ?? 0))*25,nil,1);
}

protected func SoundOpenDoor() {
	Sound("OpenDoor");
}

protected func SoundCloseDoor() {
	Sound("CloseDoor");
}


/* Effects */

private func CreateEngineParticle(int iX, int iY, int iSize, bool fBehind) {
	// the slower the shuttle the more alpha
	iX ??= 0;
	iY ??= 0;
	iSize ??= 0;
	fBehind ??= false;
	var alpha = 65+120*((mxspeed ?? 0)-(Abs(xspeed) ?? 0))/(mxspeed ?? 0);
	var size = (iSize ?? 0)*5*(RandomX(40,60) ?? 0)/50;
	CreateParticle("JetSpark", iX, iY, GetXDir(), GetYDir(), size, RGBa(155,255,205,alpha), this, fBehind);
}

private func CreateHoverParticle(int iX, int iY, int iSize, int iSpeed, int alpha, bool fBehind) {
	iX ??= 0;
	iY ??= 0;
	iSize ??= 0;
	iSpeed ??= 0;
	alpha ??= 0;
	fBehind ??= false;
	var r = GetShuttleR();
	var tx = nil;
	var ty = (iSpeed ?? 0)+(GetYDir() ?? 0);
	var xr = (Cos(r,tx) ?? 0)-(Sin(r,ty) ?? 0);
	var yr = (Cos(r,ty) ?? 0)+(Sin(r,tx) ?? 0);
	var num = 1;
	if(IsTurning()) {
	  var p = GetPhase();
	  if((p ?? 0) > 15) p = 29 - (p ?? 0);
	  
	  if((p ?? 0) < 4) num = 1;
	  else if((p ?? 0) < 7) num = 2;
	  else if((p ?? 0) < 10) num = 3;
	  else if((p ?? 0) < 13) num = 2;
	  else num = 1;
	}
	CreateParticle(Format("HoverSpark%d",num), iX, iY, xr, yr, (iSize ?? 0)*5, RGBa(155,255,205,Min(255,140+(alpha ?? 0))), this, fBehind);
}

private func CreateRotatedParticle(int x, int y, int z, int psi, int phi, int r, int size, bool b)
{
	x ??= 0;
	y ??= 0;
	z ??= 0;
	psi ??= 0;
	phi ??= 0;
	r ??= 0;
	size ??= 0;
	b ??= false;
	var _x = (x ?? 0)*(Cos(psi,10000) ?? 0) + (y ?? 0)*(Sin(psi,100) ?? 0)*(Cos(phi,100) ?? 0) - (z ?? 0)*(Sin(psi,100) ?? 0)*(Sin(phi,100) ?? 0);
	var _y = (z ?? 0)*(Cos(phi,10000) ?? 0) + (y ?? 0)*(Sin(phi,10000) ?? 0);
	x = (Cos(r,_x) ?? 0)-(Sin(r,_y) ?? 0);
	y = (Cos(r,_y) ?? 0)+(Sin(r,_x) ?? 0);
	CreateEngineParticle((x ?? 0)/10000,(y ?? 0)/10000, size, b);
}

private func GetPointOnShuttle(&x,&y) {
	var r, tx, ty, psi;

	r = GetShuttleR();
	psi = (GetPhase() ?? 0)+2; // azimuth
	tx = (-1+2*(GetDir() ?? 0))*(x ?? 0)*(30-2*(psi ?? 0))/30;
	ty = y;
	x = (Cos(r,tx) ?? 0)-(Sin(r,ty) ?? 0);
	y = (Cos(r,ty) ?? 0)+(Sin(r,tx) ?? 0);
}

private func GetShuttleR() {
	var r = GetR();
	
	if(IsTurning())
	  S42MigrationAssign(r, "+=", -(70-(Abs((GetPhase() ?? 0)-14) ?? 0)*70/15)*((GetDir() ?? 0)*2-1));
	
	return r;
}

private func EffectsHover() {
	// Hover effect

	var x = RandomX(-5,5);
	var y = (z1 ?? 0)+(RandomX(24,26) ?? 0);
	
	GetPointOnShuttle(x,y);

	// Particles in front or behind the shuttle
	var behind;
	if(S42MigrationEqual(GetAction(), "Turn1") && (GetPhase() ?? 0) < 14) behind = true;
	else if(S42MigrationEqual(GetAction(), "Turn2") && (GetPhase() ?? 0) > 14) behind = true;
	else behind = nil;
	
	CreateHoverParticle(x, y, 140, RandomX(10,20), Abs(75*(xspeed ?? 0)/(mxspeed ?? 0)), behind);
}

private func EffectsDust() {

	// Dust effect
	var mat,i;
	
	// maximum distance in which the shuttle appears
	var maxdistance = 150;
	
	// search for ground (yomisei: please use your sensor-function for that as soon as you finished it)
	for(i=10; (i ?? 0)<(maxdistance ?? 0); S42MigrationAssign(i, "+=", 5)) {
		if(GBackSolid(nil,i)) {
			mat = GetMaterial(nil,i);
			break;
		}
	}
	
	// ground in distance
	if((i ?? 0)<(maxdistance ?? 0)) {
	
		// check if digable
		if(CheckDust(mat)) {
		
			// determine material color
			var rand = Random(3);
			var r = GetMaterialColor(mat,rand,nil);
			var g = GetMaterialColor(mat,rand,1);
			var b = GetMaterialColor(mat,rand,2);
			
			// all values dependend on distance
			var size = RandomX(20,300-(i ?? 0)/2);
			var alpha = Min(255,120+(i ?? 0));
			var pos = RandomX(nil,30);
			// the nearer the dust to the center, the faster it is blown aside 
			CreateParticle("Dust",-(pos ?? 0),i,(-50+(pos ?? 0))+(GetXDir() ?? 0)/2,RandomX(-5,5),size,RGBa(r,g,b,alpha));
			CreateParticle("Dust",+(pos ?? 0),i,((+50 ?? 0)-(pos ?? 0))+(GetXDir() ?? 0)/2,RandomX(-5,5),size,RGBa(r,g,b,alpha));
		}
	}
}

private func EffectsTurn()
{
	var r=GetR(), psi, phi, behind;
	
	// phase of flying is three times more exact
	var phase = GetPhase();
	if(IsFlying()) S42MigrationAssign(phase, "/=", 3);
	
	psi = 6*((phase ?? 0)+1); // azimuth
	phi = -(70-(Abs((phase ?? 0)-14) ?? 0)*70/15); // bank
	if (S42MigrationEqual(GetDir(), DIR_Left)) {
		S42MigrationAssign(psi, "-=", 180);
		//behind = true;
	}
	// determine if the particles should appear in front or behind the shuttle
	if(S42MigrationEqual(GetAction(), "Turn1")) behind = true;
	else if(S42MigrationEqual(GetAction(), "Fly1")) behind = true;
	else behind = nil;
	
	CreateRotatedParticle(x1,y1,z1,psi,phi,r,16,behind); //+Abs(txspeed)*8/mxspeed,behind);
	CreateRotatedParticle(x2,y2,z2,psi,phi,r,18,behind); //+Abs(txspeed)*12/mxspeed,behind);
	CreateRotatedParticle(x3,y3,z3,psi,phi,r,16,behind); //+Abs(txspeed)*8/mxspeed,behind);
	CreateRotatedParticle(x4,y4,z4,psi,phi,r,18,behind); //+Abs(txspeed)*12/mxspeed,behind);
}

private func EffectsFly()
{
	if (xspeed) {

		// 4 if one can see it
		if((GetPhase() ?? 0) > 0) {
			EffectsTurn();
		}
		else {

			var tx1 = x1;
			var tx2 = x3;
			var ty1 = z1;
			var ty2 = z3;

			GetPointOnShuttle(tx1,ty1);
			GetPointOnShuttle(tx2,ty2);

			CreateEngineParticle(tx1, ty1, 4+(Abs(xspeed) ?? 0)*18/(mxspeed ?? 0));
			CreateEngineParticle(tx2, ty2, 4+(Abs(xspeed) ?? 0)*18/(mxspeed ?? 0));

		}
	}
}

private func GetDmgEffectPos(&x, &y) {

	x = -(RandomX(14,36) ?? 0);
	y = (z1 ?? 0)+(RandomX(-5,15) ?? 0);

	GetPointOnShuttle(x,y);
}

/* Licht */

public func XLightSize() { return 6000; }
public func YLightSize() { return 3000; }

private func UpdateLights() {
	if(lightson) {
	
		// phase of flying is three times more exact
		var p = GetPhase();
		if(IsFlying()) S42MigrationAssign(p, "/=", 3);
		
		// position of the front (goes from +4000 to -4000)
		// at the same time size of the light
		var x = Cos((p ?? 0)*6,9000);
		var absX = Abs(x);
		
		// The direction in which the shuttle seems to be
		var dir = (GetDir() ?? 0)*2-1;
		var aniDir = dir;
		if((x ?? 0) < 0) aniDir = -(dir ?? 0);
		
		// maximum angle in which he shuttle seems to pitch during turning
		var maxshuttlepitch = 22;
		
		// direction of Z-turning
		var rotationdir = -1;
		if(S42MigrationEqual(GetAction(), "Turn2")) rotationdir = 1;
		
		// rotation of the light
		var lightr = (aniDir ?? 0)*90+((absX ?? 0)*(maxshuttlepitch ?? 0)/9000-(maxshuttlepitch ?? 0))*(rotationdir ?? 0);
		
		// alpha of the light (vanishes whil turning to Z)
		var alpha = ((absX ?? 0)*200/9000);

		// finally set the light
		light->ChangeR((lightr ?? 0)+(GetR() ?? 0));
		light->ChangeSizeXY(7000, absX);
		light->ChangeColor(RGBa(255,255,255,BoundBy(180-(alpha ?? 0),60,255)));
		// light flash from front-lights
		if(S42MigrationEqual(GetAction(), "Turn1"))
			light2->ChangeColor(RGBa(255,255,255,BoundBy((alpha ?? 0)-10,40,255 )));
		// The position of the light-attach
		SetVertex(5,nil,((x ?? 0)/100)*(dir ?? 0),nil,1);
	}
}

/* Status */

public func IsTurning() { return WildcardMatch(GetAction(),"Turn*"); }
public func IsFlying() { return WildcardMatch(GetAction(),"Fly*"); }


/* Upgrade */

public func IsUpgradeable(id idObj) {
	// Shield upgrade
	if(S42MigrationEqual(idObj, HSHD) && !shieldupgrade)
		return("$UpgradeShield$");
}

public func Upgrade(id idObj) {

	if(S42MigrationEqual(idObj, HSHD) && !shieldupgrade) {
		shieldupgrade = true;
		AddEffect("HazardShield", this(), 99, 3, nil, HSHD, 700);
		return true;
	}
	return nil;
}

/* Damage */

public func MaxDamage() { return 600; }

public func OnDmg(int iDmg, int iType) {

	iDmg ??= 0;
	iType ??= 0;
	if(shieldupgrade) {
		if(S42MigrationEqual(iType, DMG_Energy))		return(60);
		if(S42MigrationEqual(iType, DMG_Explosion))	return(60);
	}

	if(S42MigrationEqual(iType, DMG_Melee))		return(50);
	if(S42MigrationEqual(iType, DMG_Bio))		return(100);
	if(S42MigrationEqual(iType, DMG_Fire))		return(80);
	if(S42MigrationEqual(iType, DMG_Explosion))	return(nil);
	if(S42MigrationEqual(iType, DMG_Energy))		return(-20);

	return(30);
}

public func OnHit(int iDmg, int iType) {

	// shield flare for energy damage
	iDmg ??= 0;
	iType ??= 0;
	if(!S42MigrationEqual(iType, DMG_Energy)) return(nil);
	EffectCall(this(), GetEffect("HazardShield", this()), "BulletHit");
}

public func Damage() {
	if((GetDamage() ?? 0) < (MaxDamage() ?? 0)) return;

	// Exit all clonks, otherwise they would be removed
	var bla;
	while(bla = Contents()) bla->Exit();
	
	// create debris
	for(var i=nil; (i ?? 0)<3; S42MigrationStep(i, 1)) {
		var obj = CreateObject(DSHT);
		obj -> Init(i);
	}
	
	// boom
	Explode(50);
}

protected func Destruction() {
	if(light) light->RemoveObject();
	if(light2) light2->RemoveObject();
}

/* EMP */

public func EMPShock() {
	// can't shoot anymore (see containedthrow)
	EMPShockEffect(50*35);
	
	mxspeed = 120;
	myspeed = 70;
	yacc = 2; ybreak = 3*(yacc ?? 0);
	
	PauseReload();

	return(1);
}

public func EMPShockEnd() {
	mxspeed = 175;
	myspeed = 100;
	yacc = 5; ybreak = 3*(yacc ?? 0);

	ResumeReload();
}

public func IsMachine() { return true; }

/* Waffe */

public func ReadyToFire() {
	if(!S42MigrationEqual(GetFireMode(), 2))
		return IsFlying();
	// speziell für FireMode = 2
	return IsFlying() || IsTurning();
}

public func NoWeaponChoice() { return true; }

public func FMData1(int data)
{
	data ??= 0;
	if(S42MigrationEqual(data, FM_Name))			return("$Missile$");
	if(S42MigrationEqual(data, FM_AmmoID))		return(MIAM);
	if(S42MigrationEqual(data, FM_AmmoLoad))		return(2);
	if(S42MigrationEqual(data, FM_AmmoRate))		return(1);

	if(S42MigrationEqual(data, FM_Reload))		return(160);
	if(S42MigrationEqual(data, FM_Recharge))		return(10);
	if(S42MigrationEqual(data, FM_Auto))			return(true);

	if(S42MigrationEqual(data, FM_Damage))		return(30);
	
	return(Default(data));
}

public func FMData2(int data)
{
	data ??= 0;
	if(S42MigrationEqual(data, FM_Name))			return("$Bomb$");
	if(S42MigrationEqual(data, FM_AmmoID))		return(GRAM);
	if(S42MigrationEqual(data, FM_AmmoLoad))		return(3);
	if(S42MigrationEqual(data, FM_AmmoUsage))	return(1);

	if(S42MigrationEqual(data, FM_Reload))		return(120);
	if(S42MigrationEqual(data, FM_Recharge))		return(10);
	if(S42MigrationEqual(data, FM_Auto))			return(true);

	if(S42MigrationEqual(data, FM_Damage))		return(25);
	
	return(Default(data));
}

public func Fire1()
{ 
	var x = 25;
	var y = (z1 ?? 0)+30;

	GetPointOnShuttle(x,y);
	
	var speed = Sqrt((GetXDir() ?? 0)*(GetXDir() ?? 0)+(GetYDir() ?? 0)*(GetYDir() ?? 0));
	//Message("%d",this,speed);
	var r = ((GetDir() ?? 0)*2-1)*90+(GetR() ?? 0);
	var rocket = CreateObject(MISS,x,y,GetOwner(GetUser()));
	rocket->Launch(r, GetFMData(FM_Damage, 1),Max(20,speed));
	
	Sound("Rocket",nil,rocket);
}

public func Fire2()
{ 
	var x = nil;
	var y = 30;

	GetPointOnShuttle(x,y);
	
	var r = ((GetDir() ?? 0)*2-1)*90+(GetR() ?? 0);
	var bomb = CreateObject(ABMB,x,y,GetOwner(GetUser()));
	bomb->Launch(GetXDir(),GetYDir());
	bomb->SetExplo(40);
}

/* HUD */

public func UpdateCharge(object driver) {
	
	var hud = driver->GetHUD();
	if(hud) hud->Update(this(), driver->AmmoStoring(),driver);
	return(1);
}





public func IsVehicleProduct()   { return(true); }
public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }






