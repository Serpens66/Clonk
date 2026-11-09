#strict

Launch:
  SetClrModulation(RGBa(255,255,255,80));
  SetOwner(Par(1));
  DoCon(Sub(Div(Mul(Par(0),100),128),100));
  SetLocal(0,Par(0));
  SetVar(0,Mul(Div(Sub(128,Div(Mul(128,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(0)));
  SetVar(1,FindObject(0,Mul(-1,Sum(Div(Local(0),2),5)),Mul(-1,Sum(Div(Local(0),2),5)),Sum(Local(0),10),Sum(Local(0),10),OCF_Living()));
  SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  while(SetVar(1,FindObject(0,Mul(-1,Sum(Div(Local(0),2),5)),Mul(-1,Sum(Div(Local(0),2),5)),Sum(Local(0),10),Sum(Local(0),10),OCF_Living(),0,0,0,Var(1)))) SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  BlastObjects(GetX(),GetY(),Div(Local(0),3));
  SetVar(1,FindObject(0,Mul(-1,Sum(Div(Local(0),2),5)),Mul(-1,Sum(Div(Local(0),2),5)),Sum(Local(0),10),Sum(Local(0),10),OCF_Living()));
  SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  while(SetVar(1,FindObject(0,Mul(-1,Sum(Div(Local(0),2),5)),Mul(-1,Sum(Div(Local(0),2),5)),Sum(Local(0),10),Sum(Local(0),10),OCF_Living(),0,0,0,Var(1)))) SetComponent(EXPL,Sum(1,GetOwner()),Var(1));
  BlastObjects(GetX(),GetY(),Div(Local(0),3));
  DigFree(GetX(),GetY(),Div(Local(0),3));
  Sound("explo*");
  if(Equal(Par(2),1)) return(SetAction("Blast"));
  if(Equal(Par(2),2)) return(SetAction("Blast2"));
  if(Equal(Par(2),3)) return(SetAction("Blast3"));
  SetVar(0,Random(3));
  if(Equal(Var(0),0)) return(SetAction("Blast"));
  if(Equal(Var(0),1)) return(SetAction("Blast2"));
  if(Equal(Var(0),2)) return(SetAction("Blast3"));
  return(1);
Initialize:
  SetLocal(0,128);
  return(1);  
Remove:
  RemoveObject();
  return(1);
