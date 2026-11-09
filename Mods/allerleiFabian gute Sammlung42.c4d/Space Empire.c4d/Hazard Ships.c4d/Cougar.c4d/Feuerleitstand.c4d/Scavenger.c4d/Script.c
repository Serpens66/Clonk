/* Scavenger-Marschflugkörper */

#strict 3

local mlight;

public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }
public func ReadyToFire()   { return(true); }

private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,80);
  mlight = AddLight(500, iColor, this());
  return(1);
}


//Anfangswerte
protected func Initialize()
{
  local speed, heat, timer;
  timer=nil;
  speed=10;
TurnLightOn();
  return(1);
}

func Hit()
{
  Explosion2();
  return(1);
}


func Explosion()
{
  if(heat) RemoveObject(heat);
  BlastObjects(GetX(), GetY(), 25);
  BlastObjects(GetX(), GetY(), 25);
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(50);
  else FakeExplode(50,nil,nil,50);
  return(1);
}

func Explosion2()
{ 
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(50);
  else FakeExplode(50,nil,nil,50);
  return(1);
}

func Damage()
{
  if((GetDamage() ?? 0)<10) return(nil);
  Explosion2();
  return(1);
}


//Flugkontrolle
func Traveling()
{
  SetPlrView(GetOwner(), this());
  if ((timer ?? 0)>285) Explosion();
  S42MigrationStep(timer, 1, true);
  if((speed ?? 0)<90) S42MigrationAssign(speed, "+=", 5);
  SetXDir(Cos(Sum(GetR(),270),speed));
  SetYDir(Sin(Sum(GetR(),270),speed));
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(60,80),RGBa(255,200,200,60),nil,nil);
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(60,80),RGBa(220,200,180,nil),nil,nil);
  return(1);
}


//Steuerung & Schubregelung
func ControlUp()
{
  SetRDir(nil);
  return(1);
}

func ControlDown()
{
  SetRDir(nil);
  return(1);
}

func ControlLeft()
{
  SetRDir(-5);
  return(1);
}

func ControlRight()
{
  SetRDir( 5);
  return(1);
}


func ControlThrow() 
{
  Explosion();
  return(1);
}