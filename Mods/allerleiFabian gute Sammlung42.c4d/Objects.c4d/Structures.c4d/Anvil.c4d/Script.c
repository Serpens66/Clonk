/*-- Amboss --*/

#strict 3
#include BAS1

local product;
local ProdCost;

public func IsWaste(){ return(10);}

/* Steuerung */

public func ContextProduction(pCaller)
{
  [$TxtProduce$|Image=CXCN|Condition=IsBuilt]
  return(ProductionMenu(pCaller));
}

protected func IsBuilt() {
  return((GetCon() ?? 0) >= 100);
}
  
public func ControlDigDouble(pCaller)
{
  [$TxtProduce$|Image=CXCN]
  return(ProductionMenu(pCaller));
}

public func ContainedDown()
{
  [$TxtCancelproduction$|Image=WKS2]
  ProductionCancel();
  return(1);
}

public func ContainedLeft()
{
  return(1);
}

/* Produktion */

/* Produkteigenschaften (überladbar) */
public func ProductType () { return(C4D_Vehicle); }
public func ProductCondition () { return; }

/* Produktion */

public func IsProducerOf (caller, def) {
  if (!GetPlrKnowledge (GetOwner (caller), def)) return (nil);
  if (!((GetCategory (nil, def) ?? 0) & (C4D_Object  ?? 0))) return (nil);
  if (!def->~IsAnvilProduct()) return (nil);
  if (!IsBuilt ()) return (nil);
  if(!(Or(Or(Or(Or(Or(Or(Or(Or(Or(InArray(gForschungsliste(nil), def), InArray(gForschungsliste(1), def)), InArray(gForschungsliste(2), def)), InArray(gForschungsliste(3), def)), InArray(gForschungsliste(4), def)), InArray(gForschungsliste(5), def)), InArray(gForschungsliste(6), def)), InArray(gForschungsliste(7), def)), InArray(gForschungsliste(8), def)), InArray(gForschungsliste(9), def)))) return(nil);
  return(1);
}

public func HowToProduce (clonk, def) {
  // Auf Fertigstellung warten
  AddCommand (clonk, "Wait", nil, nil, nil, nil, nil, 168);
  // Und schmieden
  ProductionOrder (def, clonk);
  return(1);
}

public func ProductionMenu(pWorker) {
  // Nur wenn inaktiv
  if (!ActIdle()) return(nil);
  // Menu erzeugen
  CreateMenu(CXCN, pWorker, this(), 1, "$TxtNoPlans$");
  // Menü mit Bauplänen füllen
  var i = nil, def; SetVar(nil,-1);
  while (def = GetPlrKnowledge(GetOwner(pWorker), nil, S42MigrationStep(i, 1, true), C4D_Object))
    if (def->~IsAnvilProduct())
      
    if(GetLength(hashSpielRegeln))
        {
            var ProdCostAnzeige = nil;
              ProdCostAnzeige = (GetValue(nil,def) ?? 0) * (HashGet(hashSpielRegeln, "Produktionskosten") ?? 0)/200;
            if (HashGet(hashSpielRegeln, "Produktionskosten"))
                AddMenuItem(Format("%d C. %s herstellen",ProdCostAnzeige,GetName(nil,def)), "ProductionOrder", def,pWorker,nil,pWorker);
            else
                AddMenuItem("%s herstellen","ProductionOrder",def,pWorker,nil,pWorker);
        }
      else
        AddMenuItem("%s herstellen","ProductionOrder",def,pWorker,nil,pWorker);
  
  // Fertig
  return(1);
}

