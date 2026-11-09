#strict 3

func Launch(...) {
  SetClrModulation(RGBa(255,255,255,80));
  DoCon(Sub(Div(Mul(Par(nil),100),32),100));
  SetLocal(nil,Par(nil));
  SetVar(nil,Mul(Div(Sub(32,Div(Mul(32,GetCon()),100)),2),-1));
  SetPosition(GetX(),Sum(GetY(),Var(nil)));
  Sound("explo*");
  return(1);
}
func Initialize(...) {
  SetAction("Blast");
  SetLocal(nil,32);
  return(1);  
}
func Remove(...) {
  RemoveObject();
  return(1);
}