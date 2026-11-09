/* Rakete */
/* Homing-Rakete */

#strict

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
  if(GetActTime() >= MaxTime()) return(Hit());

  // Effekte
  Smoking();

  // beschleunigen
  Accelerate();

  // nur lenken wenn schon etwas fliegend
  if(GetActTime() < HomingBegin()) return();

  // lenken
  if(pTarget) FollowTarget();

  if(!pTarget) 
    if(pTarget = FindTarget()) {
       // Effekt bei Zielfindung
       Sound("BipBipBip");
    }
}

global func Normalize(int iAngle, int iStart) {
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

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
    if(Abs(Normalize(myAngle-objAngle,-180)) > MaxHomingAngle()) continue;

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
    
    var iDiff = Normalize(iDAngle - iAngle,-180);
    var iTurn = Min(Abs(iDiff),6);

    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}

public func NoDecoDamage() { return(true); }




local iSpeed, iDamage, exploding;

public func Acceleration() { return(5); }
public func MaxTime() { return(60); }
public func MaxSpeed() { return(180); }

public func Launch(int iAngle, int iDmg)
{
  iSpeed = 20;
  iDamage = iDmg;
  if(!iDamage) iDamage = 35;

  SetR(+iAngle);
  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetAction("Travel");

  AddLight(100,RGB(0,50,200),this(),GLOW);

  AddEffect("HitCheck", this(), 1,1, 0, SHT1);
}

/* Timer */

private func Accelerate() { 
  if(iSpeed < MaxSpeed())
    iSpeed += Acceleration();

  SetXDir(+Sin(GetR(),iSpeed));
  SetYDir(-Cos(GetR(),iSpeed));
}

private func Smoking() {

  var dist = Distance(0,0,GetXDir(),GetYDir());
  var maxx = +Sin(GetR(),dist/10);
  var maxy = -Cos(GetR(),dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist) {

    var x = -maxx*i/dist;
    var y = -maxy*i/dist;


    var rand = RandomX(-30,30);
    var xdir = +Sin(GetR()+rand,20);
    var ydir = -Cos(GetR()+rand,20);

    CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(0,50,250,60),0,0);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(80,100),RGBa(220,200,180,0),0,0);
  }
}

  
/* Treffer */

private func Hit() {
  HitObject();
}

private func HitObject(object pObject) {
  exploding = true;
  BlastObjects(GetX(), GetY(), iDamage/2);
  BlastObjects(GetX(), GetY(), iDamage/2);
  CreateEMPWave();
  RemoveObject();
}

public func Damage() {
  if(GetDamage() > 1 && !exploding)
    Hit();
}

func CreateEMPWave()
{
    Sound("EMP");
    CreateObject(SHCK, 0, 0, GetOwner())->SetMaxSize(iDamage);
}


public func EMPShock() {
  SetAction("Idle");
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

