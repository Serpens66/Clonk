/* Context Use drone functionality */

#strict 3

/* Adds the tanks physics to the vehicle ----

The vehicle may not have any vertices. There are only four virtual vertices
 (see below). The line
   return(_inherited());
 has to be added as the last line of the function Initialize to get it working.
 
 The control, when the vehicle should stop, go, jump, turn (SetDir) is up to you. 
 For a player controlled vehicle, it could look like this:
 
	 public func ContainedLeft() {
		if(GetDir() != DIR_Left) {
			SetDir(DIR_Left);
		}
		else if(!IsDriving()) Go();
}
 
 The rest of the configuration is explained below.

Interface:
Jump() - Jumps if it is driving on the ground
Go() - Drives on
Stop() - Stops
For changing the direction just use SetDir();

*/

/* Physics */
/* ------------------------------------------------------------------------- */

// Jumping - Jump()
public func JumpStrengthX() { return(40); }	// Strength
public func JumpStrengthY() { return(30); }
public func JumpDelay()		{ return(50); }	// minimum Delay until next Jump

// The vehicle has 4 virtual vertices. One left, one right, one on top, one on bottom
public func VertexLeft()	{ return(24); }
public func VertexRight()	{ return(24); }
public func VertexTop()		{ return(12); }
public func VertexBottom()	{ return(20); }
// VertexBottom should be bigger than XOffsetToWheels otherwise
// the vehicle may loose contact all the time

// X- and Y-Offset to wheels (which are 2) from the center: assumes that the distance
// to the wheels are the same from the center.
public func XOffsToWheels() { return(18); }
public func YOffsToWheels() { return(15); }

public func MaxSpeed()		{ return(1800); }	// maximum speed
public func BrakeStrengh()	{ return(200); }	// speed of braking
public func Acceleration()	{ return(200); }	// speed of speeding up

public func TurnAction()	{ return("Turn"); }	// name of turning-action. Leave out if there is none.

//public func CatLanding()	{ return(5); }

public func SpiderMode()	{ return(true);}	//experimental drive-on-walls and ceiling
public func MaxRotation()	{ return(70); }		// max. rotation of landscape it can stand (0 for no max rotation)

/* Callbacks */
/* ------------------------------------------------------------------------- */

public func OnGo() {}
public func OnStop() {}
public func OnJumpStart() {}
public func OnJumpEnd() {}

/* Initialization */

local stop;
local speed;

local left, right, top;
local groundl, groundm, groundr;
local xr,yr, xm, ym, xl, yl;
local falling;

protected func Initialize() {
	stop = true;
	speed = nil;
	falling = nil;
	
	AddEffect("Physics",this(),1,1,this());
	
	return(_inherited());
}


/* Jumping */

public func IsJumping() { return(GetEffect("JumpDelay",this())); }

public func Jump() {
	if(!IsTurning())
		if(IsDriving())
			if(And(And(!TopContact(), !LeftContact()), !RightContact()))
				if(BottomContact())
					if(!IsJumping()) {
						var dir = ((GetDir() ?? 0)*2-1)*1;
						
						var strx = (JumpStrengthX() ?? 0) * (dir ?? 0);
						var stry = JumpStrengthY();
						
						SetXDir((+(Sin(GetR(),stry) ?? 0) ?? 0)+(Cos(GetR(),strx) ?? 0));
						SetYDir(-(Cos(GetR(),stry) ?? 0)+(Sin(GetR(),strx) ?? 0));
						SetRDir(dir);
						AddEffect("JumpDelay",this(),1,1,this(),nil,JumpDelay());
						
						OnJumpStart();
						
						return(true);
					}
}

protected func FxJumpDelayTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	// Effekt
	iEffectNumber ??= 0;
	iEffectTime ??= 0;
	pTarget->~ThrustEffect(iEffectTime);
	
	S42MigrationStep(EffectVar(nil,pTarget,iEffectNumber), -1, true);
	if((EffectVar(nil,pTarget,iEffectNumber) ?? 0) <= 0) return(-1);
}

protected func FxJumpDelayStart(object pTarget, int iEffectNumber, int iTemp, int iTime) {
	iEffectNumber ??= 0;
	iTemp ??= 0;
	iTime ??= 0;
	EffectVar(nil,pTarget,iEffectNumber) = iTime;
}

public func ThrustEffect(int iEffectTime) { iEffectTime ??= 0;
    }

