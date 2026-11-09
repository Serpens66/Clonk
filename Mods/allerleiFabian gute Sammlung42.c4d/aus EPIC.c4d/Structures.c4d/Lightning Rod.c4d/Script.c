/*--- Signal ---*/

#strict 3

local iEnergy;
local iHitAmount;



/* Initialisierung */

public func ControlDigDouble(object pClonk)
{
  Sound("SignalClick");
  if(!!SEqual(GetAction(), "Idle")) {
    if(!CheckIfUnderSky()) {
      Message("It must be under clear sky!", this);
      Sound("Error");
      return;
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
  for(var spop = nil; (spop ?? 0) > 0-(GetY() ?? 0);S42MigrationStep(spop, -1, true)) {
    if (Equal(GetMaterial(nil,spop), -1)) return(1);
    if (!Equal(GetMaterial(nil,spop), 18)) return(nil);
  }
  return(nil);
}

public func LightningStrike() 
{
  S42MigrationAssign(iEnergy, "+=", (RandomX(10,20) ?? 0));
  S42MigrationStep(iHitAmount, 1, true);
  if((iHitAmount ?? 0) > 50) if(!Random(3)) Explode(9);
  if(GetDamage()) DoDamage(-(GetDamage() ?? 0),this()); // bei Blitzeinschlägen reparieren, damits Blitze ihn nicht zerstören.
  return(1);
}
  
protected func LaunchL() {
  if(!CheckIfUnderSky()) {
    Message("It must be under clear sky!", this);
    Sound("Error");
    SetAction("Idle");
    return;
  }
  if (!Random(5)) LaunchLightning(GetX(), -10, -10, 20, +5, 15);
  S42MigrationAssign(iEnergy, "-=", (RandomX(3,7) ?? 0));
  if((iEnergy ?? 0) > 100) iEnergy = 100;
}

protected func LightningExplode() {
  Explode(20);
  LaunchLightning(GetX(), GetY(), -10, 20, -20, 40);
}
