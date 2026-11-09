/*-- Löschobjekt --*/

#strict

local objekt, Nummer, Temp;

func Check()
{
  if(GetAction()ne"Idle") return(0);
  while(Var()=FindObject(0,0,0,0,0,OCF_OnFire(),0,0,0,Var()))
    if(!FindObject(GetID(),0,0,0,0,0,"IsOnFire",Var()) && GetAlive(Var(0)))
     ObjectSetAction(CreateObject(GetID(),0,0,-1),"IsOnFire",Var());
  return(1);
}

func DoExtinguish()
{
  if(!GetAlive(GetActionTarget())) return(RemoveObject());
  if(GetActTime()>180 && !Random(4)) {
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

