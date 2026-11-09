/*-- Nuklear-Munition --*/

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
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", nil,GetX(),GetY(), 50+50*(combi ?? 0),10+10*(combi ?? 0)); // kleinen EMP Effekt hinzugefügt
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(30);
  else FakeExplode(30,nil,nil,30);
  return(1);
}

/* Zerstörung */

public func Destroy(...) {
  CastPXS("FlyAshes",25,25);
  return(Hit());
}

public func IsProjectile(...) { return(1);
}

