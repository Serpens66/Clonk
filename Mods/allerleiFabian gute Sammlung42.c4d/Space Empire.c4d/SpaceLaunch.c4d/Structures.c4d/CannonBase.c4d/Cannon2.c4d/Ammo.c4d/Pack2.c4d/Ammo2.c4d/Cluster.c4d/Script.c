/*-- Explosiver Cluster --*/

#strict 3

local size;


/* Explosionsgröße setzen */

public func SetSize(newsize)
{
  size = newsize;
  return(1);
}


/* Aufprall / Zerstörung */

protected func Hit(...) {
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(10); else FakeExplode(10,nil,nil,10);
  return(1);
}

public func Destroy(...) {
  CastPXS("FlyAshes",25,25);
  return(Hit());
}

public func IsProjectile(...) { return(1);
}

