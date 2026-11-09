/*-- Amboss --*/

#strict
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
  return(GetCon() >= 100);
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
public func ProductType () { return(C4D_Vehicle()); }
public func ProductCondition () { return(); }

/* Produktion */

public func IsProducerOf (caller, def) {
  if (!GetPlrKnowledge (GetOwner (caller), def)) return (0);
  if (!(GetCategory (0, def) & C4D_Object ())) return (0);
  if (!def->~IsAnvilProduct()) return (0);
  if (!IsBuilt ()) return (0);
  if(!(InArray(gForschungsliste(0), def) || InArray(gForschungsliste(1), def) || InArray(gForschungsliste(2), def) || InArray(gForschungsliste(3), def) || InArray(gForschungsliste(4), def) || InArray(gForschungsliste(5), def) || InArray(gForschungsliste(6), def) || InArray(gForschungsliste(7), def) || InArray(gForschungsliste(8), def) || InArray(gForschungsliste(9), def))) return(0);
  return(1);
}

public func HowToProduce (clonk, def) {
  // Auf Fertigstellung warten
  AddCommand (clonk, "Wait", 0, 0, 0, 0, 0, 168);
  // Und schmieden
  ProductionOrder (def, clonk);
  return(1);
}

public func ProductionMenu(pWorker) {
  // Nur wenn inaktiv
  if (!ActIdle()) return(0);
  // Menu erzeugen
  CreateMenu(CXCN, pWorker, this(), 1, "$TxtNoPlans$");
  // Menü mit Bauplänen füllen
  var i = 0, def; SetVar(0,-1);
  while (def = GetPlrKnowledge(GetOwner(pWorker), 0, i++, C4D_Object()))
    if (def->~IsAnvilProduct())
      
    if(GetLength(hashSpielRegeln))
        {
            var ProdCostAnzeige = 0;
              ProdCostAnzeige = GetValue(0,def) * HashGet(hashSpielRegeln, "Produktionskosten")/200;
            if (HashGet(hashSpielRegeln, "Produktionskosten"))
                AddMenuItem(Format("%d C. %s herstellen",ProdCostAnzeige,GetName(0,def)), "ProductionOrder", def,pWorker,0,pWorker);
            else
                AddMenuItem("%s herstellen","ProductionOrder",def,pWorker,0,pWorker);
        }
      else
        AddMenuItem("%s herstellen","ProductionOrder",def,pWorker,0,pWorker);
  
  // Fertig
  return(1);
}

public func ProductionOrder(idProduct, pWorker) {
  
  ProdCost = 0;
  if(GetLength(hashSpielRegeln))
      ProdCost = GetValue(0,idProduct) * HashGet(hashSpielRegeln, "Produktionskosten")/200;
  // geld pruefen und abziehen
  if(ProdCost)
  {
      if(GetWealth(GetOwner(pWorker))< ProdCost)
        return(Message("Nicht genug Geld",this()),Sound("Error"));
      SetWealth(GetOwner(pWorker),GetWealth(GetOwner(pWorker))- ProdCost);
      Sound("Cash");
  }
  
  // Arbeit beginnen
  AddCommand(pWorker,"Call",this(),idProduct,0,0, 0, "ProductionStart", 0, 3);  
  // Vorher: Position einnehmen
  AddCommand(pWorker,"MoveTo",0,GetX(),GetY());
  // Vorher: Material beschaffen
  ProductionAcquireMaterialFor(idProduct, pWorker);
  // Los geht's
  return(1);
}

private func ProductionAcquireMaterialFor(idProduct, pWorker) {
  // Alle Komponenten des herzustellenden Objekts überprüfen
  var i = 0, def;
  while (def = GetComponent(0, i++, 0, idProduct))
    // Material beschaffen
    ProductionAcquireMaterial(def,GetComponent(def,0,0,idProduct),pWorker);
  // Ok
  return(1);
}

