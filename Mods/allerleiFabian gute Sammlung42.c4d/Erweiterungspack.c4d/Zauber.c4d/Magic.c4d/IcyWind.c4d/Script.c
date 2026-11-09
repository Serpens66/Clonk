#strict 3

func Activate()
{
  Sound("Wind2");
  if (Equal(GetDir(Par(nil)), DIR_Left)) {
   while(Var()=FindObject(nil,-150,-80,150,160,OCF_CrewMember,nil,nil,NoContainer(),Var()))
     if(Hostile(GetOwner(Par()),GetOwner(Var())))
      if(And(!FindObject(CLDN,nil,nil,nil,nil,nil,"WarnCold",Var()), !FindObject(CLDN,nil,nil,nil,nil,nil,"WarnColdX",Var())))
        ObjectSetAction(CreateObject(CLDN,nil,nil,-1),"WarnCold",Var(),Var());
  }
  if (Equal(GetDir(Par(nil)), DIR_Right)) {
   while(Var()=FindObject(nil,nil,-80,150,160,OCF_CrewMember,nil,nil,NoContainer(),Var()))
     if(Hostile(GetOwner(Par()),GetOwner(Var()))) 
       if(And(!FindObject(CLDN,nil,nil,nil,nil,nil,"WarnCold",Var()), !FindObject(CLDN,nil,nil,nil,nil,nil,"WarnColdX",Var())))
         ObjectSetAction(CreateObject(CLDN,nil,nil,-1),"WarnCold",Var(),Var());
  }
  RemoveObject();
  return(1);
}
