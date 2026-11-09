/*-- Forschungslabor --*/

#strict 3

#include CXEC
#include BAS5
#include DOOR

local target;
local progress;
local progressanzeige; // der Progress in Prozent
local daueranzeige;

local restime, kosten,kosten2, restime2,restimevorrechnung,restimevorrechnung2, kostenvorrechnung, kostenvorrechnung2; // Benötigte Gesamtzeit zum Forschen, kosten: die ksotenanzeige im Menü, kosten2: die tatsächlichen kosten des nun zu erforschenden Objekts, die Gesamtzeit des objekts für die anzeige

public func IsWaste(){ return(20);}


func ContainedDig()
{
if(Equal(daueranzeige, nil)) {daueranzeige=1;ShowTemp(); return;}
if(Equal(daueranzeige, 1)) {daueranzeige=nil;ShowTemp(); return;}
}

//TimerCall
func ShowTemp()
{

if(Equal(daueranzeige, nil))
{
    if(FindObject(ENNN))
        Message(Format("%v",GetEnergy()),this());return;
}
progressanzeige=((progress ?? 0)*100)/(restime ?? 0);
var temperat;
temperat = (GetTemperature() ?? 0) / 2;  // geteilt durch 2 weil -50 bis 50°C realisterische Werte sind
if((globalnature ?? 0)>=99999) // dann kann man davon ausgehen, dass ohne umweltzeugs gespielt werden soll, also braucht man darüber auch keine info
    Message(Format("Fortschritt: %d% Temperatur:%d°C",progressanzeige,temperat),this());
else
    Message(Format("Fortschritt: %d% Temperatur:%d°C Umweltverschmutzung/ Umweltbelastbarkeit: |%d/%d           ",progressanzeige,temperat,globalpollutionanzeige, globalnature),this());

}


/* Tür */

private func SoundOpenDoor()
{
  Sound("SteelGate2");
}

private func SoundCloseDoor()
{
  Sound("SteelGate2");
}
  
/* Forschung */

protected func ResearchObjects(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Object)); }
protected func ResearchVehicles(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Vehicle)); }
protected func ResearchStructures(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Structure)); }

