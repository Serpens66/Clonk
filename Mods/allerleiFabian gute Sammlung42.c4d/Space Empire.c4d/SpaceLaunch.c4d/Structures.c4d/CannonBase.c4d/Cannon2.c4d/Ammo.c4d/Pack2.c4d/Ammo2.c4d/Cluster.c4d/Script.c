/*-- Explosiver Cluster --*/

#strict

local size;


/* Explosionsgröße setzen */

public func SetSize(newsize)
{
  size = newsize;
  return(1);
}


/* Aufprall / Zerstörung */

protected Hit:
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(10); else FakeExplode(10,0,0,10);
  return(1);

public Destroy:
  CastPXS("FlyAshes",25,25);
  return(Hit());

public IsProjectile: return(1);

