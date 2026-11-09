/*-- Löschobjekt --*/

#strict 3

local objekt, Nummer, Temp;

func Check()
{
  if(!SEqual(GetAction(), "Idle")) return(nil);
  while(Var()=FindObject(nil,nil,nil,nil,nil,OCF_OnFire,nil,nil,nil,Var()))
    if(And(!FindObject(GetID(),nil,nil,nil,nil,nil,"IsOnFire",Var()), GetAlive(Var(nil))))
     ObjectSetAction(CreateObject(GetID(),nil,nil,-1),"IsOnFire",Var());
  return(1);
}

func DoExtinguish()
{
  if(!GetAlive(GetActionTarget())) return(RemoveObject());
  if(And((GetActTime() ?? 0)>180, !Random(4))) {
    Extinguish(GetActionTarget());
   // feuerleuchteffekt sollte durch das stoppen des feuers von alleine weggehen.
    return(RemoveObject());
  }
  return(1);
}

func Activate()
{
  MessageWindow(GetDesc(),Par());
  return(1);
}

