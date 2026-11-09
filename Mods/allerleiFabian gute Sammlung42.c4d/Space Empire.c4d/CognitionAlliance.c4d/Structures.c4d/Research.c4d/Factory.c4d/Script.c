/*-- Forshcungslabor-Fabrikobjekt --*/

#strict
#include FCOB

local target;
local progress;
local progressanzeige; // der Progress in Prozent
local daueranzeige;

local restime, kosten,kosten2, restime2,restimevorrechnung,restimevorrechnung2, kostenvorrechnung, kostenvorrechnung2; // Benötigte Gesamtzeit zum Forschen, kosten: die ksotenanzeige im Menü, kosten2: die tatsächlichen kosten des nun zu erforschenden Objekts, die Gesamtzeit des objekts für die anzeige

func KeyDig()
{
if(daueranzeige==0) {daueranzeige=1;ShowTemp(); return();}
if(daueranzeige==1) {daueranzeige=0;ShowTemp(); return();}
}

//TimerCall
func ShowTemp()
{
if(daueranzeige==0) return();
var temperat;
temperat = GetTemperature() / 2;  // geteilt durch 2 weil -50 bis 50°C realisterische Werte sind
Message(Format("Temperatur:%d°C Umweltverschmutzung/ Umweltbelastbarkeit: |%d/%d",temperat,globalpollutionanzeige, globalnature),Factory);
}


public func IsWaste(){ return(10);}

/* Produktionsmenü */

protected func ResearchObjects(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Object())); }
protected func ResearchVehicles(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Vehicle())); }
protected func ResearchStructures(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Structure())); }

private func OpenProductionMenu(symbol, clonk)
{ 
  // Hauptmenü erzeugen
  CreateMenu(RSR1, clonk, this());
  if (Product) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", Product, clonk, 0, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchObjects", RSR3, clonk, 0, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchVehicles", RSR4, clonk, 0, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchStructures", RSR5, clonk, 0, clonk);
}

