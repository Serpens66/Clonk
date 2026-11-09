#strict 3

local w, l, r, c, iTime, pDmgCallback, pAttach, pLight, iMaxDist, dx, dy;
local reflections;

protected func Initialize()
{
  SetObjectBlitMode(1);
  SetClrModulation(RGB(255,nil,nil));
}

/* Interface (public functions) */

public func SetAngle(int iAngle) {
   iAngle ??= 0;
   r = (iAngle ?? 0)-180;
}
public func SetWidth(int iWidth) {
   iWidth ??= 0;
   w = 1000*(iWidth ?? 0)/(GetActMapVal("Facet","Laser",nil,2) ?? 0);
}
public func SetMaxDistance(int iDist) {
   iDist ??= 0;
   iMaxDist = iDist;
}
public func SetContinuous(int iDelay) {
  iDelay ??= 0;
  if(!iDelay) iDelay = 1;
  AddEffect("Continuous", this(), 1,iDelay, this());
}

public func Set(int iAngle, int iWidth, int iDist, int iT, object pDmg, object pAtt, int iReflect)
{
  // optional parameters
  iAngle ??= 0;
  iWidth ??= 0;
  iDist ??= 0;
  iT ??= 0;
  iReflect ??= 0;
  if(!iWidth) iWidth = 3;
  if(!iDist) iDist = 300;
  // iT = 0: no out-fading
  iTime = iT;
	reflections = iReflect; // how often the Ray should be reflected

  // reset fade-timer
  c=nil;
  // new attach and damage callback objects (optional)
  pDmgCallback = pDmg;
  pAttach = pAtt;
  if(pAttach) {
    dx = (GetX() ?? 0)-(GetX(pAttach) ?? 0);
    dy = (GetY() ?? 0)-(GetY(pAttach) ?? 0);
  }

  // create light
  pLight = CreateObject(LALI,nil,nil,-1);

  SetMaxDistance(iDist);
  SetAngle(iAngle);
  SetWidth(iWidth);
  HitCheck();
  DrawTransform();
  
 	// we are stuck?
	if(Stuck())
		RemoveObject();
}

public func GetAngle()  { return((r ?? 0)+180); }
public func GetWidth()  { return((w ?? 0)*(GetActMapVal("Facet","Laser",nil,2) ?? 0)/1000);  }
public func GetLength() { 
  if(!!SEqual(GetAction(), "Idle"))
    return((l ?? 0)*(GetActMapVal("Facet","Laser",nil,3) ?? 0)/1000);
  else
    return((l ?? 0)*(GetActMapVal("Facet",GetAction(),nil,3) ?? 0)/1000);
}

public func GetMaxDistance() { return(iMaxDist); }

public func LaserEnd(&x, &y) {
  x = +(Sin(GetAngle(),GetLength()) ?? 0);
  y = -(Cos(GetAngle(),GetLength()) ?? 0);
}

/* intern */

private func HitCheck() {

  // calculate length
  var mx=-(Sin(r,iMaxDist) ?? 0),
      my=+(Cos(r,iMaxDist) ?? 0),
      ml=nil;
  while(S42MigrationStep(ml, 1)) {
    if(GBackSolid((mx ?? 0)*(ml ?? 0)/(iMaxDist ?? 0),(my ?? 0)*(ml ?? 0)/(iMaxDist ?? 0))) break;
    if((ml ?? 0)>=(iMaxDist ?? 0)) break;
  }
  // graphic
  SetLaser(Equal(ml, iMaxDist));

  l = 1000*(ml ?? 0)/(GetActMapVal("Facet",GetAction(),nil,3) ?? 0);
  LaserEnd(mx,my); 

  // no hit check
  if(!pDmgCallback)
	return;

  // clonk = user of the weapon that fires (the shooter). Will be given with clonk
  var clonk;
  clonk = pDmgCallback->~GetUser();
  if(!clonk) clonk = pDmgCallback;
	
	var temp_l, hitobject;
	
  // search points on the line for clonks etc.
  // step for step
  for(pObj in FindObjects(Find_OnLine(nil, nil, mx, my), Sort_Distance(nil,nil),
              Find_Exclude(this()),
              Find_Exclude(pAttach),
              Find_Exclude(pDmgCallback),
              Find_Exclude(Contained(pDmgCallback)),
              Find_NoContainer(),
              Find_Or(
              	Find_Func("IsBulletTarget",GetID(),this(),clonk),
              	Find_OCF(OCF_Alive)),
              Find_Func("CheckEnemy",this())))
  {
  	// we have to save this before, if the object is removed on hit
        temp_l = 1000*(ObjectDistance(pObj) ?? 0)/(GetActMapVal("Facet",GetAction(),nil,3) ?? 0);;
  	if(pDmgCallback ->~ LaserStrike(pObj,c))
  	{
                SetLaser(nil);
  		l = temp_l;
  		hitobject = true;
  		break;
  	}
  }
  if(And(And(And(!Equal(ml, iMaxDist), reflections), !hitobject), !Stuck()))
		CheckGround(ml);
}

