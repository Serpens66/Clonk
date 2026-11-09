/*-- Fabrik-Objekt --*/

#strict 3

local Factory;                         // Objektzeiger
local Product, Endless;                // Herstellung
local CurrentPT, NextStepPT, FullPT;   // Produktionsfortschritt/-dauer
local ProdFactor, CompMenge,ProdMass;                      // spezielle Produktionsdauer in % des Normalwertes
local ProdCost;
local NeededEnergy; // die Energie, die pro Frame verbraucht wird

private func ProductionEndCall(productID)
{
  // überladen, wenn bei Fertigstellung was anderes passieren soll
  var obj = CreateContents(productID, Factory);
  if (!obj) return(nil);
  Sound("Pshshsh");
  return(1);
}


/*
var fact, fact1,fact2,fact3,fact4,fact5,fact6;
fact=GetID(Factory);
fact1=C_F1;   CHWS ist das Waffensystem
if (fact==fact1) CreateContents(CHWS,Factory);   /Chemiefabrik
if (fact==SOF1) CreateContents(CHWS,Factory);  //Chemielabor
if (fact==SOF2) CreateContents(CHWS,Factory); //Waffenfabrik
if (fact==SOF3) CreateContents(CHWS,Factory); // Maschinenfabrik
if (fact==C_F2) CreateContents(CHWS,Factory); //Raffinerie
if (fact==C_F3) CreateContents(CHWS,Factory);  //Raumschifffabrik (muss in dem script gemacht werden)
*/


/* Initialisierung */

protected func Initialize()
{
  // Produktion zurücksetzen
  Product = nil;
  Endless = nil;
}

public func Init(par_factory)
{
  Factory = par_factory;
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))) // von alter WErkstatt und alter chemiefabrik
    NeededEnergy = 1000; // dieser wert ist absolut im verhältnis zur max energie von 100.000, wenn bei doenergie der dritte parameter wahr ist, geht das. Nur bei GetEnergy gibts sowas nicht, deswehalb muss da dann weiterhin fuer 1 , also 1000 geprüft werden 
  else  // wenn wert hier verändert wird, darauf achten dass die Berechnungen weiter unten davon ausgehen, dass fuer alte gebäude absolut und für neue gebäude prozent verwendet wird
    NeededEnergy = 5; // in Prozent, als 5% von 100.000
} // energieverbrauch pro Frame


/* Steuerung durch die Fabrik */

public func KeyUp(caller)
{
  // einfache Produktion starten
  if (!Product)
    OpenProductionMenu(SYF2,caller);
  // oder bereits begonnene Herstellung fortführen
  else
    ContinueProduction();
}

public func KeyDig(caller)
{
  // Produktionsinfos sofort einblenden
  if (!Product)
    Message("Nichts in Produktion", Factory);
  else
  {
    // erstmal versuchen, Rohstoffe zu holen
    GrabNeededComponents();
    // Anzeigestring aufbauen
    var str = "";
    if (Endless) str = Format("%s (endlos)", GetName(nil,Product));
            else str = Format("%s", GetName(nil,Product));
    if (Equal(NextStepPT, FullPT))
      str = Format("%s|Benötigtes Material vorhanden", str);
    else
    {
      // Auflistung der noch benötigten Materialien
      str = Format("%s|Noch benötigt:", str);
      var i = -1;
      var objID, components, used;
      while (objID = GetComponent(nil,S42MigrationStep(i, 1),nil,Product))
      {
        components = GetComponent(objID,nil,nil,Product);
        used = ContentsCount(objID);
        if ((used ?? 0) < (components ?? 0))
          str = Format("%s|%dx %s", str, (components ?? 0)-(used ?? 0), GetName(nil,objID));
      }
    }
    PlayerMessage(GetOwner(Factory), str, Factory);
  }
  // Auftragsmenü öffnen
  CreateMenu(GetID(Factory),caller,nil,nil,GetName(Factory),nil,1);
  if (Product)
    AddMenuItem("Abbruch","StopProduction",SYF1,caller);
  AddMenuItem("Herstellung...",     "OpenProductionMenu",SYF2,caller,nil,caller);
  AddMenuItem("Endlosproduktion...","OpenProductionMenu",SYF3,caller,nil,caller);
}


/* Produktionsmenü */

private func OpenProductionMenu(symbol, caller)
{
  // bitte überladen!
}

