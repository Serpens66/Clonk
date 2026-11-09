/*-- Raketenwerfer --*/

#strict

public func ReadyToFire()   { return(true); }

func Initialize()
{
  local target, vessel, firing, r0 , rmin, rmax, timer, rock, maxdist;
  firing=0;
  target=0;
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
  return(0);
}

func Target(_target)
{
  target=_target;
}

func Loose()
{
  target=0;
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
  if(SEqual(GetAction(target),"Dead")) return(Loose());
  if(SEqual(GetAction(vessel),"Wrecked")) return(Loose());
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
    var iTurn = Min(Abs(iDiff),1);
	
    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
	if(!rock && timer<300)
	{
	timer++;
	return(1);
	}
	if(!rock) rock=3;
	if(Inside(iDiff,-20,20))	Fire();
}


func Fire()
{
  timer++;
  if(!rock)
  {
  if(timer<100) return(1);
  rock=3;
  }
  if(timer<20) return(1);
  timer=0;
  rock--;
  firing=(!firing);

  var user = this();
  var x,y;
  x=Cos( GetR() + 270,9);
  y=Sin( GetR() + 270,9);
  // not too near to clonk
  x = x*3/2;
  y = y*3/2;
  var angle = GetR();
  var rocket = CreateObject(HMIS,x,y,GetOwner(user));
  rocket->Launch(angle, 20);
  // Effekte
  var ax, ay, xdir, ydir;
  xdir = ax-x;
  ydir = ay-y;
  CreateParticle("Thrust",ax,ay,xdir/2,ydir/2,80,RGBa(255,200,200,0),0,0);

  for(var i=0; i<20; ++i) {
    var rand = RandomX(-10,+10);
    CreateParticle("Smoke2",ax+Sin(angle,rand),ay-Cos(angle,rand),
                   RandomX(0,2*xdir),RandomX(0,2*ydir),
                   RandomX(80,140),RGBa(220,200,180,0),0,0);
  }

  // Sound
  Sound("Rocket");  
 }

global func Normalize(int iAngle, int iStart) {
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

  return(iAngle);
}












