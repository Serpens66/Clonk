/*-- Forshcungslabor-Fabrikobjekt --*/

#strict 3
#include FCOB

local target;
local progress;
local progressanzeige; // der Progress in Prozent
local daueranzeige;

local restime, kosten,kosten2, restime2,restimevorrechnung,restimevorrechnung2, kostenvorrechnung, kostenvorrechnung2; // Benötigte Gesamtzeit zum Forschen, kosten: die ksotenanzeige im Menü, kosten2: die tatsächlichen kosten des nun zu erforschenden Objekts, die Gesamtzeit des objekts für die anzeige

func KeyDig()
{
if(Equal(daueranzeige, nil)) {daueranzeige=1;ShowTemp(); return;}
if(Equal(daueranzeige, 1)) {daueranzeige=nil;ShowTemp(); return;}
}

//TimerCall
func ShowTemp()
{
if(Equal(daueranzeige, nil)) return;
var temperat;
temperat = (GetTemperature() ?? 0) / 2;  // geteilt durch 2 weil -50 bis 50°C realisterische Werte sind
Message(Format("Temperatur:%d°C Umweltverschmutzung/ Umweltbelastbarkeit: |%d/%d",temperat,globalpollutionanzeige, globalnature),Factory);
}


public func IsWaste(){ return(10);}

/* Produktionsmenü */

protected func ResearchObjects(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Object)); }
protected func ResearchVehicles(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Vehicle)); }
protected func ResearchStructures(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Structure)); }

private func OpenProductionMenu(symbol, clonk)
{ 
  // Hauptmenü erzeugen
  CreateMenu(RSR1, clonk, this());
  if (Product) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", Product, clonk, nil, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchObjects", RSR3, clonk, nil, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchVehicles", RSR4, clonk, nil, clonk);
  AddMenuItem("$TxtResearchs$", "ResearchStructures", RSR5, clonk, nil, clonk);
}

