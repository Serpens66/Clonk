#strict 3

func Hit(...) {
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())) )  Explode(15);
  else FakeExplode(15,nil,nil,15);
  return(1);
}
