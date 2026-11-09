/*-- Fabrik-Objekt --*/

#strict

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
  if (!obj) return(0);
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
  Product = 0;
  Endless = 0;
}

public func Init(par_factory)
{
  Factory = par_factory;
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) // von alter WErkstatt und alter chemiefabrik 
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
    if (Endless) str = Format("%s (endlos)", GetName(0,Product));
            else str = Format("%s", GetName(0,Product));
    if (NextStepPT == FullPT)
      str = Format("%s|Benötigtes Material vorhanden", str);
    else
    {
      // Auflistung der noch benötigten Materialien
      str = Format("%s|Noch benötigt:", str);
      var i = -1;
      var objID, components, used;
      while (objID = GetComponent(0,++i,0,Product))
      {
        components = GetComponent(objID,0,0,Product);
        used = ContentsCount(objID);
        if (used < components)
          str = Format("%s|%dx %s", str, components-used, GetName(0,objID));
      }
    }
    PlayerMessage(GetOwner(Factory), str, Factory);
  }
  // Auftragsmenü öffnen
  CreateMenu(GetID(Factory),caller,0,0,GetName(Factory),0,1);
  if (Product)
    AddMenuItem("Abbruch","StopProduction",SYF1,caller);
  AddMenuItem("Herstellung...",     "OpenProductionMenu",SYF2,caller,0,caller);
  AddMenuItem("Endlosproduktion...","OpenProductionMenu",SYF3,caller,0,caller);
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
  if (factor && GetType(factor)==C4V_C4Object())
  {
      owner = GetOwner(factor);
      factor = 0;
  }
  else
      owner = GetOwner();
  
  ProdCost = 0;
  if(GetLength(hashSpielRegeln))
      ProdCost = GetValue(0,productID) * HashGet(hashSpielRegeln, "Produktionskosten")/200;
  // geld pruefen und abziehen
  if(ProdCost)
  {
      if(GetWealth(owner)< ProdCost)
        return(Message("Nicht genug Geld",this()),Sound("Error"));
      SetWealth(owner,GetWealth(owner)- ProdCost);
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
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) ProdFactor*15/10; // in den alten Produktionsgebäuden, dauert das produzieren 1.5 mal länger
  // alles auf Anfang
  CurrentPT = 0;
  NextStepPT = 0;
  FullPT = 0;
  CompMenge = 0;
  ProdMass = 0;
  // Produktionszeit berechnen
  var i = -1;
  var j, obj;
  while (obj = GetComponent(0,++i,0,Product))
    for (j = 0; j < GetComponent(obj,0,0,Product); ++j)
    {   // Wert wird mit 10 multipliziert, weil die Action Delay von 10 auf 1 reduziert wurde, damit noch kuerzere Produktionszeiten moeglich sein (vorher war das niedrigste 10 * componentenmenge Frames)
        FullPT += (GetDefCoreVal("Mass","DefCore",obj) * ProdFactor / 100) * 10; // die Masse der Komponenten für das Produkt wird aufsummiert... für jede componente ein produktionsschritt. d.h wenn hier änderung, muss auch änderung in schritte gemacht werden
        CompMenge++; // zaehlen, wieviele componenten wir insg haben
    }
  ProdMass = ((GetDefCoreVal("Mass","DefCore",Product) * ProdFactor / 100)/CompMenge)*10; // die Masse des Produkts aufteilen auf die Zahl der Componenten (wenn eine deziamlzahl rauskommt wird sie automatisch gerundet)
  for(var a = 1; a<=CompMenge; a++)
  {
      FullPT += ProdMass;    // die gerundete Masse für jede componente draufrechnen 
  }
  if(FullPT < CompMenge) FullPT = CompMenge; // es darf minimal die Zahl der Komponenten sein, weil sonst die noetigen Komponenten nicht abgefragt werden.
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
  if ((CurrentPT < FullPT) && (CurrentPT == NextStepPT))
  {
    // bei Endlos: Produktion noch gar nicht begonnen?
    if (!NextStepPT && Endless)
      return(StopProduction());
    // warten und evtl. einen Clonk was besorgen lassen
    SetAction("Wait");
    if (!Endless) AutoAcquire();
    return(1);
  }
  // weiter mit der Herstellung
  if (GetAction() eq "Wait")
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
  Product = 0;
  CurrentPT = 0;
  NextStepPT = 0;
  FullPT = 0;
  CompMenge = 0;
  ProdMass = 0;
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
  while (objID = GetComponent(0,++i,0,Product))
  {
    components = GetComponent(objID,0,0,Product);
    used = ContentsCount(objID);
    // genug davon da?
    while (used < components)
    {
      if(FindObject(QWER))  // wenn Regel kein Produktionsmaterial aktiviert ist, dann nötige Ressource einfach erzeugen
        CreateContents(objID,Factory,1); //// wenn Produktion abgebrochen wird, muss das so erstellte material gelöscht werden
      // in der Fabrik suchen
      obj = FindContents(objID,Factory);
      if (!obj)  break;    
      // hier herein verschieben
      Enter(this(),obj);
      var addiert = 0;
      addiert += (GetDefCoreVal("Mass","DefCore",objID) * ProdFactor / 100)*10;
      addiert += ProdMass;
      if(addiert<1) 
          NextStepPT += 1;
      else
      {
          NextStepPT += (GetDefCoreVal("Mass","DefCore",objID) * ProdFactor / 100)*10;
          NextStepPT += ProdMass;
      }
      ++used;
    }
  }
}