public func ResearchMenu(clonk)
{
  // Hauptmenü erzeugen
  CreateMenu(RSR1, clonk, this());
  if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, clonk, nil, clonk);
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
      // nur wenn in Forschungsbaum 0 bis 9 . danach gibt es neues forschungslabor
      if(Or(Or(Or(Or(Or(Or(Or(Or(Or(InArray(gForschungsliste(nil), idDef), InArray(gForschungsliste(1), idDef)), InArray(gForschungsliste(2), idDef)), InArray(gForschungsliste(3), idDef)), InArray(gForschungsliste(4), idDef)), InArray(gForschungsliste(5), idDef)), InArray(gForschungsliste(6), idDef)), InArray(gForschungsliste(7), idDef)), InArray(gForschungsliste(8), idDef)), InArray(gForschungsliste(9), idDef)))
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
      {
        // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden

//für anzeige
  kostenvorrechnung2 = DefinitionCall(idDef, "GetResearchCosts",this(),pClonk);
  if(GetLength(hashSpielRegeln))
    kosten=(kostenvorrechnung2 ?? 0)*(HashGet(hashSpielRegeln, "Forschungskosten") ?? 0)/10;
  else 
    {kosten=kostenvorrechnung2;}

  restimevorrechnung2 = DefinitionCall(idDef, "GetResearchTime",this(),pClonk);
  if(GetLength(hashSpielRegeln))
    restime2=(restimevorrechnung2 ?? 0)*(HashGet(hashSpielRegeln, "Forschungszeit") ?? 0)/10;
  else {restime2=restimevorrechnung2;}
  if(!restime2) restime2=1; // da es mit zeit=0 zu fehlern kommt
        var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
        fHasBase = Or(!idBase, GetPlrKnowledge(iPlr,idBase)); j=1;
        // Erweiterte Forschungsgrundlagen
        while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", S42MigrationStep(j, 1))))
          if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=nil;
        if(!FindObject(SZRE)) // nur, wenn nicht der szenario forschungsbaum verwendet werden soll, meinen forschungsbaum abfragen
        {
            var BaseList = DefinitionCall(idDef, "GetResearchBaseList"); // eine liste
            if (BaseList)
              for(var idBase in BaseList)  
                  if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=nil;
        }
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
  if(!FindObject(SZRE)) // nur die freiwilligen markieren, wenn keine szenario forschung aktiv, denn bei szenario forschungs bits keine forschungsbäume und daher auch nichts freiwilliges
      for(var element in freiwilligliste)
      {
          if (Equal(idDef, element))
              {if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen, freiwillig!",GetName(nil,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, nil, pClonk); dabei = 1;}
      }
  // if(idDef==HATR || idDef==CTW3  || idDef==CTW5  || idDef==CTW7  || idDef==ACT1  || idDef==ACT3  || idDef==LICN || idDef==CS4K  || idDef==PS4K  || idDef==BS4K  || idDef==LS4K  || idDef==SS4K  || idDef==WS4K  ||  idDef==GTE1  || idDef==KC4K   || idDef==_BAO   || idDef==_BAL   || idDef==GU01   || idDef==GU02   || idDef==GU03   || idDef==GU04   || idDef==GU05   || idDef==GU06   || idDef==GU07   || idDef==GU08   || idDef==GU09   || idDef==GU10   || idDef==GU11   || idDef==GU12   || idDef==GU13   || idDef==GU14   || idDef==GU15   || idDef==GU16   || idDef==GU17   || idDef==GU18   || idDef==GU19   || idDef==GU20   || idDef==ERIT   || idDef==_BA2 )
    // {if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit)erforschen, freiwillig!",GetName(0,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, 0, pClonk);}

// else{
if (Equal(dabei, nil)){
if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen",GetName(nil,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, nil, pClonk);
      }}  
  return(1);  
}

public func SelectResearch(def, clonk)
{
  kostenvorrechnung = DefinitionCall(def, "GetResearchCosts", this(), clonk);
  if(GetLength(hashSpielRegeln))
  kosten2=(kostenvorrechnung ?? 0)*(HashGet(hashSpielRegeln, "Forschungskosten") ?? 0)/10;
  else{kosten2=kostenvorrechnung;}

// wenn noch nie angefangen, dann prüfen ob genug Geld
if (!AccHasResearch(def, GetOwner()))
{
  if((GetWealth(GetOwner()) ?? 0)< (kosten2 ?? 0))
    return(S42MigrationFirst(Message("$NotEnoughMoney$",this()),Sound("Error")));
}


  // Anfangen
  AddCommand(clonk, "Call", this(), def, nil, nil, nil, "StartResearch", 10, 1);
  // Vorher ins Labor gehen
  if (!Equal(Contained(clonk), this()))
  {
    AddCommand(clonk, "Wait", nil, nil, nil, nil, 40);
    AddCommand(clonk, "Enter", this());
  } 
}
  
private func CancelResearch()
{
  if (Equal(GetChar(GetAction()), GetChar("R"))) SetAction("Idle");
  EnergyCheck(nil);
}

protected func StartResearch(clonk, def)
{
  // Spieler hat diesen Bauplan schon
  if (GetPlrKnowledge(clonk->GetOwner(), def))
  {
    Message("$TxtAlreadyDeveloped$", this(), GetName(nil, def));
    Sound("Error");
    return(nil);
  }
  // Bisherige Forschung abbrechen
  CancelResearch();
  // nur, wenn ansonsten inaktiv oder schon am Forschen
  if (!ActIdle()) if (!Equal(GetChar(GetAction()), GetChar("R"))) return(nil);
  // Besitzer anpassen (der Besitzer des forschenden Clonks erhält den Bauplan)
  SetOwner(GetOwner(clonk));
  // Neues Forschungsziel: Fortschritt zurücksetzen
  if (!Equal(def, target)) progress = nil;
  // Forschungsziel und Gesamtzeit setzen
  restimevorrechnung = DefinitionCall(target=def, "GetResearchTime", this(), clonk);
  if(GetLength(hashSpielRegeln))
  restime=(restimevorrechnung ?? 0)*(HashGet(hashSpielRegeln, "Forschungszeit") ?? 0)/10;
  else{restime=restimevorrechnung;}
  if(!restime) restime=1; // da es mit zeit=0 zu fehlern kommt

  // Forschungsaktion starten

// wenn noch nie vorher angefangen, dann Geld abziehen und zur Liste der angefangenen Projekte hinzufügen
if (!AccHasResearch(def, GetOwner()))
{
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)- (kosten2 ?? 0));
  Sound("Cash");
