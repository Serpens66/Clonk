/*-- Geschütz --*/

#strict

public func ReadyToFire()   { return(true); }

local vessel, r0, rmin, rmax;

func Initialize()
{
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
}

//Steuerung
func Up()
{
  return(SetRDir(0));
}

func Down()
{
  return(SetRDir(0));
}

func Right()
{
  if(GetR()==rmax-1) return(1);
  return(SetRDir(5));
}

func Left()
{
  if(GetR()==rmin+1) return(1);
  return(SetRDir(-5));
}

func Fire()
{
  var x,y;
  x=Sin( GetR(),30);
  y=-Cos( GetR(),30);
  var gren=CreateObject(ARM_,x,y,GetOwner());
  var rand = RandomX(-10,10);
  SetXDir(x*8,gren);
  SetYDir(y*8+rand,gren);
  Sound("Rocket");
  Sound("RocketLoad");
  SetAction("Shoot");
  return(1);
}








