/*-- Raketenwerfer --*/

#strict 3

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local target, vessel, firing, r0 , rmin, rmax, timer, rock, maxdist;
  firing=nil;
  target=nil;
  rock=3;
  maxdist=900;
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}

func Rotinit(_r0,_rmin,_rmax)
{
  SetR(_r0);
  rmin=_rmin;
  rmax=_rmax;
  return(1);
}

func Explosion()
{
  RemoveObject();
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

func Target(_target)
{
  target=_target;
}

func Loose()
{
  target=nil;
}

func Check()
{
  if((GetR() ?? 0)<(rmin ?? 0))
  {
  SetRDir(nil);
  SetR((rmin ?? 0)+1);
  }
  if((GetR() ?? 0)>(rmax ?? 0))
  {
  SetRDir(nil);
  SetR((rmax ?? 0)-1);
  }
  if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) Loose();
  if(SEqual(GetAction(target),"Dead")) return(Loose());
  if(SEqual(GetAction(vessel),"Wrecked")) return(Loose());
  if((ObjectDistance(this(),target) ?? 0)>(maxdist ?? 0)) 		Loose();
  if(And(!(Equal(GetOwner(this()), -1)), (And(!Hostile(GetOwner(this()),GetOwner(target)), !(Equal(GetOwner(target), -1)))))) 	Loose();
  if(!target) return(1);
  //Eindrehen
  FollowTarget();
}

private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(target),GetY(target));
    var iAngle = GetR();
    
    var iDiff = Normalize((iDAngle ?? 0) - (iAngle ?? 0),-180);
    var iTurn = Min(Abs(iDiff),1);
	
    SetR((iAngle ?? 0)+(iTurn ?? 0)*((((iDiff ?? 0) > 0) ?? 0)*2-1));
	if(And(!rock, (timer ?? 0)<300))
	{
	S42MigrationStep(timer, 1, true);
	return(1);
	}
	if(!rock) rock=3;
	if(Inside(iDiff,-20,20))	Fire();
}


func Fire()
{
  S42MigrationStep(timer, 1, true);
  if(!rock)
  {
  if((timer ?? 0)<100) return(1);
  rock=3;
  }
  if((timer ?? 0)<20) return(1);
  timer=nil;
  S42MigrationStep(rock, -1, true);
  firing=(!firing);

  var user = this();
  var x,y;
  x=Cos( (GetR() ?? 0) + 270,9);
  y=Sin( (GetR() ?? 0) + 270,9);
  // not too near to clonk
  x = (x ?? 0)*3/2;
  y = (y ?? 0)*3/2;
  var angle = GetR();
  var rocket = CreateObject(HMIS,x,y,GetOwner(user));
  rocket->Launch(angle, 20);
  // Effekte
  var ax, ay, xdir, ydir;
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

global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}












