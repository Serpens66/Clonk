#strict 3

local openmode,autoclose,closetimer,state;

func Initialize(...) {
  state = nil;
  openmode=1;
  autoclose=1;
  DigFreeRect((GetX() ?? 0)+(HMinX() ?? 0),(GetY() ?? 0)+11,HMaxX(),30);
  PutSolidMask();
  return(1);
}

func CheckArea(...) {
  if (Equal(openmode, 1))
  {
    if (Var(nil)=FindObject(nil,HMinX(),10,HMaxX(),17,OCF_CrewMember))
      ControlUp(Var(nil));
    else
      if (!(Var(nil)=FindObject(nil,HMinX(),-10,HMaxX(),20,OCF_CrewMember)))
        if (Equal(autoclose, 1)) ControlDownSingle(Var(nil));
  }
  else
  {
    if (Var(nil)=FindObject(nil,HMinX(),-10,HMaxX(),37,OCF_CrewMember))
      ControlUp(Var(nil));
    else
      if (!(Var(nil)=FindObject(nil,HMinX(),-10,HMaxX(),20,OCF_CrewMember)))
        if (Equal(autoclose, 1)) ControlDownSingle(Var(nil));
  }
  closetimer=Max(nil,(closetimer ?? 0)-1);
  if (And((Equal(autoclose, 2)), (Equal(closetimer, nil)))) ControlDownSingle();
  return(1);
}
  
func ControlThrow(...) {
  [Set close mode]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  autoclose=(autoclose ?? 0)+1;  if ((autoclose ?? 0)>2) autoclose=nil;
  if (Equal(autoclose, 2)) Message("Timer close", this());
  if (Equal(autoclose, 1)) Message("Auto close", this());
  if (Equal(autoclose, nil)) Message("Manual close", this());
  return(1);
}

func ControlDig(...) {
  [Set open mode]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  openmode=(openmode ?? 0)+1;  if ((openmode ?? 0)>1) openmode=nil;
  if (Equal(openmode, 1)) Message("Auto bottom open", this());
  if (Equal(openmode, nil)) Message("Full auto open", this());
  return(1);
}

func ControlDownSingle(...) {
  [Close]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  if (!SEqual(GetAction(), "HatchOpen")) return(nil);
  if (!SEqual(GetAction(), "CloseHatch")) SetAction("CloseHatch");
  state = nil;
  return(1);
}

func ControlUp(...) {
  [Open]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  if (!SEqual(GetAction(), "Idle")) return(1);
  closetimer=100;
  SetAction("OpenHatch");
  state = 1;
  return(1);
}

func RemoveSolidMask(...) {
  SetSolidMask();
  Sound("Airlock1");
  return(1);
}

func PutSolidMask(...) {
  SetSolidMask(nil,14,HSolidX(),6,nil,14);
  Sound("Airlock2");
  return(1);
}

func HMinX(...) { return(-10);
}
func HMaxX(...) { return(20);
}
func HSolidX(...) { return(24);
}
public func GetResearchBase(...) { return(nil);
}








