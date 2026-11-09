#strict

local openmode,autoclose,closetimer,state;

Initialize:
  state = 0;
  openmode=1;
  autoclose=1;
  DigFreeRect(GetX()+HMinX(),GetY()+11,HMaxX(),30);
  PutSolidMask();
  return(1);

CheckArea:
  if (openmode==1)
  {
    if (Var(0)=FindObject(0,HMinX(),10,HMaxX(),17,OCF_CrewMember()))
      ControlUp(Var(0));
    else
      if (!(Var(0)=FindObject(0,HMinX(),-10,HMaxX(),20,OCF_CrewMember())))
        if (autoclose==1) ControlDownSingle(Var(0));
  }
  else
  {
    if (Var(0)=FindObject(0,HMinX(),-10,HMaxX(),37,OCF_CrewMember()))
      ControlUp(Var(0));
    else
      if (!(Var(0)=FindObject(0,HMinX(),-10,HMaxX(),20,OCF_CrewMember())))
        if (autoclose==1) ControlDownSingle(Var(0));
  }
  closetimer=Max(0,closetimer-1);
  if ((autoclose==2)&&(closetimer==0)) ControlDownSingle();
  return(1);
  
ControlThrow:
  [Set close mode]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  autoclose=autoclose+1;  if (autoclose>2) autoclose=0;
  if (autoclose==2) Message("Timer close", this());
  if (autoclose==1) Message("Auto close", this());
  if (autoclose==0) Message("Manual close", this());
  return(1);

ControlDig:
  [Set open mode]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  openmode=openmode+1;  if (openmode>1) openmode=0;
  if (openmode==1) Message("Auto bottom open", this());
  if (openmode==0) Message("Full auto open", this());
  return(1);

ControlDownSingle:
  [Close]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  if (GetAction()ne"HatchOpen") return(0);
  if (GetAction()ne"CloseHatch") SetAction("CloseHatch");
  state = 0;
  return(1);

ControlUp:
  [Open]
  if (Hostile(GetController(Par(0)),GetOwner())) return(0);
  if (GetAction()ne"Idle") return(1);
  closetimer=100;
  SetAction("OpenHatch");
  state = 1;
  return(1);

RemoveSolidMask:
  SetSolidMask();
  Sound("Airlock1");
  return(1);

PutSolidMask:
  SetSolidMask(0,14,HSolidX(),6,0,14);
  Sound("Airlock2");
  return(1);

HMinX: return(-10);
HMaxX: return(20);
HSolidX: return(24);
public GetResearchBase: return(0);