protected func SelectResearchCategory(idImage, pClonk, dwCategory)
{
  // Auswahlmenü erzeugen
  CreateMenu(idImage, pClonk, this(), 0, "$TxtNothingelsetoresearc$",0,1);
  // Alle geladenen Baupläne überprüfen
  var i,idDef,iPlr=GetOwner(pClonk);
  while (idDef = GetDefinition(i++, dwCategory))
    // Passende Kategorie
    if (GetCategory(0, idDef) & C4D_Knowledge())
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
      {
        // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden

//für anzeige
  kostenvorrechnung2 = DefinitionCall(idDef, "GetResearchCosts", this(), pClonk);
  if(GetLength(hashSpielRegeln))
    kosten=kostenvorrechnung2*HashGet(hashSpielRegeln, "Forschungskosten")/10;
  else 
    {kosten=kostenvorrechnung2;}

  restimevorrechnung2 = DefinitionCall(idDef, "GetResearchTime", this(), pClonk);
    if(GetLength(hashSpielRegeln))
       restime2=restimevorrechnung2*HashGet(hashSpielRegeln, "Forschungszeit")/10;
    else {restime2=restimevorrechnung2;}
    var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
    fHasBase = !idBase || GetPlrKnowledge(iPlr,idBase); j=1;
    // Erweiterte Forschungsgrundlagen
    while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", ++j)))
      if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
    var BaseList = DefinitionCall(idDef, "GetResearchBaseList"); // eine liste
    if (BaseList)
      for(var idBase in BaseList)  
          if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
    // Ist schon durch etwas anderes erforschbar? (Zugbrücke rechts -> Zugbrücke links etc.)
    var link;
    if (link = DefinitionCall(idDef, "GetResearchLink"))
      if (GetName(0, link)) // Linkdefinition auch tatsächlich geladen?
        if(!GetPlrKnowledge(iPlr, link) ) // Linkdefinition noch nicht erforscht? (passiert durch SetPlrKnowledge-Aufrufe oder Szenariovorgaben)
          continue;
    // Menüeintrag hinzufügen. Die Ausnahmen sind nur dazu da, damit eine zusatzinfo erscheint, dass die Forschung keine Pflicht ist, um andere Forschungen freizuschalten
     //   if (fHasBase) AddMenuItem("$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk);
  var freiwilligliste = gForschungfreiwilligliste();
  var dabei = 0; 
  for(var element in freiwilligliste)
  {
      if (idDef==element)
          {if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen, freiwillig!",GetName(0,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, 0, pClonk); dabei = 1;}
  }
if (dabei==0){
if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen",GetName(0,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, 0, pClonk);
      }}  
  return(1);  
}



private func SelectResearch(productID, clonk)
{
   if(productID==Product) return(ContinueProduction());
   // Spieler hat diesen Bauplan schon?
    if (GetPlrKnowledge(clonk->GetOwner(), productID))
    {
      Message("$TxtAlreadyDeveloped$", this(), GetName(0, productID));
      Sound("Error");
      return(0); 
    }
   
   kostenvorrechnung = DefinitionCall(productID, "GetResearchCosts", this(), clonk);
   if(GetLength(hashSpielRegeln))
        kosten2=kostenvorrechnung*HashGet(hashSpielRegeln, "Forschungskosten")/10;
   else{kosten2=kostenvorrechnung;}

    // wenn noch nie angefangen, dann prüfen ob genug Geld
    if (!AccHasResearch(productID, GetOwner()))
    {
      if(GetWealth(GetOwner())< kosten2)
        return(Message("$NotEnoughMoney$",this()),Sound("Error"));
    } 
    
  // Produktion starten
  Product = productID;
  if(GetLength(hashSpielRegeln)) ProdFactor = HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit"); else ProdFactor = 100;    // Standard: 100%
  // alles auf Anfang
  CurrentPT = 0;
  NextStepPT = 0;
  FullPT = 0;
  FullPT = DefinitionCall(Product, "GetResearchTime");
  if(GetLength(hashSpielRegeln)) FullPT = FullPT * HashGet(hashSpielRegeln, "Forschungszeit")/10;
  
  // Update für Fortschrittsanzeige
  ShowInfoBar(IBRP, Factory, CurrentPT, FullPT, -1);
  
  // wenn noch nie vorher angefangen, dann Geld abziehen und zur Liste der angefangenen Projekte hinzufügen
if (!AccHasResearch(Product, GetOwner()))
{
  SetWealth(GetOwner(),GetWealth(GetOwner())- kosten2);
  Sound("Cash");
  AddToResearchAcc(Product, GetOwner());
}
  
  // und gleich anfangen
  SetAction("Process");
}

private func ProductionEndCall(productID)
{
    // Forschung abgeschlossen
    Sound("ResearchDone");
    SetPlrKnowledge(GetOwner(), productID);
      // Gelinkte Forschungsziele sind automatisch auch erforscht
      // (Linke Zugbrücke -> Rechte Zugbrücke etc.)
      var def_num, def;
      while (def = GetDefinition(def_num++)) 
      {
        var link;
        if (link = DefinitionCall(def, "GetResearchLink"))
          if (link == productID)
            SetPlrKnowledge(GetOwner(), def);
      }
      // Nachricht ausgeben
      Message("$Txtsdeveloped$", this(), GetName(0, productID));
    
    if(!Random(7)) CreateContents(HMWS,Factory);
    var verbrannt;
    verbrannt= CreateObject(VBWS);
    verbrannt->Verbrannt(GetID(),IsWaste());
  return(1);
}



public func KeyUp(caller)
{
    OpenProductionMenu(SYF2,caller);
}

private func ContinueProduction()
{
  // weiter mit der Herstellung
  if (GetAction() eq "Wait")
    SetAction("Process");
}

private func Process()
{
  // noch genug Energie da?
  if (NeedEnergy() && (GetEnergy(Factory) < 25))
    return(SetAction("CheckEnergy"));
  DoEnergy(-10, Factory);
  // weiter mit der Herstellung
  if (CurrentPT < FullPT)
  {
    ++CurrentPT;
    // Update für Fortschrittsanzeige
    ShowInfoBar(IBRP, Factory, CurrentPT,FullPT, 0,1);
    return(DoSmoke());
  }
  // komplett fertig
  ProductionEndCall(Product);
  StopProduction();
}




private func GetResearchGoal(id idType, object pWorker)
{
  // Kategorie
  var category;
  
  if(idType == RSR3) category = C4D_Object();
  if(idType == RSR4) category = C4D_Vehicle();
  if(idType == RSR5) category = C4D_Structure();
  
  // Alle geladenen Definitionen überprüfen
  var i = -1, id;
  while (id = GetDefinition(++i,category) )
    // Ist ein Bauplan
    if(GetCategory(0, id) & C4D_Knowledge() )
      // Dem Spieler noch nicht bekannt
      if(!GetPlrKnowledge(GetOwner(pWorker), id) )
      {
        // Grundlage bekannt
        var base = DefinitionCall(id, "GetResearchBase");
        if(!base || GetPlrKnowledge(GetOwner(pWorker), base))
          return(id);
      }

  // Zur Zeit kein weiteres Ziel
  return(0);
}

protected func Waiting()
{
  if (CurrentPT < FullPT)
    SetAction("Process");
}


/* Raucherzeugung */

private func DoSmoke()
{
  if (!Random(2)) Smoke(24,-39, 5+Random(6));
  Sound("Research");
  return(1);
}


/* Eigenschaften */

// private func NeedEnergy() { return(0); }

