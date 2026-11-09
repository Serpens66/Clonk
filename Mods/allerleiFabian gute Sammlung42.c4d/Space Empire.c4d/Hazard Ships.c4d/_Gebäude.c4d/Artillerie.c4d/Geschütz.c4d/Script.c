/*-- Geschütz --*/

#strict 3

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
  return(nil);
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
}

//Steuerung
func Up()
{
  return(SetRDir(nil));
}

func Down()
{
  return(SetRDir(nil));
}

func Right()
{
  if(Equal(GetR(), (rmax ?? 0)-1)) return(1);
  return(SetRDir(5));
}

func Left()
{
  if(Equal(GetR(), (rmin ?? 0)+1)) return(1);
  return(SetRDir(-5));
}

func Fire()
{
  var x,y;
  x=Sin( GetR(),30);
  y=-(Cos( GetR(),30) ?? 0);
  var gren=CreateObject(ARM_,x,y,GetOwner());
  var rand = RandomX(-10,10);
  SetXDir((x ?? 0)*8,gren);
  SetYDir((y ?? 0)*8+(rand ?? 0),gren);
  Sound("Rocket");
  Sound("RocketLoad");
  SetAction("Shoot");
  return(1);
}








