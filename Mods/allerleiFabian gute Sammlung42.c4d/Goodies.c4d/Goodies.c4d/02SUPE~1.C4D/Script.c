#strict

Hit:
  if(Not(ActIdle())) return(Exploding());
  Sound("Boing");
  SetYDir(-100);
  SetXDir(0);
  SetAction("JumpingUp");
  return(1);
Exploding:
  Sound("Blast3");
  CastObjects(SP02,40,50);
  RemoveObject();
  return(1);

/* Produkteigenschaften */

public IsChemicalProduct: return(1);
