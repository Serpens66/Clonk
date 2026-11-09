/*-- Derad-Munition --*/

#strict
#include SOAM

local timer;

/* Wird abgefeuert */

public Launch:  
  SetAction("Travel");
  SetPlrView(GetOwner(),this());
  combi = Par(0);
  timer = combi*650;
  return(1);

/* Fliegt */

protected Travel:
  // Gas ausströmen lassen
  if (Random(2)) CreateObject(SDER);
  SetDirection();
  // Timer runterzählen
  if (--timer) return(1);
  Explode(1);
  return(1);

/* Zerstörung */

public Destroy:
  return(timer = 1);

public IsProjectile: return(1);