private func FindNeededComponent()
{
  // herausfinden, was noch benötigt wird
  var i = -1;
  var objID;
  while (objID = GetComponent(0,++i,0,Product))
    if (ContentsCount(objID) < GetComponent(objID,0,0,Product))
      return(objID);
  return(0);
}

protected func AutoAcquire()
{
  // Clonk in der Fabrik suchen
  var clonk = FindObject(0, 0,0,0,0, OCF_CrewMember(),"",0, Factory);
  if (!clonk) return(0);
  // was wird gebraucht?
  var objID = FindNeededComponent();
  if (!objID) return(0);
  // Befehle geben
  SetCommand(clonk, "Acquire", 0,0,0,0, objID);             // Rohstoff besorgen
  AppendCommand(clonk, "Put", Factory,0,0,0,1000, objID);   // und in die Fabrik legen
  return(1);
}


/* Action-Calls */

protected func CheckEnergy()
{
  if(FindObject(ENNN)) // wenn Regel, dass Energie angezeigt werden soll
      Message(Format("%v",GetEnergy(Factory)),Factory);
      
  // genug Energie, um weiterzumachen?
  var gebrauchteEnergie = NeededEnergy;
  if(GetLength(hashSpielRegeln) && !(GetID(Factory)==WRKS || GetID(Factory)==CHEM)) // nur bei den zukunftsgebäuden, da alle anderen alten Gebäude das per process usw machen
    gebrauchteEnergie = ((NeededEnergy*HashGet(hashSpielRegeln, "EnergieverbrauchNeue"))/100); 
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) gebrauchteEnergie=gebrauchteEnergie/1000; // nochmal durch 1000 teilen, weil der Wert fuer alte gebäude 250 ist, und GetEnergy nur die prozentzahlen kennt
  if(gebrauchteEnergie<1) gebrauchteEnergie=1; // mind. 1, (da 0.25 wohl zu 0 gerundet wird) wenn ohne energie sein soll, dann soll die energieregel abgeschaltet werden
  if (NeedEnergy() && (GetEnergy(Factory) < gebrauchteEnergie))
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
  if (CurrentPT < NextStepPT)
    SetAction("Process");
}