protected func FxPhysicsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	
	iEffectNumber ??= 0;
	iEffectTime ??= 0;
	SetR(GetR());
	
	ContactCheck();			// Kontakt prüfen
	
	// on ground?
	if(IsStuck()) {
		StuckStop();
		falling = nil;
	}
	else if(BottomContact()) {
		if(falling) {
			falling = nil;
			OnJumpEnd();
		}
		// driving
		if(IsDriving()) Drive();
		// Fix rotation and pos
		Stand();
	}
	else {
		// else fall
		Fall();
		falling = true;
	}
	
}

/* Find ground/walls */

public func BottomContact()	{
	if(IsJumping()) {
		if((GetEffect("JumpDelay",this(),nil,6) ?? 0) < 10)
			return(nil);
	}
	return(Or(Or((groundm ?? 0) < (VertexBottom() ?? 0), (groundl ?? 0) < (VertexBottom() ?? 0)), (groundr ?? 0) < (VertexBottom() ?? 0)));
}
public func TopContact()	{ return((top ?? 0) < (VertexTop() ?? 0)); }
public func LeftContact()	{ return((left ?? 0) < (VertexLeft() ?? 0)); }
public func RightContact()	{ return((right ?? 0) < (VertexRight() ?? 0)); }
public func IsStuck()		{ return((groundm ?? 0) < (VertexBottom() ?? 0)-15); }

private func ContactCheck() {

	right =	FindGround(-4,2);
	top = FindGround(nil,1);
	left =	FindGround(-4,3);
	
	groundm = GroundPos(nil,nil,xm,ym);
	groundl = GroundPos(-12,nil,xl,yl);
	groundr = GroundPos(+12,nil,xr,yr);
}

protected func FindGround(int x, int dir, bool downisdown) {
	x ??= 0;
	dir ??= 0;
	downisdown ??= false;
	var xpos, ypos;
	return(GroundPos(x,dir,xpos,ypos,downisdown));
}

protected func GroundPos(int x, int dir, &xpos, &ypos, bool downisdown) {

	x ??= 0;
	dir ??= 0;
	downisdown ??= false;
	var startx, starty, distx, disty, rev;

	var r = GetR();
	if(downisdown) r = nil;
	
	// Ground that is further away than about 30px is not relevant for the tank
	var dist;
	// down = 0, up = 1, right = 2, left = 3
	if(Equal(dir, nil)) { dist = (VertexBottom() ?? 0)*2; rev = nil; }
	if(Equal(dir, 2)) { dist = (VertexRight() ?? 0)*2; rev = nil; }
	if(Equal(dir, 1)) { dist = -(VertexTop() ?? 0)*2; rev = 1; }
	if(Equal(dir, 3)) { dist = -(VertexLeft() ?? 0)*2; rev = 1; }
	
	// up or down
	if(Or(Equal(dir, nil), Equal(dir, 1))) {
		startx = Cos(r,x);
		starty = Sin(r,x);
		distx = Sin(r,-(dist ?? 0));
		disty = Cos(r,dist);
	}
	// left or right
	if(Or(Equal(dir, 2), Equal(dir, 3))) {
		// a bit further down
		startx = Sin(r,-(x ?? 0));
		starty = Cos(r,+(x ?? 0));
		distx = Cos(r,dist);
		disty = Sin(r,dist);
	}
	
	var i = nil;
	var phase = nil;
	//  search...
	while((Abs(i) ?? 0) <= (Abs(dist) ?? 0)) {
		var newx = (startx ?? 0)+(distx ?? 0)*(i ?? 0)/(dist ?? 0);
		var newy = (starty ?? 0)+(disty ?? 0)*(i ?? 0)/(dist ?? 0);

		// landscape borders
		if(Or(Or((GetX() ?? 0)+(newx ?? 0) < 0, (GetX() ?? 0)+(newx ?? 0) >= (LandscapeWidth() ?? 0)), (GetY() ?? 0)+(newy ?? 0) >= (LandscapeHeight() ?? 0))) {
			if(Equal(phase, nil)) { phase = 1; continue; }
			else break;
		}
		// sky found... keh
		if(Equal(phase, nil)) {
			if((rev ?? 0) ^ (GBackSolid(newx,newy) ?? 0)) { phase = 1; continue; }
			S42MigrationStep(i, 1);
		}
		// solid found... keh
		if(Equal(phase, 1)) {
			if((rev ?? 0) ^ (!GBackSolid(newx,newy) ?? 0)) break;
			S42MigrationStep(i, -1);
		}
	}

	// i is our man!
	var groundx = (startx ?? 0)+(distx ?? 0)*(i ?? 0)/(dist ?? 0);
	var groundy = (starty ?? 0)+(disty ?? 0)*(i ?? 0)/(dist ?? 0);
	
	xpos = groundx;
	ypos = groundy;
	
	if(rev) i = -(i ?? 0);
	
	// if at the bottom and it is open: fall down
	if(GetScenBottomOpen())
		if(Equal((groundy ?? 0)+(GetY() ?? 0), LandscapeHeight()))
			return(1000);

	// returns the y-pos where there is ground ... 1000 if not found
	return(i);
}

