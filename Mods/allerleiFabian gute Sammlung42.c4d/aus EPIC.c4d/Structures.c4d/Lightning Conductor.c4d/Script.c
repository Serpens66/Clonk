/*--- blitzableiter ---*/

#strict

local iHitAmount;

public func Incineration() {
  Destroy();
}

public func LightningStrike() 
{
  iHitAmount++;
  if(iHitAmount > 100) if(!Random(3)) Destroy();
  if(GetDamage()) DoDamage(-GetDamage(),this()); // bei Blitzeinschlägen reparieren, damits Blitze ihn nicht zerstören.
  return(1);
}

public func Destroy() {
  CastObjects(ROCK,4,30);
  RemoveObject();
}

