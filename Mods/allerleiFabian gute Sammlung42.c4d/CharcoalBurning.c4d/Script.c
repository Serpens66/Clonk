/*-- Köhlerei --*/

#strict

#include BAS4
#include DOOR

local pLight;

/* Produktion */

public StartProduction: // C4Object *pWorker
  // Arbeitsbefehl setzen
  AddCommand(Par(0),"Call",this(),COAL,0,0,0,"Production");
  return(1);

public Production: // C4Object *pWorker, C4ID idProduct
  AddCommand(Par(0),"Call",this(),0,0,0,0,"Acquisition");
  AddCommand(Par(0),"Put",this(),0,0,0,0,WOOD);
  AddCommand(Par(0),"Acquire",0,0,0,this(),0,WOOD);  
  AddCommand(Par(),"Wait",0,0,0,0,0,10);

  AddCommand(Par(0),"Call",this(),0,0,0,0,"Acquisition");
  AddCommand(Par(0),"Put",this(),0,0,0,0,WOOD);
  AddCommand(Par(0),"Acquire",0,0,0,this(),0,WOOD);  
  AddCommand(Par(),"Wait",0,0,0,0,0,10);

  AddCommand(Par(0),"Call",this(),0,0,0,0,"Acquisition");
  AddCommand(Par(0),"Put",this(),0,0,0,0,WOOD);
  AddCommand(Par(0),"Acquire",0,0,0,this(),0,WOOD);  
  AddCommand(Par(),"Wait",0,0,0,0,0,10);

  AddCommand(Par(0),"Call",this(),0,0,0,0,"Acquisition");
  AddCommand(Par(0),"Put",this(),0,0,0,0,WOOD);
  AddCommand(Par(0),"Acquire",0,0,0,this(),0,WOOD);  
  AddCommand(Par(),"Wait",0,0,0,0,0,10);

  AddCommand(Par(0),"Call",this(),0,0,0,0,"Acquisition");
  AddCommand(Par(0),"Put",this(),0,0,0,0,WOOD);
  AddCommand(Par(0),"Acquire",0,0,0,this(),0,WOOD);  
  AddCommand(Par(),"Wait",0,0,0,0,0,10);
  return(1);

public Acquisition: // C4Object *pWorker
  // Materialbeschaffung erfolgreich
  return(1);

public AcquisitionFailed: // C4Object *pWorker
  // Materialbeschaffung fehlgeschlagen: Meldung ausgeben
  if (!FindContents(WOOD)) return(Message("Kein Holz verfügbar.",Par()));
  return(1);

/* Kontext */

public ContextConstruction: // C4Object *pCaller
  [Kohleproduktion|Image=COAL|Condition=IsBuilt]
  return(StartProduction(Par(0)));

protected func IsBuilt() {
  return(GetCon() >= 100);
}

/* Steuerung */

protected ContainedUp: // C4Object *pCaller
  [Kohleproduktion|Image=COAL]
  return(StartProduction(Par(0)));
  
/* Eingangssteuerung */

private SoundOpenDoor:
  return(Sound("GateOpen"));

private SoundCloseDoor:
  return(Sound("GateClose"));
  
func Collection(obj) // wenn etwas betritt
{
    if(GetID(obj)==WOOD) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas verlässt
{
    if(GetID(obj)==WOOD) obj->~DoCon(100);
    return(_inherited(obj));
}

/* Produktion */

private Burning:
  if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);}  // Licht, wenn burning
  Smoke(-13,Random(3)-24,Random(5)+8);
  if(GetActTime()>600) {CreateContents(COAL);return(SetAction("Idle"));}
  if (ContentsCount(WOOD)<2) return(1);
  if ((Var(0)=FindContents(WOOD))) 
  RemoveObject(Var(0));
  if ((Var(0)=FindContents(WOOD))) 
  RemoveObject(Var(0));
  return(1);

func Burning2()
{
  if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);}  // Licht, wenn burning
  Smoke(-13,Random(3)-24,Random(5)+8);
  if(GetActTime()>600) {CreateContents(COAL);return(SetAction("Idle"));}
  if (ContentsCount(DTFH)<20) return(1);
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
  
private ContentsCheck:
  // Warten...
  if (!ActIdle()) 
    return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
  // Loren ausleeren und rausschicken
  Var()=-1;
  while (Var(1)=Contents(++Var(0)))
    if (ObjectCall(Var(1), "IsLorry"))
      {
      GrabContents(Var(1));
      SetCommand(Var(1),"Exit");
      }

  // Holz prüfen
  if (ContentsCount(WOOD)>=2)   SetAction("Burning");
  if(ContentsCount(DTFH)>=20) SetAction("Burning2");
  return(1);
