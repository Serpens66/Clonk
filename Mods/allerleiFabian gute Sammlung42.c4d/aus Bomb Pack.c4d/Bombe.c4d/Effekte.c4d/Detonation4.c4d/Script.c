#strict 3

func Launch(...) {
  SetClrModulation(RGBa(255,255,255,80));
  SetOwner(Par(1));
  DoCon(Sub(Div(Mul(Par(nil),100),128),100));
  SetLocal(nil,Par(nil));
  SetVar(nil,Mul(Div(Sub(128,Div(Mul(128,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(nil)));
  SetVar(1,FindObject(nil,Mul(-1,Sum(Div(Local(nil),2),5)),Mul(-1,Sum(Div(Local(nil),2),5)),Sum(Local(nil),10),Sum(Local(nil),10),OCF_Living));
  SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  while(SetVar(1,FindObject(nil,Mul(-1,Sum(Div(Local(nil),2),5)),Mul(-1,Sum(Div(Local(nil),2),5)),Sum(Local(nil),10),Sum(Local(nil),10),OCF_Living,nil,nil,nil,Var(1)))) SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  BlastObjects(GetX(),GetY(),Div(Local(nil),3));
  SetVar(1,FindObject(nil,Mul(-1,Sum(Div(Local(nil),2),5)),Mul(-1,Sum(Div(Local(nil),2),5)),Sum(Local(nil),10),Sum(Local(nil),10),OCF_Living));
  SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  while(SetVar(1,FindObject(nil,Mul(-1,Sum(Div(Local(nil),2),5)),Mul(-1,Sum(Div(Local(nil),2),5)),Sum(Local(nil),10),Sum(Local(nil),10),OCF_Living,nil,nil,nil,Var(1)))) SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  BlastObjects(GetX(),GetY(),Div(Local(nil),3));
  DigFree(GetX(),GetY(),Div(Local(nil),3));
  Sound("explo*");
  if(Equal(Par(2),1)) return(SetAction("Blast"));
  if(Equal(Par(2),2)) return(SetAction("Blast2"));
  if(Equal(Par(2),3)) return(SetAction("Blast3"));
  SetVar(nil,Random(3));
  if(Equal(Var(nil),nil)) return(SetAction("Blast"));
  if(Equal(Var(nil),1)) return(SetAction("Blast2"));
  if(Equal(Var(nil),2)) return(SetAction("Blast3"));
  return(1);
}
func Initialize(...) {
  SetLocal(nil,128);
  return(1);  
}
func Remove(...) {
  RemoveObject();
  return(1);
}