protected func SelectResearchCategory(idImage, pClonk, dwCategory)
{
  // Auswahlmenü erzeugen
  CreateMenu(idImage, pClonk, this(), nil, "$TxtNothingelsetoresearc$",nil,1);
  // Alle geladenen Baupläne überprüfen
  var i,idDef,iPlr=GetOwner(pClonk);
  while (idDef = GetDefinition(S42MigrationStep(i, 1, true), dwCategory))
    // Passende Kategorie
    if ((GetCategory(nil, idDef) ?? 0) & (C4D_Knowledge ?? 0))
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
      {
        // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden

//für anzeige
  kostenvorrechnung2 = DefinitionCall(idDef, "GetResearchCosts", this(), pClonk);
  if(GetLength(hashSpielRegeln))
    kosten=(kostenvorrechnung2 ?? 0)*(HashGet(hashSpielRegeln, "Forschungskosten") ?? 0)/10;
  else 
    {kosten=kostenvorrechnung2;}

  restimevorrechnung2 = DefinitionCall(idDef, "GetResearchTime", this(), pClonk);
    if(GetLength(hashSpielRegeln))
       restime2=(restimevorrechnung2 ?? 0)*(HashGet(hashSpielRegeln, "Forschungszeit") ?? 0)/10;
    else {restime2=restimevorrechnung2;}
    var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
    fHasBase = Or(!idBase, GetPlrKnowledge(iPlr,idBase)); j=1;
    // Erweiterte Forschungsgrundlagen
    while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", S42MigrationStep(j, 1))))
      if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=nil;
    var BaseList = DefinitionCall(idDef, "GetResearchBaseList"); // eine liste
    if (BaseList)
      for(var idBase in BaseList)  
          if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=nil;
    // Ist schon durch etwas anderes erforschbar? (Zugbrücke rechts -> Zugbrücke links etc.)
    var link;
    if (link = DefinitionCall(idDef, "GetResearchLink"))
      if (GetName(nil, link)) // Linkdefinition auch tatsächlich geladen?
        if(!GetPlrKnowledge(iPlr, link) ) // Linkdefinition noch nicht erforscht? (passiert durch SetPlrKnowledge-Aufrufe oder Szenariovorgaben)
          continue;
    // Menüeintrag hinzufügen. Die Ausnahmen sind nur dazu da, damit eine zusatzinfo erscheint, dass die Forschung keine Pflicht ist, um andere Forschungen freizuschalten
     //   if (fHasBase) AddMenuItem("$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk);
  var freiwilligliste = gForschungfreiwilligliste();
  var dabei = nil;
  for(var element in freiwilligliste)
  {
      if (Equal(idDef, element))
          {if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen, freiwillig!",GetName(nil,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, nil, pClonk); dabei = 1;}
  }
if (Equal(dabei, nil)){
if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen",GetName(nil,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, nil, pClonk);
      }}  
  return(1);  
}



private func SelectResearch(productID, clonk)
{
   if(Equal(productID, Product)) return(ContinueProduction());
   // Spieler hat diesen Bauplan schon?
    if (GetPlrKnowledge(clonk->GetOwner(), productID))
    {
      Message("$TxtAlreadyDeveloped$", this(), GetName(nil, productID));
      Sound("Error");
      return(nil);
    }
   
   kostenvorrechnung = DefinitionCall(productID, "GetResearchCosts", this(), clonk);
   if(GetLength(hashSpielRegeln))
        kosten2=(kostenvorrechnung ?? 0)*(HashGet(hashSpielRegeln, "Forschungskosten") ?? 0)/10;
   else{kosten2=kostenvorrechnung;}

    // wenn noch nie angefangen, dann prüfen ob genug Geld
    if (!AccHasResearch(productID, GetOwner()))
    {
      if((GetWealth(GetOwner()) ?? 0)< (kosten2 ?? 0))
        return(S42MigrationFirst(Message("$NotEnoughMoney$",this()),Sound("Error")));
    } 
    
  // Produktion starten
  Product = productID;
  if(GetLength(hashSpielRegeln)) ProdFactor = HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit"); else ProdFactor = 100;    // Standard: 100%
  // alles auf Anfang
  CurrentPT = nil;
  NextStepPT = nil;
  FullPT = nil;
  FullPT = DefinitionCall(Product, "GetResearchTime");
  if(GetLength(hashSpielRegeln)) FullPT = (FullPT ?? 0) * (HashGet(hashSpielRegeln, "Forschungszeit") ?? 0)/10;
  
  // Update für Fortschrittsanzeige
  ShowInfoBar(IBRP, Factory, CurrentPT, FullPT, -1);
  
  // wenn noch nie vorher angefangen, dann Geld abziehen und zur Liste der angefangenen Projekte hinzufügen
if (!AccHasResearch(Product, GetOwner()))
{
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)- (kosten2 ?? 0));
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
      while (def = GetDefinition(S42MigrationStep(def_num, 1, true)))
      {
        var link;
        if (link = DefinitionCall(def, "GetResearchLink"))
          if (Equal(link, productID))
            SetPlrKnowledge(GetOwner(), def);
      }
      // Nachricht ausgeben
      Message("$Txtsdeveloped$", this(), GetName(nil, productID));
    
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
  if (!!SEqual(GetAction(), "Wait"))
    SetAction("Process");
}

private func Process()
{
  // noch genug Energie da?
  if (And(NeedEnergy(), ((GetEnergy(Factory) ?? 0) < 25)))
    return(SetAction("CheckEnergy"));
  DoEnergy(-10, Factory);
  // weiter mit der Herstellung
  if ((CurrentPT ?? 0) < (FullPT ?? 0))
  {
    S42MigrationStep(CurrentPT, 1);
    // Update für Fortschrittsanzeige
    ShowInfoBar(IBRP, Factory, CurrentPT,FullPT, nil,1);
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
  
  if(Equal(idType, RSR3)) category = C4D_Object;
  if(Equal(idType, RSR4)) category = C4D_Vehicle;
  if(Equal(idType, RSR5)) category = C4D_Structure;
  
  // Alle geladenen Definitionen überprüfen
  var i = -1, id;
  while (id = GetDefinition(S42MigrationStep(i, 1),category) )
    // Ist ein Bauplan
    if((GetCategory(nil, id) ?? 0) & (C4D_Knowledge ?? 0) )
      // Dem Spieler noch nicht bekannt
      if(!GetPlrKnowledge(GetOwner(pWorker), id) )
      {
        // Grundlage bekannt
        var base = DefinitionCall(id, "GetResearchBase");
        if(Or(!base, GetPlrKnowledge(GetOwner(pWorker), base)))
          return(id);
      }

  // Zur Zeit kein weiteres Ziel
  return(nil);
}

protected func Waiting()
{
  if ((CurrentPT ?? 0) < (FullPT ?? 0))
    SetAction("Process");
}


/* Raucherzeugung */

private func DoSmoke()
{
  if (!Random(2)) Smoke(24,-39, 5+(Random(6) ?? 0));
  Sound("Research");
  return(1);
}


/* Eigenschaften */

// private func NeedEnergy() { return(0); }

