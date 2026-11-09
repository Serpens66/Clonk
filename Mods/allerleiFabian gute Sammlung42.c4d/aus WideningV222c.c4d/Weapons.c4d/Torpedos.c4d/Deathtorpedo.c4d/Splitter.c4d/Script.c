#strict 3
// MSC-Script //

func Hit(...) {
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(20); else FakeExplode(20,nil,nil,20);
  Sound("HEADCRAS");
  return(1);
}

func Check(...) {
  SetAction("Delay");
  return(1);
}

func Checkit(...) {
  if(SetVar(nil,FindObject(nil,+1,nil,nil,nil,OCF_Living))) if(Not(Contained(Var(nil)))) Hit();
  return(1);
}
