/*-- Geschütz --*/

#strict

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local target, vessel, firing, r0 , rmin, rmax, timer,maxdist;
  maxdist=600;
  firing=0;
  target=0;
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
  return(0);
}

func Target(_target)
{
  target=_target;
}

func Loose()
{
    target=0;
	if(firing)
	{
    Sound("MiniGun",0,0,0,0,-1);
    Sound("MiniTurn",0,0,0,0,-1);
	firing=0;
    }
}

func Check()
{
  if(GetR()<rmin)
  {
  SetRDir(0);
  SetR(rmin+1);
  }
  if(GetR()>rmax)
  {
  SetRDir(0);
  SetR(rmax-1);
  }
  if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) Loose();
  if(SEqual(GetAction(vessel),"Wrecked")) return(Loose());
  if(SEqual(GetAction(target),"Dead")) return(Loose());
  if(ObjectDistance(this(),target)>maxdist) 		Loose();
  if(!(GetOwner(this())==-1) && (!Hostile(GetOwner(this()),GetOwner(target)) && !(GetOwner(target)==-1))) 	Loose();
  if(!target) return(1);
  //Eindrehen
  FollowTarget();
}

private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(target),GetY(target));
    var iAngle = GetR();
    
    var iDiff = Normalize(iDAngle - iAngle,-180);
    var iTurn = Min(Abs(iDiff),4);
	
    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
	
	if(!Inside(iDiff,-20,20))
	{
	if(firing)
	{
    Sound("MiniGun",0,0,0,0,-1);
    Sound("MiniTurn",0,0,0,0,-1);
	firing=0;
    }
	return(1);
	}
	
	if(Inside(iDiff,-20,20))
	{
	Fire();
    if(!firing)
    {
    Sound("MiniGun",0,0,0,0,1);
    Sound("MiniTurn",0,0,0,0,1);
	firing=1;
    }
	}
}


func Fire()
{
  timer++;
  if(timer<5) return(1);
  timer=0;
  firing=(!firing);
  var user = this();
  // Winkel: maximal 5° anvisieren, Streuung von 12°
  var angle = GetR()+RandomX(-2,+2);
  // Schuss an der Mündung der Waffe erstellen...
  var x,y;
  x=Sin(GetR(),15);
  y=-Cos( GetR(),15);
  var ammo = CreateObject(SHT1,x/2,y/2,GetOwner(user));
  ammo->Launch(angle,300,600,4,600,20);
  // Effekte
  MuzzleFlash(RandomX(20,50),user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(x,y,-dir*Cos(angle-35*dir,40+Random(20)),-dir*Sin(angle-35*dir,40+Random(20)),5);
}

global func Normalize(int iAngle, int iStart) {
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

  return(iAngle);
}












