#strict 3

func Hit(...) {
  if(Not(ActIdle())) return(Call("Exploding"));
  Sound("Boing");
  SetYDir(-50);
  SetXDir(nil);
  SetAction("JumpingUp");
  return(1);
}
func Exploding(...) {
  Sound("Blast2");
  CastObjects(SP02,20,50);
  AssignRemoval();
  return(1);
}