private func ProductionAcquireMaterial(idMaterial, iAmount, pWorker) {
  // Benötigte Menge ermitteln
  if(FindObject(QWER))  // wenn Regel kein Produktionsmaterial aktiviert ist, dann nötige Ressource einfach erzeugen
     CreateContents(idMaterial,this(),iAmount);
  var iNeeded = Max(iAmount - ContentsCount(idMaterial), 0);
  // Der Arbeiter soll das Material beschaffen
  while (iNeeded-- > 0)
  {
    AddCommand(pWorker,"Put",this(),0,0,0, 0, idMaterial);
    AddCommand(pWorker,"Acquire",0,0,0,this(), 0, idMaterial, 5);
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
   AddCommand(pWorker,"Call",this,idProduct,0,0, 0, "ProductionStart", 0, 3);  
   AddCommand(pWorker, "Wait", 0, 0, 0, 0, 0, Max(168 - GetActTime(), 25));
   return 1;
 }

  // Alle Komponenten des Produkts überprüfen
  while((mat = GetComponent(0,i++,0,idProduct)) )
  {
    // Benötigte Komponente ist nicht ausreichend im Container vorhanden
    if (GetComponent(mat,0,0,idProduct) > ContentsCount(mat, this()) )
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
  var i = 0, j = 0, mat;
  // Alle Komponenten des Produkts überprüfen
  while ((mat = GetComponent(0,i++,0,idProduct)) )
  {
    // Benötigte Komponente ist nicht ausreichend im Container vorhanden
    if (GetComponent(mat,0,0,idProduct) > ContentsCount(mat,pMaterialContainer))
    {
       // Menge der benötigten Komponente speichern
      Var(j*2) = GetComponent(mat,0,0,idProduct) - ContentsCount(mat,pMaterialContainer);
     // ID der benötigten Komponente speichern
      Var(j*2+1) = mat;
      // Anzahl der benötigten Materialtypen erhöhen
      j++;
    }
  }
  var str = Format("$TxtNeedMaterial$",GetName(0, idProduct) );
  for(i = 0; i < j; ++ i)
    str = Format("%s|$TxtNeedMaterialComponent$", str, Var(i*2), GetName(0, Var(i*2+1)) );
    
  Message(str, pMessageTarget);
  
  // Fertig
  return(j);
}

private func Forging() {
  // Noch nicht fertig
  var ProdFactor;
  if(GetLength(hashSpielRegeln)) ProdFactor = HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit"); else ProdFactor = 100;
  var time = GetDefCoreVal("Mass","DefCore",product) * ProdFactor / 100;
  var i = -1;
  var j, obj;
  while (obj = GetComponent(0,++i,0,product))
    for (j = 0; j < GetComponent(obj,0,0,product); ++j)
    {
        time += GetDefCoreVal("Mass","DefCore",obj) * ProdFactor / 100; // die Masse der Komponenten für das Produkt wird aufsummiert
    }
  time = time * 10 * 15/10; // mit 10 multipliztieren, da time gegenüber acttime geprüft wird und diese in frames ist. ohne * 10 wäre es also zu schnell
  // und mit 1,5 multiplizieren, damit es 1.5 mal länger dauert, als die Produktion in Raffinerie
    
  if (GetActTime() < time) return(0);
  
  var pProduct = ComposeContents(product);
  // Objekt auswerfen
  if(pProduct){if(!Random(20)) {CreateObject(MTWS,0,0,GetOwner());};
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
  if (GetAction() eq "Forge") SetAction("Idle");
  // Clonk(s) auswerfen
  var clnk;
  while (clnk = FindCrewContents()) Exit(clnk,0,10);
  // Fertig
  return(1);
}

private func FindCrewContents() {
  var i = 0, obj;
  while (obj = Contents(i++))
    if (GetOCF(obj) & OCF_CrewMember())
      return(obj);
  return(0);
}
  
private func Effects() {
  Sound("AnvilWork*");
  CastParticles("PxSpark",3,10,0,0,15,30,RGB(255,223,127),RGB(255,223,127));
  return(1);
}

/* Burgteile */

public func CastleChange() {
  // Brennende Werkstatt im Hintergrund?
  if(!FindObject(CPN3, 0, -1)) return(1);
  // Ausleeren und entfernen!
  return(RemoveObject(0,1));
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
