/*-- Explosiv-Munition --*/

#strict 3
#include SOAM


/* Fliegt */

protected func Travel(...) {
  // Treffer-Überprüfung
  if (Or(FindTarget(), FindVehicle()))
    return(Hit());
  Smoke(nil,nil,5);
  SetDirection();
  return(1);
}

/* Aufprall */

protected func Hit(...) {
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(15*(combi ?? 0));  //vorheriger Wert von 31*combi war viel zu stark
  else FakeExplode(15*(combi ?? 0),nil,nil,15*(combi ?? 0));
  return(1);
}

/* Zerstörung */

public func Destroy(...) {
  CastPXS("FlyAshes",25,25);
  return(Hit());
}

public func IsProjectile(...) { return(1);
}

