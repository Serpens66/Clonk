/*-- Hochofen --*/

#strict 3

#include BAS5
#include DOOR

local pLight;

/* Produktion */

public func IsProducerOf (object clonk, id def) {
	if (!IsBuilt())
		return nil;
	if (Equal(def, METL))
		return 1;
	if (Equal(def, GLAS))
		return 1;
	return nil;
}

public func HowToProduce (object clonk, id def) {
  AddCommand (clonk, "Wait", nil, nil, nil, nil, nil, 190);
  Production (clonk, def);
}

public func StartProduction(object clonk)
{
  // Arbeitsbefehl setzen
  AddCommand(clonk,"Call",this(),ANML,nil,nil,nil,"Production");
  return(1);
}

public func Production(object clonk, id idProduct)
{
  // Kein Brennmaterial
  if (!HasFuel())
    // Der Arbeiter soll welches beschaffen
    WorkerAcquireFuel(clonk);

  // Kein Erz im Hochofen
  if (!FindContents(ORE1))
    {
    // Der Arbeiter soll welches beschaffen
    AddCommand(clonk,"Call",this(),nil,nil,nil,nil,"Acquisition", nil, 3);
    AddCommand(clonk,"Put",this(),nil,nil,nil,nil,ORE1);
    AddCommand(clonk,"Acquire",nil,nil,nil,nil,nil,ORE1,3);
    }
  // Fertig
  return(1);
}
  
func WorkerAcquireFuel (object clonk) {
  AddCommand (clonk, "Call", this (), nil, nil, nil, nil, "Acquisition", nil, 3);
  // Wenn Kohle da (Leider noch keine Überprüfung auf Kohleherstellendes Objekt)
  var obj = GetAvailableObject (COAL, this());
  // Oder Kohle kaufbar
  if (!obj)
    if (GetHomebaseMaterial (GetOwner (clonk), COAL))
      if ((GetWealth (GetOwner (clonk)) ?? 0) >= (GetValue (nil, COAL) ?? 0))
        if (FindBase (GetOwner (clonk)))
          obj = 1;
  if (obj) {
    //Kohle reintun
    AddCommand(clonk,"Put",this(),nil,nil,nil,nil,COAL);
    //und vorher besorgen
    AddCommand(clonk,"Acquire",nil,nil,nil,this(),nil,COAL);
    return(1);
  }
  // ansonsten Holz verbrennen
  // ..zweites holen
  AddCommand(clonk,"Call",this(),nil,nil,nil,nil,"SupportWithWOOD", nil, 1);
  AddCommand(clonk,"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(clonk,"Acquire",nil,nil,nil,this(),nil,WOOD);
  // Damit er es nicht nochmal holt:
  AddCommand(clonk,"Wait",nil,nil,nil,nil,nil,10);
  // Erstes Holzstück holen...
  AddCommand(clonk,"Put",this(),nil,nil,nil,nil,WOOD);
  AddCommand(clonk,"Acquire",nil,nil,nil,this(),nil,WOOD);
}    

public func SupportWithWOOD() { return(1); }

public func SupportWithWOODFailed(pClonk)
{
  // Hat nicht geklappt, also mit Kohle versuchen
  AddCommand(pClonk, "Call", this(), nil, nil, nil, nil, "SupportWithCOAL", nil, 1);
  AddCommand(pClonk, "Put", this(), nil, nil, nil, nil, COAL);
  AddCommand(pClonk, "Acquire", nil, nil, nil, this(), nil, COAL);
  return(1);
}

public func SupportWithCOAL() { return(1); }

public func SupportWithCOALFailed(pClonk)
{
  // Hat auch nicht geklappt. Dann halt Ölfässer suchen...
  AddCommand(pClonk, "Call", this(), nil, nil, nil, nil, "Acquisition", nil, 3);
  AddCommand(pClonk, "Put", this(), nil, nil, nil, nil, OBRL);
  AddCommand(pClonk, "Acquire", nil, nil, nil, this(), nil, OBRL);
  return(1);
}

// --------------------------------------------------

private func HasFuel()
{
  if ((ContentsCount(COAL) ?? 0)>0) return(1);
  if ((ContentsCount(OBRL) ?? 0)>0) return(1);
  if ((ContentsCount(WOOD) ?? 0)>1) return(1);
  return(nil);
}

public func Acquisition(object pWorker)
{
  // Materialbeschaffung erfolgreich
  return(1);
}

public func AcquisitionFailed(object pWorker)
{
  // Materialbeschaffung fehlgeschlagen: Meldung ausgeben
  if (!FindContents(ORE1)) 
    return(Message("$TxtNooreavailable$", pWorker));
  Message("$TxtNeedsfueltoburn$", pWorker);
  return(1);
}

/* Kontext */

public func ContextConstruction(object pCaller)
{
  [$TxtProducemetal$|Image=ANML|Condition=IsBuilt]
  return(StartProduction(pCaller));
}

protected func IsBuilt() {
  return((GetCon() ?? 0) >= 100);
}

/* Steuerung */

protected func ContainedUp(object pCaller)
{
  [$TxtProducemetal$|Image=ANML]
  return(StartProduction(pCaller));
}

/* Eingangssteuerung */

private func SoundOpenDoor()
{
  return(Sound("GateOpen"));
}

private func SoundCloseDoor()
{
  return(Sound("GateClose"));
}

protected func Collection(obj) 
{ 
    if(Or(Equal(GetID(obj), WOOD), Equal(GetID(obj), COAL))) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die Fackel verlässt
{
    if(Or(Equal(GetID(obj), WOOD), Equal(GetID(obj), COAL))) obj->~DoCon(100);
    return(_inherited(obj));
}



/* Produktion */

private func Burning()
{
  if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);pLight->SetPosition((GetX(pLight) ?? 0)+5,GetY(pLight));}  // Licht, wenn burning
  // Rauch
  Smoke(+23,(Random(3) ?? 0)-24,(Random(5) ?? 0)+8);
  // Noch nicht fertig
  if ((GetActTime() ?? 0)<150) return(1);
  // Fertig
  SetAction("Idle");
  // Erz suchen
  var pOre;
	if (GetName(nil, GLAS))  // wenn die Glasdefintion geladen ist
		pOre = FindContents(SAND);
	if(!pOre)
		pOre = FindContents(ORE1);
  if(And(!pOre, (ContentsCount(MTWS) ?? 0)>2)) // mit 3 Metallschrott lässt sich auch metall herstellen
    {RemoveObject(FindContents(MTWS)); RemoveObject(FindContents(MTWS)); RemoveObject(FindContents(MTWS)); Exit(CreateContents(SHLK,nil,10), -27,+13,nil, -1);Exit(CreateContents(ANML,nil,10), -27,+13,nil, -1);};
  // Metall auswerfen
	if (Equal(GetID(pOre), ORE1))
		Exit(CreateContents(ANML), -27, +13, nil, -1);
	if (Equal(GetID(pOre), SAND)){
		Exit(CreateContents(GLAS), -27, +13, nil, -1); if(GetPlrKnowledge(GetOwner(),GLSH)) Exit(CreateContents(GLAS), -27, +13, nil, -1);}
   // Erz verbrauchen
  if(pOre) RemoveObject(pOre);
  if (!Random(10)) Exit(CreateContents(SHLK), -27,+13,nil, -1);
  // Sound
  Sound("Pshshsh");
  // Fertig
  return(1);
}

