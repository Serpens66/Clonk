#strict

Launch:
  SetClrModulation(RGBa(255,255,255,80));
  DoCon(Sub(Div(Mul(Par(0),100),32),100));
  SetLocal(0,Par(0));
  SetVar(0,Mul(Div(Sub(32,Div(Mul(32,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(0)));
  Sound("explo*");
  return(1);
Initialize:
  SetAction("Blast");
  SetLocal(0,32);
  return(1);  
Remove:
  RemoveObject();
  return(1);