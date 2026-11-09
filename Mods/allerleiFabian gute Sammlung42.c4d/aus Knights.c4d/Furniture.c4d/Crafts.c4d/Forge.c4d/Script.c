/*-- Esse --*/

#strict

#include BAS1

local counter,pLight;

func Initialize()
{
    counter=0;
}

/* Steuerung */

public func ContextProduction(pCaller)
{
  [$TxtHeatmetal$|Image=ANML]
  return(ProductionOrder(ANML, pCaller));
}
 
public func ControlDigDouble(pCaller)
{
  [$TxtHeatmetal$|Image=ANML]
  return(ProductionOrder(ANML,pCaller));
}

func Collection(obj) // wenn etwas die fackel betritt
{
    obj->~DoCon(-1);  // damit es beim automatischen buaen nicht rausgenommen wird
    return(_inherited(obj));
}

func Ejection(obj) // wenn etwas die Fackel verlässt
{
    obj->~DoCon(100);
    return(_inherited(obj));
}

// außer brennzeug und metall nichts aufnehmen
protected func RejectCollect(def,pObj) { if(def!=WOOD && def!=METL && def!=COAL) return(1); }

/* Produktion */

public func IsProducerOf(clonk, def) 
{
  return (def == ANML);
}

public func HowToProduce(clonk, def) 
{
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 0, 240);
  ProductionOrder (def, clonk);
  return(1);
}

public func ProductionOrder(id idProduct, object pWorker)
{
  AddCommand(pWorker, "Call", this(), 0, 0, 0, 0, "Acquisition", 0, 3);
  // Material beschaffen
  if (!ActIdle())  ProductionAcquireMaterial(WOOD, 2, pWorker); // holz nur besorgen,wenn esse kein feuer hat
  ProductionAcquireMaterial(METL, 1, pWorker);
  // Brennvorgang beginnt selbsttätig
  return(1);
}

private func ProductionAcquireMaterial(id idMaterial, int iAmount, object pWorker)
{
  // Benötigte Menge ermitteln
  var amount = Max(iAmount - ContentsCount(idMaterial, this()), 0);
  while (amount --)
  {
    AddCommand(pWorker, "Put", this(), 0,0,0, 0, idMaterial);
    AddCommand(pWorker, "Acquire", 0,0,0,this(), 0, idMaterial, 5, 3);
  }
}

public func Acquisition(pWorker)
{
  // Materialbeschaffung erfolgreich
  return(1);
}

public func AcquisitionFailed(pWorker)
{
  // Materialbeschaffung fehlgeschlagen: Meldung ausgeben
  if (!FindContents(METL)) 
  	return(Message("$TxtNometalavailable$", pWorker));
  Message("$TxtNeedsfueltoburn$", pWorker);
  return(1);
}

/* Brennvorgang */

private func CheckContents()
{
  // Noch aktiv
  if (!ActIdle()) return(1);
  if(pLight) {RemoveObject(pLight);}  // Licht weg, wenn idle
  // Neues Feuer mit Kohle
  var coal = FindContents(COAL);
  if (coal)
  {
    RemoveObject(coal);
    SetAction("Burning");
  }
  // Neues Feuer mit Holz
  var holzz = FindContents(WOOD);
  var holz = ContentsCount(WOOD);
  if (holz>=2)
  {
    RemoveObject(holzz);
  var holzz2 = FindContents(WOOD);
    RemoveObject(holzz2);
    SetAction("Burning");
  }
}

private func Burning()
{
  if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight),GetY(pLight)+5);}  // Licht, wenn burning
  if(ContentsCount(METL))
  {SetAction("Melting");counter++;}
  if (counter>=4) {counter=0;SetAction("Idle");}  // kann mit einem Feuer = 2Holz oder 1 Kohle, 4 Metalle schmelzen
  // if(GetActTime() > 1500)  // brennt so lange, bis x metall geschmolzen wurden
    // SetAction("Idle");
}

private func Melting()
{
  if(!pLight) {pLight=CreateObject(LGFL,0,0,-1);pLight->~Init(this,0,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight),GetY(pLight)+5);}  // Licht, wenn burning
  // Noch nicht fertig
  if(GetActTime() < 200) return(1);
  var metl = FindContents(METL);
  if(!metl) return(1);
  RemoveObject(metl);
  // Glühendes Metall auswerfen
  Smoke(0, 15, 15);  
  Exit(CreateContents(ANML), 0, 15, 0, -1);
  CastParticles("PxSpark", 6, 10, 0, 20, 15, 30, RGB(255, 223, 127), RGB(255, 223, 127));
  Sound("Pshshsh");
  if (counter<4) SetAction("Burning");
}

private func Effects()
{
  Smoke(0, 15, 10);
  CastParticles("PxSpark", 3, 10, 0, 20, 15, 30, RGB(255, 223, 127), RGB(255, 223, 127));
}

protected func CalcValue() // Geringer Verkaufswert
{
  if (Contained()) return(25);
  return(150);
}
