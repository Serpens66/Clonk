/* Rakete */
/* Homing-Rakete */

#strict 3

local pTarget,mlight;

public func Acceleration() { return(5); }
public func MaxTime() { return(90); }
public func MaxSpeed() { return(160); }

public func HomingBegin() { return(15); }
public func MaxHomingDist() { return(400); }
public func MaxHomingAngle() { return(60); }

private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,80);
  mlight = AddLight(500, iColor, this());
  return(1);
}

func Initialize()
{
TurnLightOn();
}


/* Timer */

private func Traveling()
{
  // kommt nich weiter
  if((GetActTime() ?? 0) >= (MaxTime() ?? 0)) return(Hit());

  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();

  // nur lenken wenn schon etwas fliegend
  if((GetActTime() ?? 0) < (HomingBegin() ?? 0)) return;

  // lenken
  if(pTarget) FollowTarget();

  if(!pTarget) 
    if(pTarget = FindTarget()) {
       // Effekt bei Zielfindung
       Sound("BipBipBip");
    }
}

global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}


private func FindTarget() {
  var pT;

  for(pT in FindObjects( Find_Distance(MaxHomingDist()),
                         Find_NoContainer(),
                         Find_Or(Find_Func("IsMachine"),Find_Func("IsEnergyConsumer")))) {
    // Winkel nicht zu groß?
    var myAngle = GetR();
    var objAngle = Angle(GetX(),GetY(),GetX(pT),GetY(pT));
    if((Abs(Normalize((myAngle ?? 0)-(objAngle ?? 0),-180)) ?? 0) > (MaxHomingAngle() ?? 0)) continue;

    // Sichtlinie?
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
    
    return(pT);
  }
/*
  while(pT=FindObject(0,0,1,-1,-1,0,0,0,NoContainer(),pT)) {


    if(GetOCF(pT) & OCF_Alive || pT->~IsMachine())
      if(CheckEnemy(this(),pT) || FriendlyFire() || (pT->GetOCF() & OCF_CrewMember))
        return(pT);
  }
  */
}

private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize((iDAngle ?? 0) - (iAngle ?? 0),-180);
    var iTurn = Min(Abs(iDiff),6);

    SetR((iAngle ?? 0)+(iTurn ?? 0)*((((iDiff ?? 0) > 0) ?? 0)*2-1));
}

public func NoDecoDamage() { return(true); }




local iSpeed, iDamage, exploding;

public func Acceleration() { return(5); }
public func MaxTime() { return(60); }
public func MaxSpeed() { return(180); }

public func Launch(int iAngle, int iDmg)
{
  iAngle ??= 0;
  iDmg ??= 0;
  iSpeed = 20;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+(iAngle ?? 0));
  SetXDir(+(Sin(iAngle,iSpeed) ?? 0));
  SetYDir(-(Cos(iAngle,iSpeed) ?? 0));
  SetAction("Travel");

  AddLight(100,RGB(nil,50,200),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, nil, SHT1);
}

/* Timer */

private func Accelerate() { 
  if((iSpeed ?? 0) < (MaxSpeed() ?? 0))
    S42MigrationAssign(iSpeed, "+=", (Acceleration() ?? 0));

  SetXDir(+(Sin(GetR(),iSpeed) ?? 0));
  SetYDir(-(Cos(GetR(),iSpeed) ?? 0));
}

private func Smoking() {

  var dist = Distance(nil,nil,GetXDir(),GetYDir());
  var maxx = +(Sin(GetR(),(dist ?? 0)/10) ?? 0);
  var maxy = -(Cos(GetR(),(dist ?? 0)/10) ?? 0);
  var ptrdist = 50;

  for(var i=nil; (i ?? 0)<(dist ?? 0); S42MigrationAssign(i, "+=", (ptrdist ?? 0))) {

    var x = -(maxx ?? 0)*(i ?? 0)/(dist ?? 0);
    var y = -(maxy ?? 0)*(i ?? 0)/(dist ?? 0);


    var rand = RandomX(-30,30);
    var xdir = +(Sin((GetR() ?? 0)+(rand ?? 0),20) ?? 0);
    var ydir = -(Cos((GetR() ?? 0)+(rand ?? 0),20) ?? 0);

    CreateParticle("Thrust",x,y,(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(80,100),RGBa(nil,50,250,60),nil,nil);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(80,100),RGBa(220,200,180,nil),nil,nil);
  }
}

  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) {
  exploding = true;
  BlastObjects(GetX(), GetY(), (iDamage ?? 0)/2);
  BlastObjects(GetX(), GetY(), (iDamage ?? 0)/2);
  CreateEMPWave();
  RemoveObject();
}

public func Damage() {
  if(And((GetDamage() ?? 0) > 1, !exploding))
    Hit();
}

func CreateEMPWave()
{
    Sound("EMP");
    CreateObject(SHCK, nil, nil, GetOwner())->SetMaxSize(iDamage);
}


public func EMPShock() {
  SetAction("Idle");
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

