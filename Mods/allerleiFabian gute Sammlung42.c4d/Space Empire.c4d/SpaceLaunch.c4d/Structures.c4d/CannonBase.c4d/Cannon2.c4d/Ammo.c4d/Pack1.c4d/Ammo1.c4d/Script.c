/*-- Explosiv-Munition --*/

#strict
#include SOAM


/* Fliegt */

protected Travel:
  // Treffer-Überprüfung
  if (FindTarget() || FindVehicle())
    return(Hit());
  Smoke(0,0,5);
  SetDirection();
  return(1);

/* Aufprall */

protected Hit:
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(15*combi);  //vorheriger Wert von 31*combi war viel zu stark
  else FakeExplode(15*combi,0,0,15*combi);
  return(1);

/* Zerstörung */

public Destroy:
  CastPXS("FlyAshes",25,25);
  return(Hit());

public IsProjectile: return(1);