private func NotZero(int a) {
  a ??= 0;
  if(!a) return(1);
  else return(a);
}

private func SetLaser(bool end) {
  end ??= false;
  if(end) {
    if(!SEqual(GetAction(), "LaserEnd")) {
      SetAction("LaserEnd");
    }
  }
  else {
    if(!SEqual(GetAction(), "Laser")) {
      SetAction("Laser");
    }
  }
}

protected func Laser() {		// fade out
  // attach to obj
  if(pAttach) {
		var wx,wy;
		pAttach->~WeaponEnd(wx,wy);
		if(Or(wx, wy))
			SetPosition((GetX(pAttach) ?? 0) + (wx ?? 0), (GetY(pAttach) ?? 0) + (wy ?? 0));
		else
	    SetPosition((GetX(pAttach) ?? 0)+(dx ?? 0),(GetY(pAttach) ?? 0)+(dy ?? 0));
	   
		SetPosition(GetX(),GetY(),pLight);
  }

  // animation & effect
  SetPhase(Random(4));
  if(And(!!SEqual(GetAction(), "Laser"), !reflections)) {
    var x,y;
    LaserEnd(x,y);
    var color = GetClrModulation();
    LaserLight(20+(GetWidth() ?? 0)*2,color,x,y);
    var a,r,g,b;
    SplitRGBaValue(color,r,g,b,a);
    a = BoundBy((a ?? 0)+64,180,255);
    CreateParticle("PSpark",x,y,nil,nil,300+(GetWidth() ?? 0)*30,RGBa(r,g,b,a));
  }

  if(!iTime) return;

  // fade out (only if time != 0)
  var a = 200*(c ?? 0)/(iTime ?? 0);
  var rgba = SetRGBaValue(GetClrModulation(),a,nil);
  SetClrModulation(rgba);
  S42MigrationStep(c, 1);

  if((c ?? 0) >= (iTime ?? 0))
    RemoveObject();
}

protected func Destruction() {
  if(pLight) RemoveObject(pLight);
}


private func DrawTransform() {		// draw line

  var cl = l;
  cl = (l ?? 0)*(GetActMapVal("Facet","Laser",nil,3) ?? 0)/(GetActMapVal("Facet",GetAction(),nil,3) ?? 0);

  var fsin = -(Sin(r, 1000) ?? 0), fcos = Cos(r, 1000);

  var xoff = -((GetActMapVal("Facet",GetAction(),nil,2) ?? 0)*(w ?? 0)/1000)/2;
  var yoff = nil;

  var width = (+(fcos ?? 0) ?? 0)*(w ?? 0)/1000, height = (+(fcos ?? 0) ?? 0)*(cl ?? 0)/1000;
  var xskew = (+(fsin ?? 0) ?? 0)*(cl ?? 0)/1000, yskew = -(fsin ?? 0)*(w ?? 0)/1000;

  var xadjust = (+(fcos ?? 0) ?? 0)*(xoff ?? 0) + (fsin ?? 0)*(yoff ?? 0);
  var yadjust = -(fsin ?? 0)*(xoff ?? 0) + (fcos ?? 0)*(yoff ?? 0);

  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust
  );
  
  // light
  if(pLight) {
    var alphamod, sizemod;
    CalcLight(alphamod, sizemod);

    var light_width = (w ?? 0)*(5+(sizemod ?? 0)/100);
    var light_height = (cl ?? 0)/2;
    xoff = -((GetActMapVal("Facet","Laser",LALI,2) ?? 0)*(light_width ?? 0)/1000)/2;
    xadjust = (+(fcos ?? 0) ?? 0)*(xoff ?? 0) + (fsin ?? 0)*(yoff ?? 0);
    yadjust = -(fsin ?? 0)*(xoff ?? 0) + (fcos ?? 0)*(yoff ?? 0);
    width =  (+(fcos ?? 0) ?? 0)*(light_width ?? 0)/1000;
    height = (+(fcos ?? 0) ?? 0)*(light_height ?? 0)/1000;
    xskew =  (+(fsin ?? 0) ?? 0)*(light_height ?? 0)/1000;
    yskew =  -(fsin ?? 0)*(light_width ?? 0)/1000;
    
    var a,r_,g,b;
    SplitRGBaValue(GetClrModulation(),r_,g,b,a);
    SetClrModulation(RGBa(r_,g,b,Min((a ?? 0)+160+(alphamod ?? 0),180)),pLight);
    SetPosition(GetX(),GetY(),pLight);
    
    SetObjDrawTransform (
      width, xskew, xadjust,
      yskew, height, yadjust, pLight
    );
  }
}