/*  Rotation and Position */

public func SetRAt(int angle, int xoffs, int offs) {
	angle ??= 0;
	xoffs ??= 0;
	offs ??= 0;
	var r = GetR();
	//rotate
	var ox = (Sin(r,offs) ?? 0)-(Cos(r,xoffs) ?? 0);
	var oy = (Cos(r,offs) ?? 0)+(Sin(r,xoffs) ?? 0);
	SetR(angle);
	var nx = (Sin(GetR(),offs) ?? 0)-(Cos(GetR(),xoffs) ?? 0);
	var ny = (Cos(GetR(),offs) ?? 0)+(Sin(GetR(),xoffs) ?? 0);
	
	var xpos = (nx ?? 0)-(ox ?? 0);
	var ypos = -(ny ?? 0)+(oy ?? 0);
	
	SetPosition((GetX() ?? 0)+(xpos ?? 0),(GetY() ?? 0)+(ypos ?? 0));
}

private func FixPosition() {
	var gp = YOffsToWheels();
	// set the distance to ground to YOffsToWheels
	//var ground, xr,yr, xm, ym, xl, yl;
	//var groundm = GroundPos(0,0,xm,ym)-gp;
	//var groundl = GroundPos(-XOffsToWheels(),0,xl,yl)-gp;
	//var groundr = GroundPos(+XOffsToWheels(),0,xr,yr)-gp;
	var ground;
	
	// which point
	var min = Min(groundl,Min(groundm,groundr));
	var best = nil;
	if(Equal(min, groundm)) { ground = (groundm ?? 0)-(gp ?? 0); best = nil; }
	if(Equal(min, groundl)) { ground = (groundl ?? 0)-(gp ?? 0); best = -1; }
	if(Equal(min, groundr)) { ground = (groundr ?? 0)-(gp ?? 0); best = +1;}
	
	var groundx = Sin(GetR(),-(ground ?? 0));
	var groundy = Cos(GetR(),ground);
	
	SetPosition((GetX() ?? 0)+(groundx ?? 0),(GetY() ?? 0)+(groundy ?? 0));
	
	// set r at the highest
	/*var angle = 0;
	var max = Max(groundl,Max(groundm,groundr));
	if(max == groundm) angle = Angle(xl,yl,xr,yr)-90;
	if(max == groundl) angle = Angle(xm,ym,xr,yr)-90;
	if(max == groundr) angle = Angle(xl,yl,xm,ym)-90;
	
	SetRAt(angle,best*XOffsToWheels(),YOffsToWheels());
	*/
	//Message("best: %d - angle: %d - %d,%d,%d",this(),best,angle,groundl,groundm,groundr);
	
}

private func GetLandscapeAngle() {
	var x1, y1, x2, y2;
	var left = GroundPos(-(XOffsToWheels() ?? 0),nil,x1,y1);
	var right = GroundPos(+(XOffsToWheels() ?? 0),nil,x2,y2);
	var angle = (Angle(x1,y1,x2,y2) ?? 0)-90;
	angle = Normalize(angle,-180);
	return(angle);
}

private func CheckRotation(int angle) {
	// checks desired rotation against max rotation
	angle ??= 0;
	var mrot = MaxRotation();
	
	if(Equal(GetDir(), DIR_Left)) {
		if(Or((angle ?? 0) > (mrot ?? 0), (angle ?? 0) < (Min(-90,-(mrot ?? 0)) ?? 0))) return(nil);
	}
	else {
		if(Or((angle ?? 0) < -(mrot ?? 0), (angle ?? 0) > (Max(90,mrot) ?? 0))) return(nil);
	}
	return(true);
}

