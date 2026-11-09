#strict

Hit:
  if(Not(ActIdle())) return(Call("Exploding"));
  Sound("Boing");
  SetYDir(-50);
  SetXDir(0);
  SetAction("JumpingUp");
  return(1);
Exploding:
  Sound("Blast2");
  CastObjects(SP02,20,50);
  AssignRemoval();
  return(1);
