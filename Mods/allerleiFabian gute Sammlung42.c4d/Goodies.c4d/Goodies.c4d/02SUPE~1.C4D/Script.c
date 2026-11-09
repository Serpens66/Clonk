#strict 3

func Hit(...) {
  if(Not(ActIdle())) return(Exploding());
  Sound("Boing");
  SetYDir(-100);
  SetXDir(nil);
  SetAction("JumpingUp");
  return(1);
}
func Exploding(...) {
  Sound("Blast3");
  CastObjects(SP02,40,50);
  RemoveObject();
  return(1);
}

/* Produkteigenschaften */

public func IsChemicalProduct(...) { return(1);
}
