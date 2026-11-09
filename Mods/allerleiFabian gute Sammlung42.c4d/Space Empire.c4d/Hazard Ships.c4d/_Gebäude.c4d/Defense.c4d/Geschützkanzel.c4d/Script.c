/*-- Geschütz --*/

#strict 3

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local target, vessel, firing, r0 , rmin, rmax, timer,maxdist;
  maxdist=600;
  firing=nil;
  target=nil;
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
	if(firing)
	{
    Sound("MiniGun",nil,nil,nil,nil,-1);
    Sound("MiniTurn",nil,nil,nil,nil,-1);
	firing=nil;
    }
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
  if(SEqual(GetAction(vessel),"Wrecked")) return(Loose());
  if(SEqual(GetAction(target),"Dead")) return(Loose());
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
    var iTurn = Min(Abs(iDiff),4);
	
    SetR((iAngle ?? 0)+(iTurn ?? 0)*((((iDiff ?? 0) > 0) ?? 0)*2-1));
	
	if(!Inside(iDiff,-20,20))
	{
	if(firing)
	{
    Sound("MiniGun",nil,nil,nil,nil,-1);
    Sound("MiniTurn",nil,nil,nil,nil,-1);
	firing=nil;
    }
	return(1);
	}
	
	if(Inside(iDiff,-20,20))
	{
	Fire();
    if(!firing)
    {
    Sound("MiniGun",nil,nil,nil,nil,1);
    Sound("MiniTurn",nil,nil,nil,nil,1);
	firing=1;
    }
	}
}


func Fire()
{
  S42MigrationStep(timer, 1, true);
  if((timer ?? 0)<5) return(1);
  timer=nil;
  firing=(!firing);
  var user = this();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = (GetR() ?? 0)+(RandomX(-2,+2) ?? 0);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Sin(GetR(),15);
  y=-(Cos( GetR(),15) ?? 0);
  var ammo = CreateObject(SHT1,(x ?? 0)/2,(y ?? 0)/2,GetOwner(user));
  ammo->Launch(angle,300,600,4,600,20);
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  var dir = (GetDir(user) ?? 0)*2-1;
  BulletCasing(x,y,-(dir ?? 0)*(Cos((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),-(dir ?? 0)*(Sin((angle ?? 0)-35*(dir ?? 0),40+(Random(20) ?? 0)) ?? 0),5);
}

global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}












