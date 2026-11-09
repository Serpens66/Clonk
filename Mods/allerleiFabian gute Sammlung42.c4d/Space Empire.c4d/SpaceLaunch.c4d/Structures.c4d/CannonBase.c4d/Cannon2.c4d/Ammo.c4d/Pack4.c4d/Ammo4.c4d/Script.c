/*-- Derad-Munition --*/

#strict 3
#include SOAM

local timer;

/* Wird abgefeuert */

public func Launch(...) {
  SetAction("Travel");
  SetPlrView(GetOwner(),this());
  combi = Par(nil);
  timer = (combi ?? 0)*650;
  return(1);
}

/* Fliegt */

protected func Travel(...) {
  // Gas ausströmen lassen
  if (Random(2)) CreateObject(SDER);
  SetDirection();
  // Timer runterzählen
  if (S42MigrationStep(timer, -1)) return(1);
  Explode(1);
  return(1);
}

/* Zerstörung */

public func Destroy(...) {
  return(timer = 1);
}

public func IsProjectile(...) { return(1);
}

