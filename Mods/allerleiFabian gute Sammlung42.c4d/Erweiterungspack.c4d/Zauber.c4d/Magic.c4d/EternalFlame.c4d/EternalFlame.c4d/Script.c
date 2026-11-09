#strict 3

func Completion()
{
  Sound("Fire");
  SetAction("Burn1");
  SetPhase(Random(14));
  SetComDir(COMD_Down);
  return(1);
}

func ChkIncinerate()
{
  while(Var(nil)=FindObject(nil,-6,-6,12,12,OCF_Prey,nil,nil,NoContainer(),Var()))
    if(Hostile(GetOwner(),GetOwner(Var()))) if(!OnFire(Var())) Incinerate(Var(nil));
  return(1);
}

func Damage()
{
  Sound("Fire");
  Explode(15);
  return(1);
}