protected func SelectProduction(productID, factor)
{
  // Auswahl getroffen
  StopProduction();
  var owner;
  if (And(factor, Equal(GetType(factor), C4V_C4Object)))
  {
      owner = GetOwner(factor);
      factor = nil;
  }
  else
      owner = GetOwner();
  
  ProdCost = nil;
  if(GetLength(hashSpielRegeln))
      ProdCost = (GetValue(nil,productID) ?? 0) * (HashGet(hashSpielRegeln, "Produktionskosten") ?? 0)/200;
  // geld pruefen und abziehen
  if(ProdCost)
  {
      if((GetWealth(owner) ?? 0)< (ProdCost ?? 0))
        return(S42MigrationFirst(Message("Nicht genug Geld",this()),Sound("Error")));
      SetWealth(owner,(GetWealth(owner) ?? 0)- (ProdCost ?? 0));
      Sound("Cash");
  }
  
  NewProduction(productID, factor);
}


/* Produktion */

private func NewProduction(productID, factor)
{
  // Produktion starten
  Product = productID;
  if (factor) ProdFactor = factor;
         else 
             if(GetLength(hashSpielRegeln)) ProdFactor = HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit"); else ProdFactor = 100;    // Standard: 100%
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))) (ProdFactor ?? 0)*15/10; // in den alten Produktionsgebäuden, dauert das produzieren 1.5 mal länger
  // alles auf Anfang
  CurrentPT = nil;
  NextStepPT = nil;
  FullPT = nil;
  CompMenge = nil;
  ProdMass = nil;
  // Produktionszeit berechnen
  var i = -1;
  var j, obj;
  while (obj = GetComponent(nil,S42MigrationStep(i, 1),nil,Product))
    for (j = nil; (j ?? 0) < (GetComponent(obj,nil,nil,Product) ?? 0); S42MigrationStep(j, 1))
    {   // Wert wird mit 10 multipliziert, weil die Action Delay von 10 auf 1 reduziert wurde, damit noch kuerzere Produktionszeiten moeglich sein (vorher war das niedrigste 10 * componentenmenge Frames)
        S42MigrationAssign(FullPT, "+=", ((GetDefCoreVal("Mass","DefCore",obj) ?? 0) * (ProdFactor ?? 0) / 100) * 10); // die Masse der Komponenten für das Produkt wird aufsummiert... für jede componente ein produktionsschritt. d.h wenn hier änderung, muss auch änderung in schritte gemacht werden
        S42MigrationStep(CompMenge, 1, true); // zaehlen, wieviele componenten wir insg haben
    }
  ProdMass = (((GetDefCoreVal("Mass","DefCore",Product) ?? 0) * (ProdFactor ?? 0) / 100)/(CompMenge ?? 0))*10; // die Masse des Produkts aufteilen auf die Zahl der Componenten (wenn eine deziamlzahl rauskommt wird sie automatisch gerundet)
  for(var a = 1; (a ?? 0)<=(CompMenge ?? 0); S42MigrationStep(a, 1, true))
  {
      S42MigrationAssign(FullPT, "+=", (ProdMass ?? 0));    // die gerundete Masse für jede componente draufrechnen
  }
  if((FullPT ?? 0) < (CompMenge ?? 0)) FullPT = CompMenge; // es darf minimal die Zahl der Komponenten sein, weil sonst die noetigen Komponenten nicht abgefragt werden.
  // Update für Fortschrittsanzeige
  ShowInfoBar(IBRP, Factory, CurrentPT, FullPT, -1);
  // und gleich anfangen
  SetAction("Process");
}

private func ContinueProduction()
{
  // Rohstoffe verarbeiten
  GrabNeededComponents();
  // Produktionspause nötig?
  if (And(((CurrentPT ?? 0) < (FullPT ?? 0)), (Equal(CurrentPT, NextStepPT))))
  {
    // bei Endlos: Produktion noch gar nicht begonnen?
    if (And(!NextStepPT, Endless))
      return(StopProduction());
    // warten und evtl. einen Clonk was besorgen lassen
    SetAction("Wait");
    if (!Endless) AutoAcquire();
    return(1);
  }
  // weiter mit der Herstellung
  if (!!SEqual(GetAction(), "Wait"))
    SetAction("Process");
}

public func StopProduction()
{
  var obj;
  while (obj = Contents())
  {
      if(!FindObject(QWER))  // wenn keine Regel "kein Produktionsmaterial "
      {
          // alles raus hier
        Enter(Factory,obj);
      }
      else // andernfalls muss man davon ausgehen, dass die nötigen Materialien durch regel erstellt wurden, also löschen
      {
          RemoveObject(obj);
      }
  }
  // Produktion beendet
  Product = nil;
  CurrentPT = nil;
  NextStepPT = nil;
  FullPT = nil;
  CompMenge = nil;
  ProdMass = nil;
  // Anzeige ausblenden
  DisappearInfoBar(IBRP, Factory);
  SetAction("Idle");
}

