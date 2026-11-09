/*--- Signal ---*/

#strict

local iEnergy;
local iHitAmount;



/* Initialisierung */

public func ControlDigDouble(object pClonk)
{
  Sound("SignalClick");
  if(GetAction() eq "Idle") {
    if(!CheckIfUnderSky()) {
      Message("It must be under clear sky!", this);
      Sound("Error");
      return();
    }
    Message("The rod is now on!", this);
    SetAction("Startup");
    iEnergy = 100;
  } else {
    Message("The rod is now off!", this);
    SetAction("Idle");
  }
}

public func Incineration() {
  LightningExplode();
}

public func CheckIfUnderSky() {
  for(var spop = 0; spop > 0-GetY();spop--) {
    if (GetMaterial(0,spop) == -1) return(1);
    if (GetMaterial(0,spop) != 18) return(0);
  }
  return(0);
}

public func LightningStrike() 
{
  iEnergy += RandomX(10,20);
  iHitAmount++;
  if(iHitAmount > 50) if(!Random(3)) Explode(9);
  if(GetDamage()) DoDamage(-GetDamage(),this()); // bei Blitzeinschlägen reparieren, damits Blitze ihn nicht zerstören.
  return(1);
}
  
protected func LaunchL() {
  if(!CheckIfUnderSky()) {
    Message("It must be under clear sky!", this);
    Sound("Error");
    SetAction("Idle");
    return();
  }
  if (!Random(5)) LaunchLightning(GetX(), -10, -10, 20, +5, 15);
  iEnergy -= RandomX(3,7);
  if(iEnergy > 100) iEnergy = 100;
}

protected func LightningExplode() {
  Explode(20);
  LaunchLightning(GetX(), GetY(), -10, 20, -20, 40);
}
