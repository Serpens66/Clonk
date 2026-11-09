/*-- Esse --*/

#strict 3

#include BAS1

local counter,pLight;

func Initialize()
{
    counter=nil;
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
protected func RejectCollect(def,pObj) { if(And(And(!Equal(def, WOOD), !Equal(def, METL)), !Equal(def, COAL))) return(1); }

/* Produktion */

public func IsProducerOf(clonk, def) 
{
  return (Equal(def, ANML));
}

public func HowToProduce(clonk, def) 
{
  AddCommand (clonk, "Wait", nil, nil, nil, nil, nil, 240);
  ProductionOrder (def, clonk);
  return(1);
}

public func ProductionOrder(id idProduct, object pWorker)
{
  AddCommand(pWorker, "Call", this(), nil, nil, nil, nil, "Acquisition", nil, 3);
  // Material beschaffen
  if (!ActIdle())  ProductionAcquireMaterial(WOOD, 2, pWorker); // holz nur besorgen,wenn esse kein feuer hat
  ProductionAcquireMaterial(METL, 1, pWorker);
  // Brennvorgang beginnt selbsttätig
  return(1);
}

private func ProductionAcquireMaterial(id idMaterial, int iAmount, object pWorker)
{
  // Benötigte Menge ermitteln
  iAmount ??= 0;
  var amount = Max((iAmount ?? 0) - (ContentsCount(idMaterial, this()) ?? 0), nil);
  while (S42MigrationStep(amount , -1, true))
  {
    AddCommand(pWorker, "Put", this(), nil,nil,nil, nil, idMaterial);
    AddCommand(pWorker, "Acquire", nil,nil,nil,this(), nil, idMaterial, 5, 3);
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
  if ((holz ?? 0)>=2)
  {
    RemoveObject(holzz);
  var holzz2 = FindContents(WOOD);
    RemoveObject(holzz2);
    SetAction("Burning");
  }
}

private func Burning()
{
  if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight),(GetY(pLight) ?? 0)+5);}  // Licht, wenn burning
  if(ContentsCount(METL))
  {SetAction("Melting");S42MigrationStep(counter, 1, true);}
  if ((counter ?? 0)>=4) {counter=nil;SetAction("Idle");}  // kann mit einem Feuer = 2Holz oder 1 Kohle, 4 Metalle schmelzen
  // if(GetActTime() > 1500)  // brennt so lange, bis x metall geschmolzen wurden
    // SetAction("Idle");
}

private func Melting()
{
  if(!pLight) {pLight=CreateObject(LGFL,nil,nil,-1);pLight->~Init(this,nil,-6,300,RGB(75,60,10),true);pLight->SetPosition(GetX(pLight),(GetY(pLight) ?? 0)+5);}  // Licht, wenn burning
  // Noch nicht fertig
  if((GetActTime() ?? 0) < 200) return(1);
  var metl = FindContents(METL);
  if(!metl) return(1);
  RemoveObject(metl);
  // Glühendes Metall auswerfen
  Smoke(nil, 15, 15);
  Exit(CreateContents(ANML), nil, 15, nil, -1);
  CastParticles("PxSpark", 6, 10, nil, 20, 15, 30, RGB(255, 223, 127), RGB(255, 223, 127));
  Sound("Pshshsh");
  if ((counter ?? 0)<4) SetAction("Burning");
}

private func Effects()
{
  Smoke(nil, 15, 10);
  CastParticles("PxSpark", 3, 10, nil, 20, 15, 30, RGB(255, 223, 127), RGB(255, 223, 127));
}

protected func CalcValue() // Geringer Verkaufswert
{
  if (Contained()) return(25);
  return(150);
}
