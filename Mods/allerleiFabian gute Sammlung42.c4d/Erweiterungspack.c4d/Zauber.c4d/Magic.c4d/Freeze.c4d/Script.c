#strict 3

func Activate()
{
  Sound("Magic1");
  while(Var()=FindObject(nil,nil,nil,nil,nil,OCF_CrewMember,nil,nil,nil,Var())) {
    if(!Equal(GetOwner(Par()), GetOwner(Var()))) ObjectSetAction(Var(1)=CreateObject(ICBL,GetX(Var()),GetY(Var()),-1),"Freeze",Var());
    Extinguish(Var());
    AddVertex(GetVertex(nil,nil,Var()),
              GetVertex(nil,1,Var()),Var(1));
  }

  SetAction("Ticking");
  return(1);  
}

func Remove()
{
  while(Var()=FindObject(ICBL,nil,nil,nil,nil,nil,nil,nil,nil,Var())) Var()->ICBL::Destruct();
return(RemoveObject());
}

func Tick()
{
 Sound("TickTack");
 return(1);
} 
