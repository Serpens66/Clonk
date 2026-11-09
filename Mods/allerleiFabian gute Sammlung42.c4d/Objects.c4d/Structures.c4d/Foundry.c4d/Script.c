/*-- Hochofen --*/

#strict

#include BAS5
#include DOOR

local pLight;

/* Produktion */

public func IsProducerOf (object clonk, id def) {
	if (!IsBuilt())
		return 0;
	if (def == METL)
		return 1;
	if (def == GLAS)
		return 1;
	return 0;
}

public func HowToProduce (object clonk, id def) {
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 0, 190);
  Production (clonk, def);
}

public func StartProduction(object clonk)
{
  // Arbeitsbefehl setzen
  AddCommand(clonk,"Call",this(),ANML,0,0,0,"Production");
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
    AddCommand(clonk,"Call",this(),0,0,0,0,"Acquisition", 0, 3);
    AddCommand(clonk,"Put",this(),0,0,0,0,ORE1);
    AddCommand(clonk,"Acquire",0,0,0,0,0,ORE1,3);  
    }
  // Fertig
  return(1);
}
  
func WorkerAcquireFuel (object clonk) {
  AddCommand (clonk, "Call", this (), 0, 0, 0, 0, "Acquisition", 0, 3);
  // Wenn Kohle da (Leider noch keine Überprüfung auf Kohleherstellendes Objekt)
  var obj = GetAvailableObject (COAL, this());
  // Oder Kohle kaufbar
  if (!obj)
    if (GetHomebaseMaterial (GetOwner (clonk), COAL))
      if (GetWealth (GetOwner (clonk)) >= GetValue (0, COAL))
        if (FindBase (GetOwner (clonk)))
          obj = 1;
  if (obj) {
    //Kohle reintun
    AddCommand(clonk,"Put",this(),0,0,0,0,COAL);
    //und vorher besorgen
    AddCommand(clonk,"Acquire",0,0,0,this(),0,COAL);
    return(1);
  }
  // ansonsten Holz verbrennen
  // ..zweites holen
  AddCommand(clonk,"Call",this(),0,0,0,0,"SupportWithWOOD", 0, 1);
  AddCommand(clonk,"Put",this(),0,0,0,0,WOOD);
  AddCommand(clonk,"Acquire",0,0,0,this(),0,WOOD);
  // Damit er es nicht nochmal holt:
  AddCommand(clonk,"Wait",0,0,0,0,0,10);
  // Erstes Holzstück holen...
  AddCommand(clonk,"Put",this(),0,0,0,0,WOOD);
  AddCommand(clonk,"Acquire",0,0,0,this(),0,WOOD);
}    

public func SupportWithWOOD() { return(1); }

public func SupportWithWOODFailed(pClonk)
{
  // Hat nicht geklappt, also mit Kohle versuchen
  AddCommand(pClonk, "Call", this(), 0, 0, 0, 0, "SupportWithCOAL", 0, 1);
  AddCommand(pClonk, "Put", this(), 0, 0, 0, 0, COAL);
  AddCommand(pClonk, "Acquire", 0, 0, 0, this(), 0, COAL);
  return(1);
}

public func SupportWithCOAL() { return(1); }

public func SupportWithCOALFailed(pClonk)
{
  // Hat auch nicht geklappt. Dann halt Ölfässer suchen...
  AddCommand(pClonk, "Call", this(), 0, 0, 0, 0, "Acquisition", 0, 3);
  AddCommand(pClonk, "Put", this(), 0, 0, 0, 0, OBRL);
  AddCommand(pClonk, "Acquire", 0, 0, 0, this(), 0, OBRL);
  return(1);
}

// --------------------------------------------------

private func HasFuel()
{
  if (ContentsCount(COAL)>0) return(1);
  if (ContentsCount(OBRL)>0) return(1);
  if (ContentsCount(WOOD)>1) return(1);
  return(0);  
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
  return(GetCon() >= 100);
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
    if(GetID(obj)==WOOD || GetID(obj)==COAL) obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    Sound("Clonk");
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die Fackel verlässt
{
    if(GetID(obj)==WOOD || GetID(obj)==COAL) obj->~DoCon(100);
    return(_inherited(obj));
}



/* Produktion */

private func Burning()
{
  if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight)+5,GetY(pLight));}  // Licht, wenn burning
  // Rauch
  Smoke(+23,Random(3)-24,Random(5)+8);
  // Noch nicht fertig
  if (GetActTime()<150) return(1);
  // Fertig
  SetAction("Idle");
  // Erz suchen
  var pOre;
	if (GetName(0, GLAS))  // wenn die Glasdefintion geladen ist
		pOre = FindContents(SAND);
	if(!pOre)
		pOre = FindContents(ORE1);
  if(!pOre && ContentsCount(MTWS)>2) // mit 3 Metallschrott lässt sich auch metall herstellen
    {RemoveObject(FindContents(MTWS)); RemoveObject(FindContents(MTWS)); RemoveObject(FindContents(MTWS)); Exit(CreateContents(SHLK,0,10), -27,+13,0, -1);Exit(CreateContents(ANML,0,10), -27,+13,0, -1);};
  // Metall auswerfen
	if (GetID(pOre) == ORE1)
		Exit(CreateContents(ANML), -27, +13, 0, -1);
	if (GetID(pOre) == SAND){
		Exit(CreateContents(GLAS), -27, +13, 0, -1); if(GetPlrKnowledge(GetOwner(),GLSH)) Exit(CreateContents(GLAS), -27, +13, 0, -1);}
   // Erz verbrauchen
  if(pOre) RemoveObject(pOre);
  if (!Random(10)) Exit(CreateContents(SHLK), -27,+13,0, -1);
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
  for(var i = 0; obj = Contents(i); i++)
    if(obj->~IsLorry())
      {
      SetCommand(obj,"Exit");
      }
  // Erstes Inhaltsobjekt überprüfen
  obj = Contents(0); var id = GetID(obj);
  // Alles außer Mannschaftsmitgliedern, Rohmaterial und Loren sofort auswerfen
  if (obj)
    if ( !(    GetOCF(obj) & OCF_CrewMember()
            || obj->~IsLorry()
            || id==COAL
            || id==OBRL
            || id==WOOD
            || id==ORE1
            || id==MTWS
            || ((id = SAND) && (GetName(0, GLAS)))))
      if(GetDefFragile(id) || GetCategory(obj) & C4D_Vehicle)
        SetCommand(obj, "Exit");
      else
        Exit(obj,-27,+13,0,-1);
  // Erz prüfen
  if (!(FindContents(ORE1)) && !(ContentsCount(MTWS)>2)  && !(FindContents(SAND)) ) return(1);
  // Holz verbrennen
  if (ContentsCount(WOOD)>1) return(BurnWood());
  // Öl verbrennen
  if (ContentsCount(OBRL)>0) return(BurnOil());
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