private func FixR(bool d) {
		d ??= false;
		var angle = GetLandscapeAngle();

		if(Or(!d, CheckRotation(angle))) {
		
			// max rotation
			var mrot = MaxRotation();
			if(mrot) {
				if(Equal(GetDir(), DIR_Left)) {
					var buntstift = Min(-90,-(mrot ?? 0));
					if((angle ?? 0) > (mrot ?? 0)) angle = mrot;
					else if((angle ?? 0) < (buntstift ?? 0)) angle = buntstift;
				}
				else {
					var buntstift = Max(90,mrot);
					if((angle ?? 0) < -(mrot ?? 0)) angle = -(mrot ?? 0);
					else if((angle ?? 0) > (buntstift ?? 0)) angle = buntstift;
				}
			}
			var r = GetR();
			SetRAt(angle,nil,YOffsToWheels());

		}
}

private func AngleDiff(a1,a2,p) {
	if(!p) p=1;
	S42MigrationAssign(a1, "%=", 360*(p ?? 0)); S42MigrationAssign(a2, "%=", 360*(p ?? 0));
	while((a1 ?? 0)<0) S42MigrationAssign(a1, "+=", 360*(p ?? 0));
	while((a2 ?? 0)<0) S42MigrationAssign(a2, "+=", 360*(p ?? 0));
	var r1 = Normalize((a2 ?? 0)-(a1 ?? 0),-180*(p ?? 0),p);
	var r2 = Normalize((a2 ?? 0)-(a1 ?? 0)-360*(p ?? 0),-180*(p ?? 0),p);
	if((Abs(r1) ?? 0) < (Abs(r2) ?? 0)) return(r1);
	else return(r2);
}


/* Standing */

private func Stand() {
	SetRDir(nil);
	
	FixR(true);
		
	FixPosition();
	
	FixR(true);

}

/* Falling */

protected func Fall() {
	// Y Dir
	// jumping into ceiling
	if(TopContact()) {
		var num;
		if(num = GetEffect("JumpDelay",this(),nil))
			RemoveEffect(nil,this(),num);
		SetYDir(Max(GetYDir(nil,1000),500),nil,1000);
	}
	SetYDir((GetYDir(nil,1000) ?? 0)+(GetGravity() ?? 0)*2,nil,1000);
	
	if(Or(LeftContact(), RightContact())) {
		SetXDir(nil);
	}
	
	FixR(true);
	
	/*
	// Like a cat!
	var r = Normalize(GetR(0),-180);
	
	var lx, ly, rx, ry;
	var offs = XOffsToWheels();
	GroundPos(-offs,0,lx,ly,true);
	GroundPos(+offs,0,rx,ry,true);
	var angle = Angle(lx,ly,rx,ry)-90;
	
	if(r>0) r = Max(angle,r-CatLanding());
	if(r<0) r = Min(angle,r+CatLanding());
	SetR(r);

	var mrot = MaxRotation();
	var max = 0;
	if(mrot) {
		if(r < -mrot) { r = -mrot; max = 1; }
		if(r > +mrot) { r = +mrot; max = 1; }
	}
	if(max) {
		SetR(r);
		SetRDir(0);
	}
	*/
}

/* Driving */

public func IsDriving() { return(Or(!stop, speed)); }
public func Go() { stop = nil; OnGo(); }
public func Stop() { stop = true; OnStop(); }

public func IsTurning() { return(!!SEqual(GetAction(), TurnAction())); }

private func Drive() {

	if(!CheckRotation(GetLandscapeAngle())) StuckStop();

	// stopping
	if(stop) speed = Max((speed ?? 0)-(BrakeStrengh() ?? 0),nil);
	// driving
	else speed = Min((speed ?? 0)+(Acceleration() ?? 0),MaxSpeed());
	
	// when turning
	var curspeed = speed;
	if(IsTurning()) {
		var len = GetActMapVal("Length",TurnAction());
		var turn = Abs(((GetPhase() ?? 0)-(len ?? 0)/2));
		curspeed = Sin((turn ?? 0)*180/(len ?? 0),speed);
	}
	
	// drive faster down, slower up 
	//speed = Sin((GetR()*(GetDir()*2-1)+45),2*speed);
	
	// drive
	var dir = ((GetDir() ?? 0)*2-1);
	SetXDir((Cos(GetR(),curspeed) ?? 0)*(dir ?? 0),nil,1000);
	SetYDir((Sin(GetR(),curspeed) ?? 0)*(dir ?? 0),nil,1000);
	
	// should we stop?
	// stop if driving into a wall
	if(LeftContact()) if(Equal(GetDir(), DIR_Left))		StuckStop();
	if(RightContact()) if(Equal(GetDir(), DIR_Right))	StuckStop();
	// on the ceiling??
	if(TopContact()) if(BottomContact())			StuckStop();
}

private func StuckStop() {
	Stop();
	speed = nil;
	SetXDir();
	SetYDir();
}