private func FxContinuousTimer(object target, int effect, int time) {
  // ein bischen Hax: wenn der Laser bleibt, wird auch der Winkel
  // angepasst, falls der Target AimAngle hat (Clonk hat das)
  effect ??= 0;
  time ??= 0;
  var angle;
  if(pAttach) {
    if(angle = (pAttach->~AimAngle(1)))
      SetAngle(angle);
    if(And((Equal(GetDir(pAttach), DIR_Right)), ((dx ?? 0) < 0))) S42MigrationAssign(dx, "*=", -1);
    if(And((Equal(GetDir(pAttach), DIR_Left)), ((dx ?? 0) > 1)))  S42MigrationAssign(dx, "*=", -1);
    }
  HitCheck();
  DrawTransform();
}


// uber-cool reflection script
func CheckGround(int length)
{

	length ??= 0;
	var x,y;
	LaserEnd(x,y);
	
	var r = Normalize(LocalN("r"),nil);
	var angle1, angle2; // Winkel des Materials
	var angle1set, angle2set;
	var rout; //ausgangswinkel
	var i;

	var oldx = x, oldy = y;
	// Adjust position a little bit
	for(i = nil; And(Equal(oldx, x), Equal(oldy, y)); S42MigrationStep(i, 1, true))
	{
		x = (oldx ?? 0)+(Sin(r, i) ?? 0);
		y = (oldy ?? 0)-(Cos(r, i) ?? 0);
	}

	// Search for the angle of the solid material
	for(i = nil; (i ?? 0) < 180; S42MigrationStep(i, 1, true))
	{
		if(And(GBackSolid((x ?? 0)+(Sin((r ?? 0)+(i ?? 0),5) ?? 0), (y ?? 0)-(Cos((r ?? 0)+(i ?? 0),5) ?? 0)), !angle1set))
		{
			angle1set = true;
		}
		if(And(GBackSolid((x ?? 0)+(Sin((r ?? 0)-(i ?? 0),5) ?? 0), (y ?? 0)-(Cos((r ?? 0)-(i ?? 0),5) ?? 0)), !angle2set))
		{
			angle2set = true;
		}	
		if(!angle2set)
		{
			angle1 = (r ?? 0)-(i ?? 0);
		}
		if(!angle1set)
		{
			angle2 = (r ?? 0)+(i ?? 0);
		}
	}

	//nichts gefunden? wir müssen im Material feststecken.
	if(Or(!angle1set, !angle2set))
		return;
	
	var angle = Normalize(Angle(Sin(angle1,5),-(Cos(angle1,5) ?? 0),Sin(angle2,5),-(Cos(angle2,5) ?? 0)),nil);
	
	// calculate the angle of the new laser
	rout = Normalize((angle ?? 0)*2 - ((r ?? 0)+180),nil);

	oldx = x; oldy = y;
	// adjust position
	for(i = nil; GBackSolid(x,y); S42MigrationStep(i, 1, true))
	{
		x = (oldx ?? 0)+(Sin(rout, i) ?? 0);
		y = (oldy ?? 0)-(Cos(rout, i) ?? 0);
	}

	// create new laser
	var newlaser = CreateObject(GetID(), x,y, GetOwner());
	newlaser->SetClrModulation(GetClrModulation());
	newlaser->Set(rout, GetWidth(), (iMaxDist ?? 0)-(length ?? 0), iTime, pDmgCallback, pAttach, (reflections ?? 0)-1);
}