AddToResearchAcc(def, GetOwner());
}

  SetAction("Research");
  return (1);
}

private func Researching()
{
  // Kein Ziel
  if (!target) return(nil);
  // Effekte
  if (Random(3)) Smoke(5+8*(Random(2) ?? 0),-23,3+(Random(8) ?? 0));
  // Fortschritt
  var Process = Min(S42MigrationStep(progress, 1)*4 / (restime ?? 0), 3);
  Process = Format("Research%d", Process);
  if (!SEqual(Process, GetAction())) SetAction(Process);
  // Forschung abbrechen, wenn irgendein Clonk im Inneren das Gebäude verlassen möchte
  var pObj;
  while (pObj = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember, nil,nil, this(), pObj))
    if (!!SEqual(pObj->GetCommand(), "Exit"))
      return(CancelResearch());
  // Weitermachen
  if (!restime) restime=100;
  if ((progress ?? 0) < (restime ?? 0)) return(1);
  // Forschung abgeschlossen
  Sound("ResearchDone");
var verbrannt;
verbrannt= CreateObject(VBWS);
verbrannt->Verbrannt(GetID(),IsWaste());
  // Dem Spieler den neuen Bauplan verpassen
  SetPlrKnowledge(GetOwner(), target);

// wenn gebäude wohnungseinheit erforscht, wird hier auch Hausmüll produziert
// if(GetPlrKnowledge(GetOwner(),SDWU)) {if(!Random(7)) CreateContents(HMWS,this());};    // neues forschungsgebäude fuer neue forschung

  // Alle Clonks suchen, die ein Menü für dieses Labor geöffnet haben - noch nicht implementierbar, da kein GetMenuTarget()...
  /*var pClonk;
  while (pClonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, 0, pClonk))
    if ((pClonk->GetMenu() == RSR1) && (pClonk->GetMenuTarget() == this())
      // Menü neu öffnen, damit Forschung-Fortsetzen-Einträge aktualisiert werden
      ResearchMenu(pClonk);*/  

  // Gelinkte Forschungsziele sind automatisch auch erforscht
  // (Linke Zugbrücke -> Rechte Zugbrücke etc.)
  var def_num, def;
  while (def = GetDefinition(S42MigrationStep(def_num, 1, true)))
  {
    var link;
    if (link = DefinitionCall(def, "GetResearchLink"))
      if (Equal(link, target))
        SetPlrKnowledge(GetOwner(), def);
  }
  // Nachricht ausgeben
  Message("$Txtsdeveloped$", this(), GetName(nil, target));
  // Forschung beenden
  CancelResearch();
  target = nil;
  // Fertig
  return(1);  
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

/* Kontext */

public func ContextResearch(clonk) 
{
  [$TxtResearch$|Image=RSR1|Condition=IsBuilt]
  return (ResearchMenu(clonk));
}

protected func IsBuilt() 
{
  return ((GetCon () ?? 0) >= 100);
}
    
/* Steuerung */

public func ContainedUp(object pClonk)
{
  [$TxtResearch$|Image=RSR1]
  ResearchMenu(pClonk);
  return(1);
}

public func ContainedDown(object pClonk)
{
  [$TxtCancelresearch$|Image=RSR2]
  CancelResearch();
  return(nil);
}