private func Process()
{
  if(FindObject(ENNN)) // wenn Regel, dass Energie angezeigt werden soll
      Message(Format("%v",GetEnergy(Factory)),Factory);
  // Clonk muss bei WRKS und CHEM im Gebäude sein, damit produziert wird
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) 
  {
      // Clonk in der Fabrik suchen
      var clonk = FindObject(0, 0,0,0,0, OCF_CrewMember(),"",0, Factory);
      if (!clonk) return(0);
  }
  // noch genug Energie da?
  var gebrauchteEnergie = NeededEnergy;
  if(GetLength(hashSpielRegeln) && !(GetID(Factory)==WRKS || GetID(Factory)==CHEM)) // nur bei den zukunftsgebäuden, da alle anderen alten Gebäude das per process usw machen
    gebrauchteEnergie = (NeededEnergy*HashGet(hashSpielRegeln, "EnergieverbrauchNeue"))/100;
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) gebrauchteEnergie=gebrauchteEnergie/1000; // nochmal durch 1000 teilen, weil der Wert fuer alte gebäude 250 ist, und GetEnergy nur die prozentzahlen kennt
  if(gebrauchteEnergie<1) gebrauchteEnergie=1; // mind. 1, (da 0.25 wohl zu 0 gerundet wird) wenn ohne energie sein soll, dann soll die energieregel abgeschaltet werden
  if (NeedEnergy() && (GetEnergy(Factory) < gebrauchteEnergie))
    return(SetAction("CheckEnergy"));
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) gebrauchteEnergie=NeededEnergy; // wieder eigentlichen Wert von 250 setzen, denn DoEnergie kann mit absolten werten rechnen
  DoEnergy(-gebrauchteEnergie, Factory,1); // mir dritten wert wahr, damit 250 von 100.000 genommen wird
  
  // weiter mit der Herstellung, bis Arbeitsschritt fertig
  if (CurrentPT < NextStepPT)
  {
    ++CurrentPT;
    // Update für Fortschrittsanzeige
    ShowInfoBar(IBRP, Factory, CurrentPT,FullPT, 0,1);
    return(DoSmoke());
  }
  
  // nächsten Schritt anfangen
  if (CurrentPT < FullPT)
    return(ContinueProduction());

  // komplett fertig
  var obj;
  // Aufruf für spezielle Auswertung
  if (!ProductionEndCall(Product)) return(1);
  // Komponenten löschen
  var i = -1;
  var j, objID;
  while (objID = GetComponent(0,++i,0,Product))
  {
    j = GetComponent(objID,0,0,Product) + 1;
    while (--j)
    {
      obj = FindContents(objID);
      if (obj) RemoveObject(obj);
    }
    // gefüllte Fässer: leeres Fass als Ersatz
    if (objID==WBRL || objID==SBRL || objID==OBRL ||
        objID==ABRL || objID==LBRL || objID==DBRL)
    {
      j = GetComponent(objID,0,0,Product) + 1;
      while (--j)
        CreateContents(BARL,Factory);
    }
  }
  // Meldung, dass fertig
  if (FullPT >= 150*10) // mal 10, weil action delay nun 1 statt 10
    CreateObject(SE07,0,0,GetOwner()); 
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
  if(GetLength(hashSpielRegeln) && !(GetID(Factory)==WRKS || GetID(Factory)==CHEM)) // nur bei den zukunftsgebäuden, da alle anderen alten Gebäude das per process usw machen
    gebrauchteEnergie = (NeededEnergy*HashGet(hashSpielRegeln, "EnergieverbrauchNeue"))/100;
  if(GetID(Factory)==WRKS || GetID(Factory)==CHEM) gebrauchteEnergie=gebrauchteEnergie/100; // nochmal durch 100 teilen, weil der Wert fuer alte gebäude 250 ist, und GetEnergy nur die prozentzahlen kennt
  if(gebrauchteEnergie<1) gebrauchteEnergie=1; // mind. 1, (da 0.25 wohl zu 0 gerundet wird) wenn ohne energie sein soll, dann soll die energieregel abgeschaltet werden
  return(GetEnergy(Factory) >= gebrauchteEnergie);
}

/* Eigenschaften */

private NeedEnergy: return(FindObject(ENRG));