private func GrabNeededComponents()
{
  // noch benötigte Components holen
  var i = -1;
  var obj, objID;
  var components, used;
  while (objID = GetComponent(nil,S42MigrationStep(i, 1),nil,Product))
  {
    components = GetComponent(objID,nil,nil,Product);
    used = ContentsCount(objID);
    // genug davon da?
    while ((used ?? 0) < (components ?? 0))
    {
      if(FindObject(QWER))  // wenn Regel kein Produktionsmaterial aktiviert ist, dann nötige Ressource einfach erzeugen
        CreateContents(objID,Factory,1); //// wenn Produktion abgebrochen wird, muss das so erstellte material gelöscht werden
      // in der Fabrik suchen
      obj = FindContents(objID,Factory);
      if (!obj)  break;    
      // hier herein verschieben
      Enter(this(),obj);
      var addiert = nil;
      S42MigrationAssign(addiert, "+=", ((GetDefCoreVal("Mass","DefCore",objID) ?? 0) * (ProdFactor ?? 0) / 100)*10);
      S42MigrationAssign(addiert, "+=", (ProdMass ?? 0));
      if((addiert ?? 0)<1)
          S42MigrationAssign(NextStepPT, "+=", 1);
      else
      {
          S42MigrationAssign(NextStepPT, "+=", ((GetDefCoreVal("Mass","DefCore",objID) ?? 0) * (ProdFactor ?? 0) / 100)*10);
          S42MigrationAssign(NextStepPT, "+=", (ProdMass ?? 0));
      }
      S42MigrationStep(used, 1);
    }
  }
}

private func FindNeededComponent()
{
  // herausfinden, was noch benötigt wird
  var i = -1;
  var objID;
  while (objID = GetComponent(nil,S42MigrationStep(i, 1),nil,Product))
    if ((ContentsCount(objID) ?? 0) < (GetComponent(objID,nil,nil,Product) ?? 0))
      return(objID);
  return(nil);
}

protected func AutoAcquire()
{
  // Clonk in der Fabrik suchen
  var clonk = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember,"",nil, Factory);
  if (!clonk) return(nil);
  // was wird gebraucht?
  var objID = FindNeededComponent();
  if (!objID) return(nil);
  // Befehle geben
  SetCommand(clonk, "Acquire", nil,nil,nil,nil, objID);             // Rohstoff besorgen
  AppendCommand(clonk, "Put", Factory,nil,nil,nil,1000, objID);   // und in die Fabrik legen
  return(1);
}


/* Action-Calls */

protected func CheckEnergy()
{
  if(FindObject(ENNN)) // wenn Regel, dass Energie angezeigt werden soll
      Message(Format("%v",GetEnergy(Factory)),Factory);
      
  // genug Energie, um weiterzumachen?
  var gebrauchteEnergie = NeededEnergy;
  if(And(GetLength(hashSpielRegeln), !(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))))) // nur bei den zukunftsgebäuden, da alle anderen alten Gebäude das per process usw machen
    gebrauchteEnergie = (((NeededEnergy ?? 0)*(HashGet(hashSpielRegeln, "EnergieverbrauchNeue") ?? 0))/100);
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))) gebrauchteEnergie=(gebrauchteEnergie ?? 0)/1000; // nochmal durch 1000 teilen, weil der Wert fuer alte gebäude 250 ist, und GetEnergy nur die prozentzahlen kennt
  if((gebrauchteEnergie ?? 0)<1) gebrauchteEnergie=1; // mind. 1, (da 0.25 wohl zu 0 gerundet wird) wenn ohne energie sein soll, dann soll die energieregel abgeschaltet werden
  if (And(NeedEnergy(), ((GetEnergy(Factory) ?? 0) < (gebrauchteEnergie ?? 0))))
    return(1);
  // Energie reicht
  SetAction("Process");
}

private func Waiting()
{
  if(FindObject(ENNN)) // wenn Regel, dass Energie angezeigt werden soll
      Message(Format("%v",GetEnergy(Factory)),Factory);
  // Materialien suchen, um weiter produzieren zu können
  GrabNeededComponents();
  // welche gefunden?
  if ((CurrentPT ?? 0) < (NextStepPT ?? 0))
    SetAction("Process");
}

