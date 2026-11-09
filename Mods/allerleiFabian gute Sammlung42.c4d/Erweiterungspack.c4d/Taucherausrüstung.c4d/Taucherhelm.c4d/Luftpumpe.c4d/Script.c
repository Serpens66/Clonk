/*-- Luftpumpe --*/

#strict 3
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
  if(And(!Equal(GetID(pWorker), CLNK), !Equal(GetID(pWorker), HZCK))) return(Message("Ein %s kann keine Luftpumpe benutzen.|Die Luftpumpe kann nur von|Clonks benutzt werden.",pWorker,   GetName(nil,GetID(pWorker))));
  if(!!SEqual(GetAction(), "Working")) return(Message("Luftpumpe wird bereits bedient.",pWorker));
  AddCommand(pWorker,"Call",this(),nil,nil,nil,nil,"Work",nil,1);
  AddCommand(pWorker,"MoveTo",nil,(GetX() ?? 0)+10,GetY());
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
  if(And(!FindContents(CLNK), !FindContents(HZCK))) return(SetAction("Idle"));
  CheckForSolid();
  return(nil);
}

public func CheckForSolid() { if(GBackSemiSolid()) return(WorkCancel()); }

private func WorkCancel(pWorker)
{
  if(!pWorker)  pWorker=FindContents(CLNK);
  if(!pWorker)  pWorker=FindContents(HZCK);
  if (!!SEqual(GetAction(), "Working")) SetAction("Idle");
  Exit(pWorker,10,10);
  SetDir(DIR_Left, pWorker);
  return(1);
}