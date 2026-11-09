/*-- Dynamo --*/

#strict 3

#include BAS2

/* Steuerung */

public func ContextProduction(pCaller)
{
  [$TxtProduce$|Image=RSR1|Condition=IsBuilt]
  return(StartWork(pCaller));
}

protected func IsBuilt() {
  return((GetCon() ?? 0) >= 100);
}

func ContainedThrow() { return(1); }
  
public func ControlDigDouble(pCaller)
{
  [$TxtProduce$|Image=RSR1]
  return(StartWork(pCaller));
}

public func ContainedDown(pCaller)
{
  [$TxtCancelproduction$|Image=RSR2]
  WorkCancel(pCaller);
  return(1);
}

func StartWork(pWorker)
  {
  if(S42MigrationEqual(GetAction(), "Working")) return(Message("$TxtAlreadyInUse$", pWorker));
  // Arbeit beginnen
  AddCommand(pWorker,"Call",this(),nil,nil,nil, nil, "Work", nil, 1);
  // Vorher: Position einnehmen
  AddCommand(pWorker,"MoveTo",nil,(GetX() ?? 0)+10,GetY());
  }

func Work(pWorker)
  {
  // Clonk einsaugen
  Enter(this(),pWorker);
  // Besitzer setzen für Farbe des animierten Clonk
  SetOwner(GetOwner(pWorker));
  // Farbe nach Clonkfarbe richten
  SetColorDw(GetColorDw(pWorker));
  // Aktion starten
  SetAction("Working");
  return(1);
  }
// TimerCall
func EnergyTimer()
{
    if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
}
  
func Working()
  {
  // Ich bin ja ein vielbeschäftigter Timer
  if(!FindObject2(Find_OCF(OCF_CrewMember),Find_Container(this()))) return(SetAction("Idle"));
  CheckForSolid();
  var energy;
  if(GetPlrKnowledge(GetOwner(FindObject2(Find_OCF(OCF_CrewMember),Find_Container(this()))), DYYN)) // wenn arbeiter forschung hat, dann mehr energie, ansonsten wenig
      energy = +50; // 2,5 pro Frames
  else
      energy = +25; // 1.25 pro Frame
  return(DoEnergy(energy));
  }

func CheckForSolid()
  {
  if(GBackSemiSolid()) return(WorkCancel());
  }

private func WorkCancel(pWorker)
  {
  if(!pWorker) pWorker=FindObject2(Find_OCF(OCF_CrewMember),Find_Container(this()));
  // Arbeit abbrechen
  if (S42MigrationEqual(GetAction(), "Working")) SetAction("Idle");
  if(!pWorker) return(1);
  Exit(pWorker,10,10);
  SetDir(DIR_Left, pWorker);
  // Fertig
  return(1);
  }
