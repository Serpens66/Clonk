/*--- blitzableiter ---*/

#strict 3

local iHitAmount;

public func Incineration() {
  Destroy();
}

public func LightningStrike() 
{
  S42MigrationStep(iHitAmount, 1, true);
  if((iHitAmount ?? 0) > 100) if(!Random(3)) Destroy();
  if(GetDamage()) DoDamage(-(GetDamage() ?? 0),this()); // bei Blitzeinschlägen reparieren, damits Blitze ihn nicht zerstören.
  return(1);
}

public func Destroy() {
  CastObjects(ROCK,4,30);
  RemoveObject();
}

