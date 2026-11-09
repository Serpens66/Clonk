/* Spore */


#strict

local pTarget,iSpeed, iDamage, exploding, victim, hunger;

public func IsStippel()     { return(true); }
public func Acceleration() { return(5); }
public func MaxTime() { return(90); }
public func MaxSpeed() { return(160); }

public func HomingBegin() { return(15); }
public func MaxHomingDist() { return(400); }
public func MaxHomingAngle() { return(60); }

func Initialize()  {
 hunger=6+RandomX(-3,+3);
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
	   Sound("Sporelock*");
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
                         Find_And(
                           Find_OCF(OCF_Alive),
                           Find_Func("CheckEnemy",this())))) {
    // Winkel nicht zu groß?
    var myAngle = GetR();
    var objAngle = Angle(GetX(),GetY(),GetX(pT),GetY(pT));
    if(Abs(Normalize(myAngle-objAngle,-180)) > MaxHomingAngle()) continue;

    // Sichtlinie?
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
	
	if(IdentifyStippel(GetID(pT))) continue;
    
    return(pT);
  }
}

private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize(iDAngle - iAngle,-180);
    var iTurn = Min(Abs(iDiff),6);

    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}

public func NoDecoDamage() { return(true); }
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
  CastParticles ("StippelFetzen",5,80,0,0,10,10); 
  CreateParticle("Thrust",Cos( GetR() + 270 , 2),Sin( GetR() + 270, 2),GetXDir()/2,GetYDir()/2,RandomX(70,90),RGBa(50,50,250,60),0,0);
  
}

  
/* Treffer */

private func Hit() {
  CastParticles ("StippelFetzen",20,80,0,0,10,10); 
  Sound("Sporehit");
  RemoveObject();
  return(1);
}

private func HitObject(object pObject) {
  if(victim) return(1);
  if((GetID(pObject)==HZCK && !Contained(pObject)))
  {
  victim=pObject;
  ObjectSetAction(this(),"Attaching",pObject);
  Sound("Sporehit");
  return(1);
  }
  if((GetID(pObject)==HZCK && Contained(pObject)))   DoDmg(10,DMG_Melee,Contained(pObject));
  exploding = true;
  Hit();  
}



public func Damage() {
  if(GetDamage() > 1 && !exploding)
    Hit();
}

func Eat()
{
  if(GetName(0,ST5B) && !GetEnergy(victim)) ObjectCall(CreateObject(ST5B,0,0,-1),"Kokon",1); 
  if(hunger<=0 || !GetEnergy(victim)) Hit();
  hunger--;
  DoDmg(2,DMG_Melee,victim);
  Sound("SPStechen");
  return(1);
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

