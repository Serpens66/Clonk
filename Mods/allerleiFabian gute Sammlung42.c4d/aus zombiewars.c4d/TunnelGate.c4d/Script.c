#strict 3


local openmode,autoclose,closetimer;

func Initialize() {
  openmode=nil;
  autoclose=1;
  SetAction("Ready");
  PutSolidMask();
  return(1);
}

func CheckArea() {
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
  if (!EnergyCheck(25000)) 
  {
      if(!SEqual(GetAction(), "Ready")) return(1);
      if(!!SEqual(GetAction(), "GateOpen")) return(1);
      return(SetAction("OpenGate"));
  }
  if (Equal(openmode, 2)) if (Var(nil)=FindObject(nil,-10,-11,10,23,OCF_CrewMember))
    ControlUp(Var(nil));
  if (Equal(openmode, 1)) if (Var(nil)=FindObject(nil,1,-11,11,23,OCF_CrewMember))
    ControlUp(Var(nil));
  if (Equal(openmode, nil)) if (Var(nil)=FindObject(nil,-10,-11,21,23,OCF_CrewMember))
    ControlUp(Var(nil));
  if (And(!(Var(nil)=FindObject(nil,-10,-11,21,23,OCF_CrewMember)), (Equal(autoclose, 1))))
    ControlDownSingle();
  closetimer=Max(nil,(closetimer ?? 0)-1);
  if (And((Equal(autoclose, 2)), (Equal(closetimer, nil)))) ControlDownSingle();
  return(1);
}
  
func ControlThrow() {
  [Set close mode]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  autoclose=(autoclose ?? 0)+1;  if ((autoclose ?? 0)>2) autoclose=nil;
  if (Equal(autoclose, 2)) Message("Timer close", this());
  if (Equal(autoclose, 1)) Message("Auto close", this());
  if (Equal(autoclose, nil)) Message("Manual close", this());
  return(1);
}

func ControlDig() {
  [Set open mode]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  openmode=(openmode ?? 0)+1;  if ((openmode ?? 0)>3) openmode=nil;
  if (Equal(openmode, 3)) Message("Manual open", this());
  if (Equal(openmode, 2)) Message("Left side auto open", this());
  if (Equal(openmode, 1)) Message("Right side auto open", this());
  if (Equal(openmode, nil)) Message("Full auto open", this());
  return(1);
}

func ControlDownSingle() {
  [Close]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  if (!SEqual(GetAction(), "GateOpen")) return(nil);
  if (!SEqual(GetAction(), "CloseGate")) SetAction("CloseGate");
  return(1);
}

func ControlUp() {
  [Open]
  if (Hostile(GetController(Par(nil)),GetOwner())) return(nil);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  if (!SEqual(GetAction(), "Ready")) return(1);
  closetimer=100;
  SetAction("OpenGate");
  return(1);
}

func RemoveSolidMask() {
  SetSolidMask(nil,nil,14,26,nil,nil);
  Sound("Airlock1");
  return(1);
}

func PutSolidMask() {
  SetSolidMask(nil,26,14,26,nil,nil);
  Sound("Airlock2");
  return(1);
}

// func Damage() {
  // if (GetDamage()>600)
  // {
    // RemoveObject();
    // CastObjects(METL,1,30);
  // }
  // return(1);
// }

// public func GetResearchBase() { return(0); }
