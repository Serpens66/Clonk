/*-- Köhlerei --*/

#strict 3

#include BAS4
#include DOOR

local pLight;

/* Produktion */

public func StartProduction(...) { // C4Object *pWorker
  // Arbeitsbefehl setzen
  AddCommand(Par(nil),"Call",this(),COAL,nil,nil,nil,"Production");
  return(1);
}

public func Production(...) { // C4Object *pWorker, C4ID idProduct
  AddCommand(Par(nil),"Call",this(),nil,nil,nil,nil,"Acquisition");
  AddCommand(Par(nil),"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(Par(nil),"Acquire",nil,nil,nil,this(),nil,WOOD);
  AddCommand(Par(),"Wait",nil,nil,nil,nil,nil,10);

  AddCommand(Par(nil),"Call",this(),nil,nil,nil,nil,"Acquisition");
  AddCommand(Par(nil),"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(Par(nil),"Acquire",nil,nil,nil,this(),nil,WOOD);
  AddCommand(Par(),"Wait",nil,nil,nil,nil,nil,10);

  AddCommand(Par(nil),"Call",this(),nil,nil,nil,nil,"Acquisition");
  AddCommand(Par(nil),"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(Par(nil),"Acquire",nil,nil,nil,this(),nil,WOOD);
  AddCommand(Par(),"Wait",nil,nil,nil,nil,nil,10);

  AddCommand(Par(nil),"Call",this(),nil,nil,nil,nil,"Acquisition");
  AddCommand(Par(nil),"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(Par(nil),"Acquire",nil,nil,nil,this(),nil,WOOD);
  AddCommand(Par(),"Wait",nil,nil,nil,nil,nil,10);

  AddCommand(Par(nil),"Call",this(),nil,nil,nil,nil,"Acquisition");
  AddCommand(Par(nil),"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(Par(nil),"Acquire",nil,nil,nil,this(),nil,WOOD);
  AddCommand(Par(),"Wait",nil,nil,nil,nil,nil,10);
  return(1);
}

public func Acquisition(...) { // C4Object *pWorker
  // Materialbeschaffung erfolgreich
  return(1);
}

public func AcquisitionFailed(...) { // C4Object *pWorker
  // Materialbeschaffung fehlgeschlagen: Meldung ausgeben
  if (!FindContents(WOOD)) return(Message("Kein Holz verfügbar.",Par()));
  return(1);
}

/* Kontext */

public func ContextConstruction(...) { // C4Object *pCaller
  [Kohleproduktion|Image=COAL|Condition=IsBuilt]
  return(StartProduction(Par(nil)));
}

protected func IsBuilt() {
  return((GetCon() ?? 0) >= 100);
}

/* Steuerung */

protected func ContainedUp(...) { // C4Object *pCaller
  [Kohleproduktion|Image=COAL]
  return(StartProduction(Par(nil)));
}
  
/* Eingangssteuerung */

private func SoundOpenDoor(...) {
  return(Sound("GateOpen"));
}

private func SoundCloseDoor(...) {
  return(Sound("GateClose"));
}
  
func Collection(obj) // wenn etwas betritt
{
    if(Equal(GetID(obj), WOOD)) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas verlässt
{
    if(Equal(GetID(obj), WOOD)) obj->~DoCon(100);
    return(_inherited(obj));
}

/* Produktion */

private func Burning(...) {
  if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);}  // Licht, wenn burning
  Smoke(-13,(Random(3) ?? 0)-24,(Random(5) ?? 0)+8);
  if((GetActTime() ?? 0)>600) {CreateContents(COAL);return(SetAction("Idle"));}
  if ((ContentsCount(WOOD) ?? 0)<2) return(1);
  if ((Var(nil)=FindContents(WOOD)))
  RemoveObject(Var(nil));
  if ((Var(nil)=FindContents(WOOD)))
  RemoveObject(Var(nil));
  return(1);
}

func Burning2()
{
  if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);}  // Licht, wenn burning
  Smoke(-13,(Random(3) ?? 0)-24,(Random(5) ?? 0)+8);
  if((GetActTime() ?? 0)>600) {CreateContents(COAL);return(SetAction("Idle"));}
  if ((ContentsCount(DTFH) ?? 0)<20) return(1);
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
 if((Var(7) = FindContents(DTFH)))
  RemoveObject(Var(7));
  return(1);
}
  
/* Inhaltsüberprüfung */  
  
private func ContentsCheck(...) {
  // Warten...
  if (!ActIdle()) 
    return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
  // Loren ausleeren und rausschicken
  Var()=-1;
  while (Var(1)=Contents(S42MigrationStep(Var(nil), 1)))
    if (ObjectCall(Var(1), "IsLorry"))
      {
      GrabContents(Var(1));
      SetCommand(Var(1),"Exit");
      }

  // Holz prüfen
  if ((ContentsCount(WOOD) ?? 0)>=2)   SetAction("Burning");
  if((ContentsCount(DTFH) ?? 0)>=20) SetAction("Burning2");
  return(1);
}