public func ProductionOrder(idProduct, pWorker) {
  
  ProdCost = nil;
  if(GetLength(hashSpielRegeln))
      ProdCost = (GetValue(nil,idProduct) ?? 0) * (HashGet(hashSpielRegeln, "Produktionskosten") ?? 0)/200;
  // geld pruefen und abziehen
  if(ProdCost)
  {
      if((GetWealth(GetOwner(pWorker)) ?? 0)< (ProdCost ?? 0))
        return(S42MigrationFirst(Message("Nicht genug Geld",this()),Sound("Error")));
      SetWealth(GetOwner(pWorker),(GetWealth(GetOwner(pWorker)) ?? 0)- (ProdCost ?? 0));
      Sound("Cash");
  }
  
  // Arbeit beginnen
  AddCommand(pWorker,"Call",this(),idProduct,nil,nil, nil, "ProductionStart", nil, 3);
  // Vorher: Position einnehmen
  AddCommand(pWorker,"MoveTo",nil,GetX(),GetY());
  // Vorher: Material beschaffen
  ProductionAcquireMaterialFor(idProduct, pWorker);
  // Los geht's
  return(1);
}

private func ProductionAcquireMaterialFor(idProduct, pWorker) {
  // Alle Komponenten des herzustellenden Objekts überprüfen
  var i = nil, def;
  while (def = GetComponent(nil, S42MigrationStep(i, 1, true), nil, idProduct))
    // Material beschaffen
    ProductionAcquireMaterial(def,GetComponent(def,nil,nil,idProduct),pWorker);
  // Ok
  return(1);
}

private func ProductionAcquireMaterial(idMaterial, iAmount, pWorker) {
  // Benötigte Menge ermitteln
  if(FindObject(QWER))  // wenn Regel kein Produktionsmaterial aktiviert ist, dann nötige Ressource einfach erzeugen
     CreateContents(idMaterial,this(),iAmount);
  var iNeeded = Max((iAmount ?? 0) - (ContentsCount(idMaterial) ?? 0), nil);
  // Der Arbeiter soll das Material beschaffen
  while (S42MigrationStep(iNeeded, -1, true) > 0)
  {
    AddCommand(pWorker,"Put",this(),nil,nil,nil, nil, idMaterial);
    AddCommand(pWorker,"Acquire",nil,nil,nil,this(), nil, idMaterial, 5);
  }
  return(1);
}

public func ProductionStart(pWorker, idProduct)
{
  var i, mat;
  
  // Nur wenn inaktiv
  if (!ActIdle())
  {
   // Später nochmal probieren wenn Amboss bereits belegt
   AddCommand(pWorker,"Call",this,idProduct,nil,nil, nil, "ProductionStart", nil, 3);
   AddCommand(pWorker, "Wait", nil, nil, nil, nil, nil, Max(168 - (GetActTime() ?? 0), 25));
   return 1;
 }

  // Alle Komponenten des Produkts überprüfen
  while((mat = GetComponent(nil,S42MigrationStep(i, 1, true),nil,idProduct)) )
  {
    // Benötigte Komponente ist nicht ausreichend im Container vorhanden
    if ((GetComponent(mat,nil,nil,idProduct) ?? 0) > (ContentsCount(mat, this()) ?? 0) )
    {
      // Fehler
      return(Sound("Error") );
    }
  }
  
  // Zielprodukt speichern
  product = idProduct;
  // Clonk einsaugen
  Enter(this(),pWorker);
  // Besitzer setzen für Farbe des animierten Clonk
  SetOwner(GetOwner(pWorker));
  // Aktion starten
  SetAction("Forge");
  // Weiter geht's
  return(1);    
}

public func ProductionStartFailed(pWorker, idProduct) {
  // Produktionsbefehle fehlgeschlagen
  // Wir nehmen an, dass der Arbeiter benötigtes Material nicht beschaffen konnte
  MessageBuildNeedsMaterial(pWorker,idProduct,this());
  return(1);
}