/* Inhaltsüberprüfung */  
  
private func ContentsCheck()
{
  // Warten...
  if (!ActIdle()) 
    return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
  // Loren rausschicken
  var obj;
  for(var i = nil; obj = Contents(i); S42MigrationStep(i, 1, true))
    if(obj->~IsLorry())
      {
      SetCommand(obj,"Exit");
      }
  // Erstes Inhaltsobjekt überprüfen
  obj = Contents(nil); var id = GetID(obj);
  // Alles außer Mannschaftsmitgliedern, Rohmaterial und Loren sofort auswerfen
  if (obj)
    if ( !(    Or(Or(Or(Or(Or(Or(Or((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0)
            , obj->~IsLorry())
            , Equal(id, COAL))
            , Equal(id, OBRL))
            , Equal(id, WOOD))
            , Equal(id, ORE1))
            , Equal(id, MTWS))
            , (And((id = SAND), (GetName(nil, GLAS)))))))
      if(Or(GetDefFragile(id), (GetCategory(obj) ?? 0) & (C4D_Vehicle ?? 0)))
        SetCommand(obj, "Exit");
      else
        Exit(obj,-27,+13,nil,-1);
  // Erz prüfen
  if (And(And(!(FindContents(ORE1)), !((ContentsCount(MTWS) ?? 0)>2)), !(FindContents(SAND))) ) return(1);
  // Holz verbrennen
  if ((ContentsCount(WOOD) ?? 0)>1) return(BurnWood());
  // Öl verbrennen
  if ((ContentsCount(OBRL) ?? 0)>0) return(BurnOil());
  // Kohle verbrennen
  if (!(obj=FindContents(COAL))) return(1);
  RemoveObject(obj);
  SetAction("Burning");
  return(1);
}

private func BurnWood()
{
  RemoveObject(FindContents(WOOD));
  RemoveObject(FindContents(WOOD));
  SetAction("Burning");
  return(1);
}

private func BurnOil()
{
  ChangeDef(BARL,FindContents(OBRL));
  SetAction("Burning");
  return(1);
}
