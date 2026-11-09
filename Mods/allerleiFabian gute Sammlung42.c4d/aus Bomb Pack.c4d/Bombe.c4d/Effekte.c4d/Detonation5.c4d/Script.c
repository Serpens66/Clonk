#strict 3

func Launch(...) {
  SetClrModulation(RGBa(255,255,255,80));
  DoCon(Sub(Div(Mul(Par(nil),100),64),100));
  SetLocal(nil,Par(nil));
  SetVar(nil,Mul(Div(Sub(64,Div(Mul(64,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(nil)));
  SetVar(1,FindObject(nil,Mul(-1,Sum(Div(Local(nil),2),5)),Mul(-1,Sum(Div(Local(nil),2),5)),Sum(Local(nil),10),Sum(Local(nil),10),OCF_Living));
  SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  while(SetVar(1,FindObject(nil,Mul(-1,Sum(Div(Local(nil),2),5)),Mul(-1,Sum(Div(Local(nil),2),5)),Sum(Local(nil),10),Sum(Local(nil),10),OCF_Living,nil,nil,nil,Var(1)))) SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  BlastObjects(GetX(),GetY(),Div(Local(nil),3));
  DigFree(GetX(),GetY(),Div(Local(nil),3));
  Sound("explo*");
  return(1);
}
func Initialize(...) {
  SetAction("Blast");
  SetLocal(nil,64);
  return(1);  
}
func Remove(...) {
  RemoveObject();
  return(1);
}