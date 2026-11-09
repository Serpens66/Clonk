/*-- Nuklear-Munition --*/

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
  var obj = CreateObject(SWAV);
  ObjectCall(obj, "SetSize", 0,GetX(),GetY(), 50+50*combi,10+10*combi); // kleinen EMP Effekt hinzugefügt
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(30);
  else FakeExplode(30,0,0,30);
  return(1);

/* Zerstörung */

public Destroy:
  CastPXS("FlyAshes",25,25);
  return(Hit());

public IsProjectile: return(1);

