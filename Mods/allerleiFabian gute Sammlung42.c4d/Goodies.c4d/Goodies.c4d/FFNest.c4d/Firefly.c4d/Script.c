#strict 3

func FlyProcess(...) {
  if (Not(Random(3)))
    if (AssignVar(nil,FindObject( nil, +1,nil,nil,nil, OCF_Prey )))
      Call("Sting",Var(nil));
  return(1);
}

func Activity(...) {

  if (Random(2)) return(1);

  if (Not( SEqual(GetAction(),"Fly") )) return(1);

  if (Not(Random(25))) Sound("Inflame");

  if (GBackSolid()) return(Call("Death"));

  if (Not(Random(30))) return(Call("Death"));

  if (Not(Random(2))) SetComDir(Sum(Random(9),+1));

  if (Not(Random(3))) SetCommand(this(),"None");

  if (Not(Random(8)))
    if (AssignVar(nil, FindObject(nil, nil,nil,-1,-1, OCF_Prey) ))
      SetCommand(this(),"Follow",Var(nil));

  return(1);
}
  
func Sting(...) {
  Incinerate(Par(nil));
  Sound("Sting");
  return(1);
}

func Completion(...) {
  SetAction("Fly");
  SetComDir(Sum(Random(9),+1));
  return(1);
}

func Death(...) {
  AssignRemoval();
  return(1);
}

