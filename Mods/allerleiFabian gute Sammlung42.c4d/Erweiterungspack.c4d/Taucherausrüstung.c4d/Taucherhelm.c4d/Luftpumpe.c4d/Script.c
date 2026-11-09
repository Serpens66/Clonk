/*-- Luftpumpe --*/

#strict
#include BAS2

public func ContainedThrow() { return(1); }

public func ControlDigDouble(pCaller)
{
  [Pumpen|Image=RSR1]
  return(StartWork(pCaller));
}

public func ContainedDown(pCaller)
{
  [Arbeit abbrechen|Image=RSR2]
  WorkCancel(pCaller);
  return(1);
}

public func StartWork(pWorker)
  {
  if(GetID(pWorker)!= CLNK && GetID(pWorker)!= HZCK) return(Message("Ein %s kann keine Luftpumpe benutzen.|Die Luftpumpe kann nur von|Clonks benutzt werden.",pWorker,   GetName(0,GetID(pWorker))));
  if(GetAction() eq "Working") return(Message("Luftpumpe wird bereits bedient.",pWorker)); 
  AddCommand(pWorker,"Call",this(),0,0,0,0,"Work",0,1);
  AddCommand(pWorker,"MoveTo",0,GetX()+10,GetY());
  }

public func Work(pWorker)
{
  Enter(this(),pWorker);
  SetOwner(GetOwner(pWorker));
  SetColorDw(GetColorDw(pWorker));
  SetAction("Working");
  return(1);
}

public func Working()
{
  if(!FindContents(CLNK) && !FindContents(HZCK)) return(SetAction("Idle"));
  CheckForSolid();
  return(0);
}

public func CheckForSolid() { if(GBackSemiSolid()) return(WorkCancel()); }

private func WorkCancel(pWorker)
{
  if(!pWorker)  pWorker=FindContents(CLNK);
  if(!pWorker)  pWorker=FindContents(HZCK);
  if (GetAction() eq "Working") SetAction("Idle");
  Exit(pWorker,10,10);
  SetDir(DIR_Left(), pWorker);
  return(1);
}