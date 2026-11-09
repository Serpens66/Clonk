/* Spore */


#strict 3

local pTarget,iSpeed, iDamage, exploding, victim, hunger;

public func IsStippel()     { return(true); }
public func Acceleration() { return(5); }
public func MaxTime() { return(90); }
public func MaxSpeed() { return(160); }

public func HomingBegin() { return(15); }
public func MaxHomingDist() { return(400); }
public func MaxHomingAngle() { return(60); }

func Initialize()  {
 hunger=6+(RandomX(-3,+3) ?? 0);
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
	   Sound("Sporelock*");
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
                         Find_And(
                           Find_OCF(OCF_Alive),
                           Find_Func("CheckEnemy",this())))) {
    // Winkel nicht zu groß?
    var myAngle = GetR();
    var objAngle = Angle(GetX(),GetY(),GetX(pT),GetY(pT));
    if((Abs(Normalize((myAngle ?? 0)-(objAngle ?? 0),-180)) ?? 0) > (MaxHomingAngle() ?? 0)) continue;

    // Sichtlinie?
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
	
	if(IdentifyStippel(GetID(pT))) continue;
    
    return(pT);
  }
}

private func FollowTarget() {

    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize((iDAngle ?? 0) - (iAngle ?? 0),-180);
    var iTurn = Min(Abs(iDiff),6);

    SetR((iAngle ?? 0)+(iTurn ?? 0)*((((iDiff ?? 0) > 0) ?? 0)*2-1));
}

public func NoDecoDamage() { return(true); }
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
  CastParticles ("StippelFetzen",5,80,nil,nil,10,10);
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , 2),Sin( (GetR() ?? 0) + 270, 2),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(70,90),RGBa(50,50,250,60),nil,nil);
  
}

  
/* Treffer */

private func Hit() {
  CastParticles ("StippelFetzen",20,80,nil,nil,10,10);
  Sound("Sporehit");
  RemoveObject();
  return(1);
}

private func HitObject(object pObject) {
  if(victim) return(1);
  if((And(Equal(GetID(pObject), HZCK), !Contained(pObject))))
  {
  victim=pObject;
  ObjectSetAction(this(),"Attaching",pObject);
  Sound("Sporehit");
  return(1);
  }
  if((And(Equal(GetID(pObject), HZCK), Contained(pObject))))   DoDmg(10,DMG_Melee,Contained(pObject));
  exploding = true;
  Hit();  
}



public func Damage() {
  if(And((GetDamage() ?? 0) > 1, !exploding))
    Hit();
}

func Eat()
{
  if(And(GetName(nil,ST5B), !GetEnergy(victim))) ObjectCall(CreateObject(ST5B,nil,nil,-1),"Kokon",1);
  if(Or((hunger ?? 0)<=0, !GetEnergy(victim))) Hit();
  S42MigrationStep(hunger, -1, true);
  DoDmg(2,DMG_Melee,victim);
  Sound("SPStechen");
  return(1);
}

public func IsBulletTarget() { return(true); }
public func NoDecoDamage() { return(true); }
public func IsMachine() { return(true); }