private func MessageBuildNeedsMaterial(pMessageTarget, idProduct, pMaterialContainer) {
  // Für das Produkt benötigtes Material überprüfen
  var i = nil, j = nil, mat;
  // Alle Komponenten des Produkts überprüfen
  while ((mat = GetComponent(nil,S42MigrationStep(i, 1, true),nil,idProduct)) )
  {
    // Benötigte Komponente ist nicht ausreichend im Container vorhanden
    if ((GetComponent(mat,nil,nil,idProduct) ?? 0) > (ContentsCount(mat,pMaterialContainer) ?? 0))
    {
       // Menge der benötigten Komponente speichern
      Var((j ?? 0)*2) = (GetComponent(mat,nil,nil,idProduct) ?? 0) - (ContentsCount(mat,pMaterialContainer) ?? 0);
     // ID der benötigten Komponente speichern
      Var((j ?? 0)*2+1) = mat;
      // Anzahl der benötigten Materialtypen erhöhen
      S42MigrationStep(j, 1, true);
    }
  }
  var str = Format("$TxtNeedMaterial$",GetName(nil, idProduct) );
  for(i = nil; (i ?? 0) < (j ?? 0); S42MigrationStep( i, 1))
    str = Format("%s|$TxtNeedMaterialComponent$", str, Var((i ?? 0)*2), GetName(nil, Var((i ?? 0)*2+1)) );
    
  Message(str, pMessageTarget);
  
  // Fertig
  return(j);
}

private func Forging() {
  // Noch nicht fertig
  var ProdFactor;
  if(GetLength(hashSpielRegeln)) ProdFactor = HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit"); else ProdFactor = 100;
  var time = (GetDefCoreVal("Mass","DefCore",product) ?? 0) * (ProdFactor ?? 0) / 100;
  var i = -1;
  var j, obj;
  while (obj = GetComponent(nil,S42MigrationStep(i, 1),nil,product))
    for (j = nil; (j ?? 0) < (GetComponent(obj,nil,nil,product) ?? 0); S42MigrationStep(j, 1))
    {
        S42MigrationAssign(time, "+=", (GetDefCoreVal("Mass","DefCore",obj) ?? 0) * (ProdFactor ?? 0) / 100); // die Masse der Komponenten für das Produkt wird aufsummiert
    }
  time = (time ?? 0) * 10 * 15/10; // mit 10 multipliztieren, da time gegenüber acttime geprüft wird und diese in frames ist. ohne * 10 wäre es also zu schnell
  // und mit 1,5 multiplizieren, damit es 1.5 mal länger dauert, als die Produktion in Raffinerie
    
  if ((GetActTime() ?? 0) < (time ?? 0)) return(nil);
  
  var pProduct = ComposeContents(product);
  // Objekt auswerfen
  if(pProduct){if(!Random(20)) {CreateObject(MTWS,nil,nil,GetOwner());};
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
    Exit(pProduct);}
  // Aktion beenden und Clonk auswerfen
  ProductionCancel();
  return(1);
}

private func ProductionCancel() {
  // Arbeit abbrechen
  if (!!SEqual(GetAction(), "Forge")) SetAction("Idle");
  // Clonk(s) auswerfen
  var clnk;
  while (clnk = FindCrewContents()) Exit(clnk,nil,10);
  // Fertig
  return(1);
}

private func FindCrewContents() {
  var i = nil, obj;
  while (obj = Contents(S42MigrationStep(i, 1, true)))
    if ((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0))
      return(obj);
  return(nil);
}
  
private func Effects() {
  Sound("AnvilWork*");
  CastParticles("PxSpark",3,10,nil,nil,15,30,RGB(255,223,127),RGB(255,223,127));
  return(1);
}

/* Burgteile */

public func CastleChange() {
  // Brennende Werkstatt im Hintergrund?
  if(!FindObject(CPN3, nil, -1)) return(1);
  // Ausleeren und entfernen!
  return(RemoveObject(nil,1));
}

protected func CalcValue() { // Geringer Verkaufswert
  if (Contained()) return(25);
  return(50);
}


/* CastleCheck */

public func CastleChange() {
  if (!FindObject(CPN3,1)) return (1);
  while (Contents()) Exit(Contents());
  Split2Components();
  return(1);
}
