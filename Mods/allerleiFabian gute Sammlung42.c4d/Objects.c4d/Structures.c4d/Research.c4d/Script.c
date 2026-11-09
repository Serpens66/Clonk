/*-- Forschungslabor --*/

#strict

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
if(daueranzeige==0) {daueranzeige=1;ShowTemp(); return();}
if(daueranzeige==1) {daueranzeige=0;ShowTemp(); return();}
}

//TimerCall
func ShowTemp()
{

if(daueranzeige==0) 
{
    if(FindObject(ENNN))
        Message(Format("%v",GetEnergy()),this());return();
}
progressanzeige=(progress*100)/restime;
var temperat;
temperat = GetTemperature() / 2;  // geteilt durch 2 weil -50 bis 50°C realisterische Werte sind
if(globalnature>=99999) // dann kann man davon ausgehen, dass ohne umweltzeugs gespielt werden soll, also braucht man darüber auch keine info
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

protected func ResearchObjects(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Object())); }
protected func ResearchVehicles(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Vehicle())); }
protected func ResearchStructures(idImage, pClonk) { return(SelectResearchCategory(idImage, pClonk, C4D_Structure())); }

public func ResearchMenu(clonk)
{
  // Hauptmenü erzeugen
  CreateMenu(RSR1, clonk, this());
  if (target) AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", target, clonk, 0, clonk);
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
      // nur wenn in Forschungsbaum 0 bis 9 . danach gibt es neues forschungslabor
      if(InArray(gForschungsliste(0), idDef) || InArray(gForschungsliste(1), idDef) || InArray(gForschungsliste(2), idDef) || InArray(gForschungsliste(3), idDef) || InArray(gForschungsliste(4), idDef) || InArray(gForschungsliste(5), idDef) || InArray(gForschungsliste(6), idDef) || InArray(gForschungsliste(7), idDef) || InArray(gForschungsliste(8), idDef) || InArray(gForschungsliste(9), idDef))
      // Dem Spieler noch nicht bekannt
      if (!GetPlrKnowledge(iPlr,idDef))
      {
        // Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden

//für anzeige
  kostenvorrechnung2 = DefinitionCall(idDef, "GetResearchCosts",this(),pClonk);
  if(GetLength(hashSpielRegeln))
    kosten=kostenvorrechnung2*HashGet(hashSpielRegeln, "Forschungskosten")/10;
  else 
    {kosten=kostenvorrechnung2;}

  restimevorrechnung2 = DefinitionCall(idDef, "GetResearchTime",this(),pClonk);
  if(GetLength(hashSpielRegeln))
    restime2=restimevorrechnung2*HashGet(hashSpielRegeln, "Forschungszeit")/10;
  else {restime2=restimevorrechnung2;}
  if(!restime2) restime2=1; // da es mit zeit=0 zu fehlern kommt
        var idBase = DefinitionCall(idDef,"GetResearchBase"), fHasBase, j;
        fHasBase = !idBase || GetPlrKnowledge(iPlr,idBase); j=1;
        // Erweiterte Forschungsgrundlagen
        while (idBase = DefinitionCall(idDef, Format("GetResearchBase%d", ++j)))
          if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
        if(!FindObject(SZRE)) // nur, wenn nicht der szenario forschungsbaum verwendet werden soll, meinen forschungsbaum abfragen
        {
            var BaseList = DefinitionCall(idDef, "GetResearchBaseList"); // eine liste
            if (BaseList)
              for(var idBase in BaseList)  
                  if (!GetPlrKnowledge(iPlr,idBase)) fHasBase=0;
        }
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
  if(!FindObject(SZRE)) // nur die freiwilligen markieren, wenn keine szenario forschung aktiv, denn bei szenario forschungs bits keine forschungsbäume und daher auch nichts freiwilliges
      for(var element in freiwilligliste)
      {
          if (idDef==element)
              {if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen, freiwillig!",GetName(0,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, 0, pClonk); dabei = 1;}
      }
  // if(idDef==HATR || idDef==CTW3  || idDef==CTW5  || idDef==CTW7  || idDef==ACT1  || idDef==ACT3  || idDef==LICN || idDef==CS4K  || idDef==PS4K  || idDef==BS4K  || idDef==LS4K  || idDef==SS4K  || idDef==WS4K  ||  idDef==GTE1  || idDef==KC4K   || idDef==_BAO   || idDef==_BAL   || idDef==GU01   || idDef==GU02   || idDef==GU03   || idDef==GU04   || idDef==GU05   || idDef==GU06   || idDef==GU07   || idDef==GU08   || idDef==GU09   || idDef==GU10   || idDef==GU11   || idDef==GU12   || idDef==GU13   || idDef==GU14   || idDef==GU15   || idDef==GU16   || idDef==GU17   || idDef==GU18   || idDef==GU19   || idDef==GU20   || idDef==ERIT   || idDef==_BA2 )
    // {if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit)erforschen, freiwillig!",GetName(0,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, 0, pClonk);}

// else{
if (dabei==0){
if (fHasBase) AddMenuItem(Format("%s (%d C. %d Zeit) erforschen",GetName(0,idDef),kosten,restime2),"SelectResearch",idDef, pClonk, 0, pClonk);
      }}  
  return(1);  
}

public func SelectResearch(def, clonk)
{
  kostenvorrechnung = DefinitionCall(def, "GetResearchCosts", this(), clonk);
  if(GetLength(hashSpielRegeln))
  kosten2=kostenvorrechnung*HashGet(hashSpielRegeln, "Forschungskosten")/10;
  else{kosten2=kostenvorrechnung;}

// wenn noch nie angefangen, dann prüfen ob genug Geld
if (!AccHasResearch(def, GetOwner()))
{
  if(GetWealth(GetOwner())< kosten2)
    return(Message("$NotEnoughMoney$",this()),Sound("Error"));
}


  // Anfangen
  AddCommand(clonk, "Call", this(), def, 0, 0, 0, "StartResearch", 10, 1);
  // Vorher ins Labor gehen
  if (Contained(clonk) != this())
  {
    AddCommand(clonk, "Wait", 0, 0, 0, 0, 40);
    AddCommand(clonk, "Enter", this());
  } 
}
  
private func CancelResearch()
{
  if (GetChar(GetAction()) == GetChar("R")) SetAction("Idle");
  EnergyCheck(0);
}

protected func StartResearch(clonk, def)
{
  // Spieler hat diesen Bauplan schon
  if (GetPlrKnowledge(clonk->GetOwner(), def))
  {
    Message("$TxtAlreadyDeveloped$", this(), GetName(0, def));
    Sound("Error");
    return(0); 
  }
  // Bisherige Forschung abbrechen
  CancelResearch();
  // nur, wenn ansonsten inaktiv oder schon am Forschen
  if (!ActIdle()) if (GetChar(GetAction()) != GetChar("R")) return(0);
  // Besitzer anpassen (der Besitzer des forschenden Clonks erhält den Bauplan)
  SetOwner(GetOwner(clonk));
  // Neues Forschungsziel: Fortschritt zurücksetzen
  if (def != target) progress = 0;
  // Forschungsziel und Gesamtzeit setzen
  restimevorrechnung = DefinitionCall(target=def, "GetResearchTime", this(), clonk);
  if(GetLength(hashSpielRegeln))
  restime=restimevorrechnung*HashGet(hashSpielRegeln, "Forschungszeit")/10;
  else{restime=restimevorrechnung;}
  if(!restime) restime=1; // da es mit zeit=0 zu fehlern kommt

  // Forschungsaktion starten

// wenn noch nie vorher angefangen, dann Geld abziehen und zur Liste der angefangenen Projekte hinzufügen
if (!AccHasResearch(def, GetOwner()))
{
  SetWealth(GetOwner(),GetWealth(GetOwner())- kosten2);
  Sound("Cash");
AddToResearchAcc(def, GetOwner());
}

  SetAction("Research");
  return (1);
}

private func Researching()
{
  // Kein Ziel
  if (!target) return(0);
  // Effekte
  if (Random(3)) Smoke(5+8*Random(2),-23,3+Random(8));
  // Fortschritt
  var Process = Min(++progress*4 / restime, 3);
  Process = Format("Research%d", Process);
  if (Process ne GetAction()) SetAction(Process);
  // Forschung abbrechen, wenn irgendein Clonk im Inneren das Gebäude verlassen möchte
  var pObj;
  while (pObj = FindObject(0, 0,0,0,0, OCF_CrewMember, 0,0, this(), pObj))
    if (pObj->GetCommand() eq "Exit")
      return(CancelResearch());
  // Weitermachen
  if (!restime) restime=100;
  if (progress < restime) return(1);
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
  while (def = GetDefinition(def_num++)) 
  {
    var link;
    if (link = DefinitionCall(def, "GetResearchLink"))
      if (link == target)
        SetPlrKnowledge(GetOwner(), def);
  }
  // Nachricht ausgeben
  Message("$Txtsdeveloped$", this(), GetName(0, target));
  // Forschung beenden
  CancelResearch();
  target = 0;
  // Fertig
  return(1);  
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

/* Kontext */

public func ContextResearch(clonk) 
{
  [$TxtResearch$|Image=RSR1|Condition=IsBuilt]
  return (ResearchMenu(clonk));
}

protected func IsBuilt() 
{
  return (GetCon () >= 100);
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
  return(0);
}