private func Process()
{
  if(FindObject(ENNN)) // wenn Regel, dass Energie angezeigt werden soll
      Message(Format("%v",GetEnergy(Factory)),Factory);
  // Clonk muss bei WRKS und CHEM im Gebäude sein, damit produziert wird
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM)))
  {
      // Clonk in der Fabrik suchen
      var clonk = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember,"",nil, Factory);
      if (!clonk) return(nil);
  }
  // noch genug Energie da?
  var gebrauchteEnergie = NeededEnergy;
  if(And(GetLength(hashSpielRegeln), !(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))))) // nur bei den zukunftsgebäuden, da alle anderen alten Gebäude das per process usw machen
    gebrauchteEnergie = ((NeededEnergy ?? 0)*(HashGet(hashSpielRegeln, "EnergieverbrauchNeue") ?? 0))/100;
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))) gebrauchteEnergie=(gebrauchteEnergie ?? 0)/1000; // nochmal durch 1000 teilen, weil der Wert fuer alte gebäude 250 ist, und GetEnergy nur die prozentzahlen kennt
  if((gebrauchteEnergie ?? 0)<1) gebrauchteEnergie=1; // mind. 1, (da 0.25 wohl zu 0 gerundet wird) wenn ohne energie sein soll, dann soll die energieregel abgeschaltet werden
  if (And(NeedEnergy(), ((GetEnergy(Factory) ?? 0) < (gebrauchteEnergie ?? 0))))
    return(SetAction("CheckEnergy"));
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))) gebrauchteEnergie=NeededEnergy; // wieder eigentlichen Wert von 250 setzen, denn DoEnergie kann mit absolten werten rechnen
  DoEnergy(-(gebrauchteEnergie ?? 0), Factory,1); // mir dritten wert wahr, damit 250 von 100.000 genommen wird
  
  // weiter mit der Herstellung, bis Arbeitsschritt fertig
  if ((CurrentPT ?? 0) < (NextStepPT ?? 0))
  {
    S42MigrationStep(CurrentPT, 1);
    // Update für Fortschrittsanzeige
    ShowInfoBar(IBRP, Factory, CurrentPT,FullPT, nil,1);
    return(DoSmoke());
  }
  
  // nächsten Schritt anfangen
  if ((CurrentPT ?? 0) < (FullPT ?? 0))
    return(ContinueProduction());

  // komplett fertig
  var obj;
  // Aufruf für spezielle Auswertung
  if (!ProductionEndCall(Product)) return(1);
  // Komponenten löschen
  var i = -1;
  var j, objID;
  while (objID = GetComponent(nil,S42MigrationStep(i, 1),nil,Product))
  {
    j = (GetComponent(objID,nil,nil,Product) ?? 0) + 1;
    while (S42MigrationStep(j, -1))
    {
      obj = FindContents(objID);
      if (obj) RemoveObject(obj);
    }
    // gefüllte Fässer: leeres Fass als Ersatz
    if (Or(Or(Or(Or(Or(Equal(objID, WBRL), Equal(objID, SBRL)), Equal(objID, OBRL)),
        Equal(objID, ABRL)), Equal(objID, LBRL)), Equal(objID, DBRL)))
    {
      j = (GetComponent(objID,nil,nil,Product) ?? 0) + 1;
      while (S42MigrationStep(j, -1))
        CreateContents(BARL,Factory);
    }
  }
  // Meldung, dass fertig
  if ((FullPT ?? 0) >= 150*10) // mal 10, weil action delay nun 1 statt 10
    CreateObject(SE07,nil,nil,GetOwner());
  // bei Endlosproduktion: gleich weiter
  if (Endless) NewProduction(Product, ProdFactor);
          else StopProduction();
}



private func DoSmoke()
{
  // auch zum Überladen da
}

func EnoughEnergy() // wird in hauptgebäudeskripten evlt noch überladen, dies zumindest in allen factory objekten entfernen
{
    var gebrauchteEnergie = NeededEnergy;
  if(And(GetLength(hashSpielRegeln), !(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))))) // nur bei den zukunftsgebäuden, da alle anderen alten Gebäude das per process usw machen
    gebrauchteEnergie = ((NeededEnergy ?? 0)*(HashGet(hashSpielRegeln, "EnergieverbrauchNeue") ?? 0))/100;
  if(Or(Equal(GetID(Factory), WRKS), Equal(GetID(Factory), CHEM))) gebrauchteEnergie=(gebrauchteEnergie ?? 0)/100; // nochmal durch 100 teilen, weil der Wert fuer alte gebäude 250 ist, und GetEnergy nur die prozentzahlen kennt
  if((gebrauchteEnergie ?? 0)<1) gebrauchteEnergie=1; // mind. 1, (da 0.25 wohl zu 0 gerundet wird) wenn ohne energie sein soll, dann soll die energieregel abgeschaltet werden
  return((GetEnergy(Factory) ?? 0) >= (gebrauchteEnergie ?? 0));
}

/* Eigenschaften */

private func NeedEnergy(...) { return(FindObject(ENRG));
}

