#strict

Hit:
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())) )  Explode(15);
  else FakeExplode(15,0,0,15);
  return(1);
