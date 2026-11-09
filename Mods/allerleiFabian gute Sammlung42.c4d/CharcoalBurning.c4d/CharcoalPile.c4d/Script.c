/*-- Kohlenmeiler --*/

#strict 3

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
  if (!FindContents(WOOD)) return(Message("Kein Holz verf¸gbar.",Par()));
  return(1);
}

/* Steuerung */

protected func ControlUp(...) { // C4Object *pCaller
  [Kohleproduktion|Image=COAL]
  return(StartProduction(Par(nil)));
}
  
  
func Collection(obj) // wenn etwas betritt
{
    if(Equal(GetID(obj), WOOD)) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas verl‰sst
{
    if(Equal(GetID(obj), WOOD)) obj->~DoCon(100);
    return(_inherited(obj));
}


func Burning()
{
 if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);pLight->SetPosition((GetX(pLight) ?? 0)-4,(GetY(pLight) ?? 0)+4);}  // Licht, wenn burning
 Smoke(nil,(Random(3) ?? 0)-18,(Random(3) ?? 0)+3);
 if((GetActTime() ?? 0)>600) {CreateContents(COAL);return(SetAction("Idle"));}
  if ((ContentsCount(WOOD) ?? 0)<3) return(1);
 if (!(Var(nil)=FindContents(WOOD))) return(1);
 RemoveObject(Var(nil));
 if (!(Var(nil)=FindContents(WOOD))) return(1);
 RemoveObject(Var(nil));
 if (!(Var(nil)=FindContents(WOOD))) return(1);
 RemoveObject(Var(nil));
}

func Burning2() //Gras verbrennen
{
 if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);pLight->SetPosition((GetX(pLight) ?? 0)-4,(GetY(pLight) ?? 0)+4);}  // Licht, wenn burning
 Smoke(nil,(Random(3) ?? 0)-18,(Random(3) ?? 0)+3);
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

  
func ContentsCheck()
{
 if(!ActIdle()) 
   return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
 if ((ContentsCount(WOOD) ?? 0)>=3) SetAction("Burning");
  if((ContentsCount(DTFH) ?? 0)>=20) SetAction("Burning2");
}


func Construction()
{
 if(!FindCastlePart())
  Local(9)=CreateObject(BA35,nil,8,GetOwner());
}
  
func Destruction()
{
 if(Local(9)) RemoveObject(Local(9));
}


// auﬂer Holz und Kohle nichts aufnehmen 
protected func RejectCollect(def,pObj) { if(And(!Equal(def, WOOD), !Equal(def, COAL))) return(1); }


func FindCastlePart()
{
 while (Var()=FindObject(nil, Par(nil),Par(1),nil,nil, OCF_Fullcon,nil,nil, NoContainer(), Var(nil)))
  if (ObjectCall(Var(nil),"CastlePartWidth"))
   return(Var(nil));
}