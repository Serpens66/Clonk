#strict


local openmode,autoclose,closetimer;

func Initialize() {
  openmode=0;
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
      if(GetAction()ne"Ready") return(1);
      if(GetAction()eq"GateOpen") return(1);
      return(SetAction("OpenGate"));
  }
  if (openmode==2) if (Var(0)=FindObject(0,-10,-11,10,23,OCF_CrewMember()))
    ControlUp(Var(0));
  if (openmode==1) if (Var(0)=FindObject(0,1,-11,11,23,OCF_CrewMember()))
    ControlUp(Var(0));
  if (openmode==0) if (Var(0)=FindObject(0,-10,-11,21,23,OCF_CrewMember()))
    ControlUp(Var(0));
  if (!(Var(0)=FindObject(0,-10,-11,21,23,OCF_CrewMember()))&&(autoclose==1))
    ControlDownSingle();
  closetimer=Max(0,closetimer-1);
  if ((autoclose==2)&&(closetimer==0)) ControlDownSingle();
  return(1);
}
  
func ControlThrow() {
  [Set close mode]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  autoclose=autoclose+1;  if (autoclose>2) autoclose=0;
  if (autoclose==2) Message("Timer close", this());
  if (autoclose==1) Message("Auto close", this());
  if (autoclose==0) Message("Manual close", this());
  return(1);
}

func ControlDig() {
  [Set open mode]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  openmode=openmode+1;  if (openmode>3) openmode=0;
  if (openmode==3) Message("Manual open", this());
  if (openmode==2) Message("Left side auto open", this());
  if (openmode==1) Message("Right side auto open", this());
  if (openmode==0) Message("Full auto open", this());
  return(1);
}

func ControlDownSingle() {
  [Close]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  if (GetAction()ne"GateOpen") return(0);
  if (GetAction()ne"CloseGate") SetAction("CloseGate");
  return(1);
}

func ControlUp() {
  [Open]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  if (!EnergyCheck(25000)) return(Message(Format("%v braucht Energie",GetName(this())),this()));
  if (GetAction()ne"Ready") return(1);
  closetimer=100;
  SetAction("OpenGate");
  return(1);
}

func RemoveSolidMask() {
  SetSolidMask(0,0,14,26,0,0);
  Sound("Airlock1");
  return(1);
}

func PutSolidMask() {
  SetSolidMask(0,26,14,26,0,0);
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
