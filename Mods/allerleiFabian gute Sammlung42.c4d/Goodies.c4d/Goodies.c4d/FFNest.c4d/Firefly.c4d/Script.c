#strict

FlyProcess:
  if (Not(Random(3)))
    if (AssignVar(0,FindObject( 0, +1,0,0,0, OCF_Prey() )))
      Call("Sting",Var(0));
  return(1);

Activity: 

  if (Random(2)) return(1);

  if (Not( SEqual(GetAction(),"Fly") )) return(1);

  if (Not(Random(25))) Sound("Inflame");

  if (GBackSolid()) return(Call("Death"));

  if (Not(Random(30))) return(Call("Death"));

  if (Not(Random(2))) SetComDir(Sum(Random(9),+1));

  if (Not(Random(3))) SetCommand(this(),"None");

  if (Not(Random(8)))
    if (AssignVar(0, FindObject(0, 0,0,-1,-1, OCF_Prey()) ))
      SetCommand(this(),"Follow",Var(0));

  return(1);
  
Sting:
  Incinerate(Par(0));
  Sound("Sting");
  return(1);

Completion:
  SetAction("Fly");
  SetComDir(Sum(Random(9),+1));
  return(1);

Death:
  AssignRemoval();
  return(1);

