/* Rakete */

#strict 3
#include MISS

local pTarget;

public func Acceleration() { return(4); }
public func MaxTime() { return(70); }
public func MaxSpeed() { return(160); }

public func HomingBegin() { return(15); }
public func MaxHomingDist() { return(400); }
public func MaxHomingAngle() { return(60); }

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
