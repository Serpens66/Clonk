/*-- Kohlenmeiler --*/

#strict

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
  if (!FindContents(WOOD)) return(Message("Kein Holz verf¸gbar.",Par()));
  return(1);

/* Steuerung */

protected ControlUp: // C4Object *pCaller
  [Kohleproduktion|Image=COAL]
  return(StartProduction(Par(0)));
  
  
func Collection(obj) // wenn etwas betritt
{
    if(GetID(obj)==WOOD) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas verl‰sst
{
    if(GetID(obj)==WOOD) obj->~DoCon(100);
    return(_inherited(obj));
}


func Burning()
{
 if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight)-4,GetY(pLight)+4);}  // Licht, wenn burning
 Smoke(0,Random(3)-18,Random(3)+3);
 if(GetActTime()>600) {CreateContents(COAL);return(SetAction("Idle"));}
  if (ContentsCount(WOOD)<3) return(1);
 if (!(Var(0)=FindContents(WOOD))) return(1);
 RemoveObject(Var(0));
 if (!(Var(0)=FindContents(WOOD))) return(1);
 RemoveObject(Var(0));
 if (!(Var(0)=FindContents(WOOD))) return(1);
 RemoveObject(Var(0));
}

func Burning2() //Gras verbrennen
{
 if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight)-4,GetY(pLight)+4);}  // Licht, wenn burning
 Smoke(0,Random(3)-18,Random(3)+3);
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

  
func ContentsCheck()
{
 if(!ActIdle()) 
   return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
 if (ContentsCount(WOOD)>=3) SetAction("Burning");
  if(ContentsCount(DTFH)>=20) SetAction("Burning2");
}


func Construction()
{
 if(!FindCastlePart())
  Local(9)=CreateObject(BA35,0,8,GetOwner());
}
  
func Destruction()
{
 if(Local(9)) RemoveObject(Local(9));
}


// auﬂer Holz und Kohle nichts aufnehmen 
protected func RejectCollect(def,pObj) { if(def!=WOOD && def!=COAL) return(1); }


func FindCastlePart()
{
 while (Var()=FindObject(0, Par(0),Par(1),0,0, OCF_Fullcon(),0,0, NoContainer(), Var(0)))
  if (ObjectCall(Var(0),"CastlePartWidth"))
   return(Var(0));
}