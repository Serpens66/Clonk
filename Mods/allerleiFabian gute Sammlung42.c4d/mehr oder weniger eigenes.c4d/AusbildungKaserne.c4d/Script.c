/*-- Ausbildungszentrale Kaserne --*/

#strict 3

//sollte fertig sein

#include B116
#include DOOR

local derclonk, timer,aktiveausbildung;
local stkaufkosten0, stkaufkosten1,stkaufkosten2,stkaufkosten3,stkaufkosten4;
local stkosten1,stkosten2,stkosten3, stkosten4,stkosten5;
local sterfahrung1, sterfahrung2, sterfahrung3,sterfahrung4,sterfahrung5;
local stzeitd1,stzeitd2,stzeitd3,stzeitd4,stzeitd5;
local kosten1,kosten2,kosten3, kosten4,kosten5;
local erfahrung1, erfahrung2, erfahrung3,erfahrung4,erfahrung5;
local zeitd1,zeitd2,zeitd3,zeitd4,zeitd5;

//nur clonks aufnehmen
protected func RejectCollect(def,pObj) { if(!S42MigrationEqual(def, CLNK) && !S42MigrationEqual(def, HZCK)) return(1); }

/* Steuerung */

func Collection2(object pClonk) {			
//nur ein Clonk gleichzeitig, damit "derclonk" richtig gesetzt ist
_inherited(pClonk);
	SetEntrance(nil);

//wenn mehr als 1 Clonk drin ist, rausschmeißen
  while ((ObjectCount2(Find_OCF(OCF_CrewMember),Find_Container(this)) ?? 0)>=2)
      {
  var doorX = (GetDefCoreVal("Entrance","DefCore",nil,nil) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,2) ?? 0) / 2;
  var doorY = (GetDefCoreVal("Entrance","DefCore",nil,1) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,3) ?? 0);
var clonk = FindObject2(Find_OCF(OCF_CrewMember));
Exit(clonk,doorX,doorY);}
}

func Ejection(pClonk) {
  _inherited (pClonk) ;
 SetEntrance(1);
//wenn mehr als 1 Clonk drin ist, rausschmeißen
  while ((ObjectCount2(Find_OCF(OCF_CrewMember),Find_Container(this)) ?? 0)>=2)
      {
  var doorX = (GetDefCoreVal("Entrance","DefCore",nil,nil) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,2) ?? 0) / 2;
  var doorY = (GetDefCoreVal("Entrance","DefCore",nil,1) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,3) ?? 0);
var clonk = FindObject2(Find_OCF(OCF_CrewMember));
Exit(clonk,doorX,doorY);}
}

//TimerCall
func BewohnerCheck()
{
//falls jemand während ausbildung ohne den ausgang zu nutzen rausgeht (z.b weil er stirbt), dann eingang wieder frei machen
  if (S42MigrationEqual(ObjectCount2(Find_OCF(OCF_CrewMember),Find_Container(this)), nil))
      {aktiveausbildung=nil;if(S42MigrationEqual(zuu, nil)) SetEntrance(1);}

if(S42MigrationEqual(zuu, 1)) SetEntrance(nil);


// setzen der lokalen Variablen, also der Standardwerte für Kosten und Erfahrung für die einzelnen Stufen.  Für die Zeitdauer ist es etwas komplizierter, da diese recht umständlich über aktionen gemacht ist und zwar genau so, dass es nach einem bruchteil der Zeit auch nur ein Bruchteil der neuen Physics gibt. die zeitd-Werte sind also lediglich die nötige Framezahl der jweiligen timer. Der Delay in der action ist aber auch wichtig. Dennoch reicht es, wenn man den zeitd-Wert halbiert oderso, nur müssen dann halt auch die Physicwerte angepasst werden, was bei einer verdopplung der zeitd nicht ohe aufwendige anpassung möglich ist.
// Wenn man diese Standardwerte ändern möchte, so muss dies nur hier getan werden und nicht in den Stufenfunktionen. Nur bei der Zeit muss in den Stufenfunktionen der +Physicswert angepasst werden.


//stufe 6-10, 200 pro stufe (also insg. 700 für stufe 6, bzw. 1500 für stufe 10) und 100Gold.
// stkosten3 und sterfahrung3 werden aus den variablen Einstellungen ausgelesen, wenn vorhanden. Aus diesen ergeben sich dann alle anderen Standardwerte
// Für die Zeit bleibt erstmal alles beim Alten. Es werden keine Regelobjekte mehr für erfahrung und kosten erstellt, sind also immer 0. Aber für Zeit wird es weiterhin erstellt.
if(GetLength(hashSpielRegeln)) {if(HashGet(hashSpielRegeln, "Ausbildungskosten")) stkosten3=HashGet(hashSpielRegeln, "Ausbildungskosten");}
else {stkosten3= 100;}
if(GetLength(hashSpielRegeln)) {if(HashGet(hashSpielRegeln, "Ausbildungserfahrung")) sterfahrung3=HashGet(hashSpielRegeln, "Ausbildungserfahrung");}
else {sterfahrung3=200;}
stzeitd3=1000;    // 5000 Frames


// Stufe 1-4, 100Erfahrung und 20 Gold pro stufe
stkosten1= (stkosten3 ?? 0)/5; // 20
sterfahrung1= (sterfahrung3 ?? 0)/2;
stzeitd1= 500;  // also 3000 Frames

// stufe 5 insg. 500 erfahrung und 50 gold für die Stufe
stkosten2= (stkosten3 ?? 0)/2;  // 50
sterfahrung2= (sterfahrung3 ?? 0)/2; // 100 zusätzlich für diese stufe
stzeitd2=500;  // also 3000Frames

// stufe 11-15 300 exp pro weitere stufe und 300 gold
stkosten4= (stkosten3 ?? 0)*3; // 300
sterfahrung4= ((sterfahrung3 ?? 0)*6)/2;  // 300
stzeitd4=1000;   // 7000Frames

//stufe 16-20 500 exp pro weitere Stufe und 500 Gold
stkosten5=(stkosten3 ?? 0)*5;  // 500
sterfahrung5=((sterfahrung3 ?? 0)*5)/2;  // 500
stzeitd5=1000;   // 7000 Frames


// jetzt noch je nach aktiven Regeln anpassen:
//Kosten
if(S42MigrationEqual(ObjectCount(PP1P), nil)) {kosten1=stkosten1;kosten2=stkosten2;kosten3=stkosten3;kosten4=stkosten4;kosten5=stkosten5;}

if(S42MigrationEqual(ObjectCount(PP1P), 1))  {kosten1=(stkosten1 ?? 0)*0;kosten2=(stkosten2 ?? 0)*0;kosten3=(stkosten3 ?? 0)*0;kosten4=(stkosten4 ?? 0)*0;kosten5=(stkosten5 ?? 0)*0;}

if(S42MigrationEqual(ObjectCount(PP1P), 2)) {kosten1=(stkosten1 ?? 0)/2;kosten2=(stkosten2 ?? 0)/2;kosten3=(stkosten3 ?? 0)/2;kosten4=(stkosten4 ?? 0)/2;kosten5=(stkosten5 ?? 0)/2;}

if(S42MigrationEqual(ObjectCount(PP1P), 3)) {kosten1=(stkosten1 ?? 0)*2;kosten2=(stkosten2 ?? 0)*2;kosten3=(stkosten3 ?? 0)*2;kosten4=(stkosten4 ?? 0)*2;kosten5=(stkosten5 ?? 0)*2;}

// Zeit
if(S42MigrationEqual(ObjectCount(PP2P), nil)) {zeitd1=stzeitd1;zeitd2=stzeitd2;zeitd3=stzeitd3;zeitd4=stzeitd4;zeitd5=stzeitd5;}

if(S42MigrationEqual(ObjectCount(PP2P), 1)) {zeitd1=1;zeitd2=1;zeitd3=1;zeitd4=1;zeitd5=1;}

if(S42MigrationEqual(ObjectCount(PP2P), 2)) {zeitd1=(stzeitd1 ?? 0)/2;zeitd2=(stzeitd2 ?? 0)/2;zeitd3=(stzeitd3 ?? 0)/2;zeitd4=(stzeitd4 ?? 0)/2;zeitd5=(stzeitd5 ?? 0)/2;}

// Erfahrung
if(S42MigrationEqual(ObjectCount(PP3P), nil)) {erfahrung1=sterfahrung1;erfahrung2=sterfahrung2;erfahrung3=sterfahrung3;erfahrung4=sterfahrung4;erfahrung5=sterfahrung5;}

if(S42MigrationEqual(ObjectCount(PP3P), 1)) {erfahrung1=(sterfahrung1 ?? 0)*0;erfahrung2=(sterfahrung2 ?? 0)*0;erfahrung3=(sterfahrung3 ?? 0)*0;erfahrung4=(sterfahrung4 ?? 0)*0;erfahrung5=(sterfahrung5 ?? 0)*0;}

if(S42MigrationEqual(ObjectCount(PP3P), 2)) {erfahrung1=(sterfahrung1 ?? 0)/2;erfahrung2=(sterfahrung2 ?? 0)/2;erfahrung3=(sterfahrung3 ?? 0)/2;erfahrung4=(sterfahrung4 ?? 0)/2;erfahrung5=(sterfahrung5 ?? 0)/2;}

if(S42MigrationEqual(ObjectCount(PP3P), 3)) {erfahrung1=(sterfahrung1 ?? 0)*2;erfahrung2=(sterfahrung2 ?? 0)*2;erfahrung3=(sterfahrung3 ?? 0)*2;erfahrung4=(sterfahrung4 ?? 0)*2;erfahrung5=(sterfahrung5 ?? 0)*2;}

// für die Kostenanzeige beim Söldnerkauf
// durch die Verwendung der lokalen Variablen, passen die Werte immer, wobei der Faktor Delay eingebaut ist. Die tatsächlichen Kosten für die Stufe werden mit 1,5 multipliziert (mal 15, durch 10). Die Kosten des neuen Clonks sind 30 Clunker, deswegen +30.
stkaufkosten0 = 30;
stkaufkosten1 = ((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100)*15)/10+30;   // mit standardwerten: 280*1,5
stkaufkosten2 = (((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100) + 5*(kosten3 ?? 0)+(5*(zeitd3 ?? 0)*5)/100)*15)/10+30;  //1030*1,5
stkaufkosten3 = ((((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100) + 5*(kosten3 ?? 0)+(5*(zeitd3 ?? 0)*5)/100)+5*(kosten4 ?? 0)+(5*(zeitd4 ?? 0)*7)/100)*15)/10+30;  //2880*1,5
stkaufkosten4 = (((((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100) + 5*(kosten3 ?? 0)+(5*(zeitd3 ?? 0)*5)/100)+5*(kosten4 ?? 0)+(5*(zeitd4 ?? 0)*7)/100)+5*(kosten5 ?? 0)+(5*(zeitd5 ?? 0)*7)/100)*15)/10+30;  // 5730*1,5
}


func Completion()
{
SetEntrance(1);
}


public func ContainedUp(object pClonk)
{
  [Ausbildung|Image=RSR1]
  HauptMenu(pClonk);
  derclonk=pClonk;

//wenn mehr als 1 Clonk drin ist, rausschmeißen
  while ((ObjectCount2(Find_OCF(OCF_CrewMember),Find_Container(this)) ?? 0)>=2)
      {
  var doorX = (GetDefCoreVal("Entrance","DefCore",nil,nil) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,2) ?? 0) / 2;
  var doorY = (GetDefCoreVal("Entrance","DefCore",nil,1) ?? 0) + (GetDefCoreVal("Entrance","DefCore",nil,3) ?? 0);
var clonk = FindObject2(Find_OCF(OCF_CrewMember));
Exit(clonk,doorX,doorY);}
  return(1);
}

public func ContainedDown(object pClonk)
{
if(!aktiveausbildung)  // nur wenn grad keine ausblidung im gange ist
SetEntrance(1);
  return(nil);
}

func HauptMenu(clonk,zahl)
{
 // Hauptmenü erzeugen
  CreateMenu(HZCK, clonk, this,nil,nil,nil,1);
  SelectMenuItem(zahl,derclonk);
if(aktiveausbildung)  AddMenuItem("Fortschritt anzeigen", "Fortschrittanzeigen", RSR3, clonk, nil, clonk,"Zeigt den Ausbildungsfortschritt an");
  AddMenuItem("Infos zum Clonk", "ClonkInfoMenu", RSR4, clonk, nil, clonk,"Hier kannst du die die Physics Werte deines Clonks anzeigen lassen.");
if(!aktiveausbildung) AddMenuItem("Ausbilden", "AusbildungsMenu", RSR3, clonk, nil, clonk,"Hier lässt sich die nächste Ausbildungsstufe wählen");
  AddMenuItem("Allgemeine Information", "InfoMenu", RSR4, clonk, nil, clonk,"Hier gibt es Infos zur Ausblidung und den neuen Fähigkeiten");
if(aktiveausbildung)  AddMenuItem("Ausbildung abbrechen", "CancelAusbildung", S__9, clonk, nil, clonk,"Bricht die Ausbildung ab, Fortschritte bleiben aber erhalten.");
// Söldner anheuern, also Clonks die schon ein gewisses Level haben und man sie daher nicht ausbilden muss. Kosten aber auch dementsprechend viel!
AddMenuItem("Söldner anheuern", "ClonkKaufMenu", HZCK, clonk, nil, nil,"Heuere bereits ausgebildete Söldner an!");
}



func ClonkKaufMenu(idImage, iSelection)
{
 // Menü erzeugen
  CreateMenu(HZCK, derclonk, this,nil,"wenn noch keine Söldner verfügbar, schaue nach weiterer Forschung nochmal nach",nil,1);
// erst bei bestimmten Wissensstand des Gebäudeinhabers verfügbar 
SelectMenuItem(iSelection,derclonk);
if(FindObject2(Find_ID(PPP2)))
  {
AddMenuItem(Format("Clonk Stufe 0 für %v Clunker anheuern",stkaufkosten0), "KaufClonk", CLNK, derclonk, nil, nil,"Kaufe dir einen neuen Clonk Stufe 0");
if(GetPlrKnowledge(GetOwner(),S_O1)) AddMenuItem(Format("Soldat Stufe 5 für %v Clunker anheuern",stkaufkosten1), "KaufClonk", S_XY, derclonk, nil, 5,"Kaufe dir für viel Geld einen neuen Clonk bestimmter Stufe und überspringe damit eine lange Ausbildung");
if(GetPlrKnowledge(GetOwner(),S_O1)) AddMenuItem(Format("Arbeiter Stufe 5 für %v Clunker anheuern",stkaufkosten1), "KaufClonk", X_12, derclonk, nil, 5,"Kaufe dir für viel Geld einen neuen Clonk bestimmter Stufe und überspringe damit eine lange Ausbildung");
  }
// wenn keine angepassten Pyhsics aktiviert
if(!FindObject2(Find_ID(PPP2)))
 {
 AddMenuItem(Format("Clonk für %v Clunker anheuern",stkaufkosten0), "KaufClonk", CLNK, derclonk, nil, nil,"Kaufe dir einen neuen Clonk");
 }
 AddMenuItem(Format("Pferd für %v Clunker kaufen",GetValue(nil, HORS)), "KaufClonk", HORS, derclonk, nil, 5,"Kaufe dir einen treuen Begleiter");
}

func KaufClonk(idImage,istufe)
{
//Die Kosten für Söldner, je nach gewünschter stufe: kaufkosten
// abhängig von den eingestellten Ausbildungskosten
//errechnet sich aus den Gesamtkosten bis zu dieser stufe, plus die GesamtZeit, wobei 10.000Frames mit 100 Clunker gewichtet werden. Die nötige Erfahrung geht erstmal nicht ein, da sonst zuviele Fallunterscheidungen nötig sind.
// PP1P regelt die Kosten und PP2P die Zeit und PP3P die nötige Erfahrung mit: 0 Standard, 1 nichts, 2 halb standard, 3 doppelt standard.  Wobei bei Zeit doppelt nicht so leicht umsetzbar ist, aufgrund der Aktion-Kombi und deswegen erstmal ausfällt.

var DieSelection;

// durch die Verwendung der lokalen Variablen, passen die Werte immer. Die tatsächlichen Kosten für die Stufe werden mit 1,5 multipliziert (mal 15, durch 10). Kaufkosten eines Clonks ist 30, daher +30
var kaufkosten;
if(S42MigrationEqual(istufe, nil))
kaufkosten = 30;
if(S42MigrationEqual(istufe, 5))
kaufkosten = ((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100)*15)/10+30;   // mit standardwerten: 280*1,5
if(S42MigrationEqual(istufe, 10))
kaufkosten = (((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100) + 5*(kosten3 ?? 0)+(5*(zeitd3 ?? 0)*5)/100)*15)/10+30;  //1030*1,5
if(S42MigrationEqual(istufe, 15))
kaufkosten = ((((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100) + 5*(kosten3 ?? 0)+(5*(zeitd3 ?? 0)*5)/100)+5*(kosten4 ?? 0)+(5*(zeitd4 ?? 0)*7)/100)*15)/10+30;  //2880*1,5
if(S42MigrationEqual(istufe, 20))
kaufkosten = (((((4*(kosten1 ?? 0)+(kosten2 ?? 0)+(4*(zeitd1 ?? 0)*6+(zeitd2 ?? 0)*6)/100) + 5*(kosten3 ?? 0)+(5*(zeitd3 ?? 0)*5)/100)+5*(kosten4 ?? 0)+(5*(zeitd4 ?? 0)*7)/100)+5*(kosten5 ?? 0)+(5*(zeitd5 ?? 0)*7)/100)*15)/10+30;  // 5730*1,5

if (S42MigrationEqual(idImage, HORS)) kaufkosten = GetValue (nil, HORS);

  // genug finanzielle Rücklagen?
  if ((GetWealth(GetOwner(derclonk)) ?? 0) < (kaufkosten ?? 0))
  {
    PlayerMessage(GetOwner(derclonk), "Nicht genug Geld!", this);
    Sound("Error");
    return(ClonkKaufMenu(idImage,DieSelection));
  }
  // Clonkanzahl prüfen
  if ((GetCrewCount(GetOwner(derclonk)) ?? 0) >= 10 && !S42MigrationEqual(idImage, HORS))
  {
    PlayerMessage(GetOwner(derclonk), "Maximal 10 Clonks pro Spieler", this);
    Sound("Error");
    return(ClonkKaufMenu(idImage,DieSelection));
  }
  //Log("kurz vor ware erzeugen");
  // Ware erzeugen
  DoWealth(GetOwner(derclonk), -(kaufkosten ?? 0));
  Sound("Cash");
  if (S42MigrationEqual(idImage, HORS))
    return(CreateObject(HORS,nil,nil,GetOwner(derclonk)));
  if(!S42MigrationEqual(istufe, nil))
  var gekaufterclonk = CreateObject(HZCK,nil,nil,GetOwner(derclonk));
  if(S42MigrationEqual(istufe, nil))
  var gekaufterclonk = CreateObject(CLNK,nil,nil,GetOwner(derclonk));
  //Log("kurz vor makecrew");
  // frische Clonks müssen noch zur Crew hinzugefügt werden
   MakeCrewMember(gekaufterclonk, GetOwner(derclonk));
    //Log("nach makecrew");

// nun noch Stufe und Grafik des neuen Clonks anpassen
DieSelection = nil;
if(S42MigrationEqual(istufe, 5))
 {
  //Log("stufe 5 aufgerufen");
gekaufterclonk->~Stufenanstieg(1);
SetGraphics("Normal",gekaufterclonk);  //Hazardclonk mit Clonkgrafik
SetPortrait("random", gekaufterclonk, CLNK, nil, nil);
// die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",30000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",30000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",30000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",50000 , PHYS_StackTemporary, gekaufterclonk);
 DieSelection=1;
if(S42MigrationEqual(idImage, X_12)) {gekaufterclonk->~NunBauarbeiter();DieSelection=2;}
  //Log("stufe 5 fertig");
 }

// Stufe 10 Arbeiter
 if(S42MigrationEqual(istufe, 10) && S42MigrationEqual(idImage, X_12))
  {
gekaufterclonk->~Stufenanstieg(10);
  // Log("stufe 10 aufgerufen");
 SetGraphics("Bauer",gekaufterclonk); SetPortrait("BauerP", gekaufterclonk, HZCK); gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();
  // die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",200000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",75000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",70000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",45000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",45000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",45000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",70000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",50000 , PHYS_StackTemporary, gekaufterclonk);
  //  Log("stufe 10 fertig");
 DieSelection=4;
  } 

// Stufe 10 Soldat
 if(S42MigrationEqual(istufe, 10) && S42MigrationEqual(idImage, S_XY))
 {
gekaufterclonk->~Stufenanstieg(10);
 // Log("stufe 10s aufgerufen");
SetGraphics("SG1",gekaufterclonk);SetPortrait("SGP", gekaufterclonk, HZCK);
   // die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",200000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",75000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",70000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",45000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",45000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",35000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",70000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",65000 , PHYS_StackTemporary, gekaufterclonk);
 // Log("stufe 10s fertig");
DieSelection=3;
 }


// Stufe 15 Arbeiter
 if(S42MigrationEqual(istufe, 15) && S42MigrationEqual(idImage, X_12))
  {
gekaufterclonk->~Stufenanstieg(15);
 // Log("stufe 15 aufgerufen");
 SetGraphics("Bauer",gekaufterclonk); SetPortrait("BauerP", gekaufterclonk, HZCK); gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();
  // die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",300000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",90000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",50000 , PHYS_StackTemporary, gekaufterclonk);
 // Log("stufe 15 fertig");
DieSelection=6;
  } 


// Stufe 15 Soldat
 if(S42MigrationEqual(istufe, 15) && S42MigrationEqual(idImage, S_XY))
 {
gekaufterclonk->~Stufenanstieg(15);
 // Log("stufe 15s aufgerufen");
 //Grafik muss nicht gesetzt werden, weil Hazardclonk schon die hazardgrafik hat 
   // die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",300000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",60000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",90000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",80000 , PHYS_StackTemporary, gekaufterclonk);
 // Log("stufe 15s fertig");
DieSelection=5;
 }

// Stufe 20 Arbeiter
 if(S42MigrationEqual(istufe, 20) && S42MigrationEqual(idImage, X_12))
  {
gekaufterclonk->~Stufenanstieg(20);
 // Log("stufe 20 aufgerufen");
 SetGraphics("Bauer",gekaufterclonk); SetPortrait("BauerP", gekaufterclonk, HZCK); gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();gekaufterclonk->~NunBauarbeiter();
  // die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",400000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",150000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",50000 , PHYS_StackTemporary, gekaufterclonk);
 // Log("stufe 20 fertig");
DieSelection=8;
  } 


// Stufe 20 Soldat
 if(S42MigrationEqual(istufe, 20) && S42MigrationEqual(idImage, S_XY))
 {
gekaufterclonk->~Stufenanstieg(20);
 // Log("stufe 20s aufgerufen");
 SetGraphics("Future",gekaufterclonk);
 SetPortrait("FutureP", gekaufterclonk, HZCK);
   // die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",400000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Breath",150000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Walk",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Scale",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Hangle",80000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Dig",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Swim",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Push",100000 , PHYS_StackTemporary, gekaufterclonk);
 SetPhysical("Fight",150000 , PHYS_StackTemporary, gekaufterclonk);
 // Log("stufe 20s fertig");
DieSelection=7;
 }


if ((GetEnergy(gekaufterclonk) ?? 0) < (GetPhysical("Energy",nil,gekaufterclonk) ?? 0)/1000)
{
var lebenspunkteheilen = (((GetPhysical("Energy",nil,gekaufterclonk) ?? 0)/1000)-(GetEnergy(gekaufterclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),gekaufterclonk);
}
  // und fertig
   //Log("fertig");
   return(ClonkKaufMenu(idImage,DieSelection));
}


// im clonk die neuen Physics aktivieren


func Fortschrittanzeigen(idImage, clonk)
{
var stuufe=derclonk->~AktuelleStufe();
var timeraktuell=derclonk->~StandTimerAusbildung();
var fortschritt;
if(S42MigrationEqual(stuufe, nil) || S42MigrationEqual(stuufe, 1) || S42MigrationEqual(stuufe, 2) || S42MigrationEqual(stuufe, 3) || S42MigrationEqual(stuufe, 4))
{
fortschritt=(100-(((500-(timeraktuell ?? 0))*100)/500));
}

if(S42MigrationEqual(stuufe, 5) || S42MigrationEqual(stuufe, 6) || S42MigrationEqual(stuufe, 7) || S42MigrationEqual(stuufe, 8) || S42MigrationEqual(stuufe, 9) || S42MigrationEqual(stuufe, 10) || S42MigrationEqual(stuufe, 11) || S42MigrationEqual(stuufe, 12) || S42MigrationEqual(stuufe, 13) || S42MigrationEqual(stuufe, 14) || S42MigrationEqual(stuufe, 15) || S42MigrationEqual(stuufe, 16) || S42MigrationEqual(stuufe, 17) || S42MigrationEqual(stuufe, 18) || S42MigrationEqual(stuufe, 19))
{
fortschritt=(100-(((1000-(timeraktuell ?? 0))*100)/1000));
}

PlayerMessage(GetOwner(derclonk),Format("Fortschritt: %v%",fortschritt),derclonk);
HauptMenu(clonk,nil);
}


func CancelAusbildung(idImage, clonk)
{
PlayerMessage(GetOwner(derclonk),"Ausbildung abgebrochen, die bisherigen Physicssteigerungen bleiben bestehen, aber die Stufe ändert sich nicht und kann jederzeit fortgesetzt werden",derclonk); SetAction("Idle");aktiveausbildung=nil;   return(nil);
}


//doch nicht an hp festmachen, weil sonst durch Rüstung oderso fälschlicherweise die falsche stufe gelernt wird

func AusbildungsMenu(idImage, clonk)
{
CreateMenu(CLNK, clonk, this(),nil,"Ausbildung wählen",nil,1);
var stuufe=derclonk->~AktuelleStufe();
var erfahrung= derclonk->~RundenErfahrung();

if(FindObject2(Find_ID(PPP2)))
{
if (S42MigrationEqual(stuufe, nil)) {  AddMenuItem(Format("Ausbildung Stufe 1 für %v Clunker",kosten1), "Stufe1bis4", CLNK, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 1)) {  AddMenuItem(Format("Ausbildung Stufe 2 für %v Clunker",kosten1), "Stufe1bis4", CLNK, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 2)) {  AddMenuItem(Format("Ausbildung Stufe 3 für %v Clunker",kosten1), "Stufe1bis4", CLNK, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 3)) {  AddMenuItem(Format("Ausbildung Stufe 4 für %v Clunker",kosten1), "Stufe1bis4", CLNK, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 4)) {  AddMenuItem(Format("Ausbildung als SOLDAT Stufe 5 für %v Clunker",kosten2), "SicherheitsabfrageStufe5", S_XY, clonk, nil, clonk,"Der Soldat kann mit allen Ausrüstungen umgehen, gräbt und baut aber langsamer als der Arbeiter und kann nur 2 Items tragen");}
if (S42MigrationEqual(stuufe, 5) && !(clonk->~IstBauarbeiter())) {  AddMenuItem(Format("Soldatenausbildung Stufe 6 für %v Clunker",kosten3), "Stufe6bis10", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 6) && !(clonk->~IstBauarbeiter())) {  AddMenuItem(Format("Soldatenausbildung Stufe 7 für %v Clunker",kosten3), "Stufe6bis10", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 7) && !(clonk->~IstBauarbeiter())) {  AddMenuItem(Format("Soldatenausbildung Stufe 8 für %v Clunker",kosten3), "Stufe6bis10", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 8) && !(clonk->~IstBauarbeiter())) {  AddMenuItem(Format("Soldatenausbildung Stufe 9 für %v Clunker",kosten3), "Stufe6bis10", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 4)) {  AddMenuItem(Format("Ausbildung als ARBEITER Stufe 5 für %v Clunker",kosten2), "SicherheitsabfrageStufe5", X_12, clonk, nil, clonk,"Der Arbeiter kann nicht annährend so gut Kämpfen wie der Soldat und nur schlecht mit Munition umgehen. Aber er baut und gräbt deutlich schneller und kann in der letzten Stufe bis zu 5 Items tragen. Auf Stufe 5 ist seine Bauzahl 1.");}
if (S42MigrationEqual(stuufe, 5) && clonk->~IstBauarbeiter()) {  AddMenuItem(Format("Arbeiterausbildung Stufe 6 für %v Clunker",kosten3), "Stufe6bis10Arbeiter", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 6) && clonk->~IstBauarbeiter()) {  AddMenuItem(Format("Arbeiterausbildung Stufe 7 für %v Clunker",kosten3), "Stufe6bis10Arbeiter", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 7) && clonk->~IstBauarbeiter()) {  AddMenuItem(Format("Arbeiterausbildung Stufe 8 für %v Clunker",kosten3), "Stufe6bis10Arbeiter", S__7, clonk, nil, clonk);}
if (S42MigrationEqual(stuufe, 8) && clonk->~IstBauarbeiter()) {  AddMenuItem(Format("Arbeiterausbildung Stufe 9 für %v Clunker",kosten3), "Stufe6bis10Arbeiter", S__7, clonk, nil, clonk);}
if((stuufe ?? 0)>=5)  {  AddMenuItem("Umschulung auf Stufe 5", "Sicherheitsabfrageumschulung", CLNK, clonk, nil, clonk,"Du kannst hier die Spezialisierung deines Clonks ändern. Dafür wird er allerdings wieder auf Stufe 5 gesetzt!");}
}

// Im Fall, dass alle Clonks dieselben Werte haben und daher keine Wertsteigerungen brauchen. Hier findet ein Sprunghafter Anstieg der Werte bei Stufe 5 statt, weil von Clonk zu Hazardclonk gewechselt wird. Ansonsten gibt es keine Wertveränderungen.
if(!FindObject2(Find_ID(PPP2)))
{
if (S42MigrationEqual(stuufe, nil)) {  AddMenuItem("Stufe 5 Soldat", "SicherheitsabfrageStufe5keineWerte", S_XY, clonk, nil, clonk,"Der Soldat kann mit allen Ausrüstungen umgehen, gräbt und baut aber langsamer als der Arbeiter und kann nur 2 Items tragen");}
if (S42MigrationEqual(stuufe, nil)) {  AddMenuItem("Stufe 5 Arbeiter", "SicherheitsabfrageStufe5keineWerte", X_12, clonk, nil, clonk,"Der Arbeiter kann nicht annährend so gut Kämpfen wie der Soldat und nur schlecht mit Munition umgehen. Aber er baut und gräbt deutlich schneller und kann in der letzten Stufe bis zu 5 Items tragen. Auf Stufe 5 ist seine Bauzahl 1.");}

if ((stuufe ?? 0) >=5)  {  AddMenuItem("Umschulung auf Stufe 5",
"SicherheitsabfrageumschulungkeineWerte", CLNK, clonk, nil, clonk,"Du kannst hier die Spezialisierung deines Clonks ändern. Dafür wird er allerdings wieder auf Stufe 5 gesetzt!");}
}

}




func SicherheitsabfrageumschulungkeineWerte(idImage,clonk)
{
CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
AddMenuItem("Nein, nicht umschulen", "machnichtsumschulung", S__9, clonk, nil, clonk,"Keine Umschulung, Stufe und Spezialisierung beibehalten");
AddMenuItem("Ja, umschulen!", "Stufe5keineWerte", CLNK, clonk, nil, clonk,"Du kannst hier die Spezialisierung deines Clonks ändern. Dafür wird er allerdings wieder auf Stufe 5 gesetzt!");
}


func Sicherheitsabfrageumschulung(idImage,clonk)
{
CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
AddMenuItem("Nein, nicht umschulen", "machnichtsumschulung", S__9, clonk, nil, clonk,"Keine Umschulung, Stufe und Spezialisierung beibehalten");
AddMenuItem("Ja, umschulen!", "Stufe5", CLNK, clonk, nil, clonk,"Du kannst hier die Spezialisierung deines Clonks ändern. Dafür wird er allerdings wieder auf Stufe 5 gesetzt!");
}

func machnichtsumschulung(idImage,clonk)
{
HauptMenu(clonk);
}

func SicherheitsabfrageStufe5(idImage,clonk)
{
CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
AddMenuItem("Zurück", "machnichtsumschulung", S__9, clonk, nil, clonk,"Zurück zum Hauptmenü");
if(S42MigrationEqual(idImage, S_XY))
AddMenuItem("Bestätigung: Stufe 5 Soldat", "Stufe5", S_XY, clonk, nil, clonk,"Der Soldat kann mit allen Ausrüstungen umgehen, gräbt und baut aber langsamer als der Arbeiter und kann nur 2 Items tragen");
if(S42MigrationEqual(idImage, X_12))
AddMenuItem("Bestätigung:Stufe 5 Arbeiter", "Stufe5", X_12, clonk, nil, clonk,"Der Arbeiter kann nicht annährend so gut Kämpfen wie der Soldat und nicht mit Waffen umgehen. Aber er baut und gräbt deutlich schneller und kann in der letzten Stufe bis zu 5 Items tragen");
}

func SicherheitsabfrageStufe5keineWerte(idImage,clonk)
{
CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
AddMenuItem("Zurück", "machnichtsumschulung", S__9, clonk, nil, clonk,"Zurück zum Hauptmenü");
if(S42MigrationEqual(idImage, S_XY))
AddMenuItem("Bestätigung: Stufe 5 Soldat", "Stufe5keineWerte", S_XY, clonk, nil, clonk,"Der Soldat kann mit allen Ausrüstungen umgehen, gräbt und baut aber langsamer als der Arbeiter und kann nur 2 Items tragen");
if(S42MigrationEqual(idImage, X_12))
AddMenuItem("Bestätigung:Stufe 5 Arbeiter", "Stufe5keineWerte", X_12, clonk, nil, clonk,"Der Arbeiter kann nicht annährend so gut Kämpfen wie der Soldat und nicht mit Waffen umgehen. Aber er baut und gräbt deutlich schneller und kann in der letzten Stufe bis zu 5 Items tragen");
}




/*
//funktionen im Clonksskript


func TimerAusbildung()
{
++ausbildungstimer;
}

func StandTimerAusbildung()
{
return(ausbildungstimer);
}


func NunBauarbeiter(zahl)
{
++bauarbeiter;  if.... -> wenn zahl==10, dann wird auf 0 gesetzt.
}

func IstBauarbeiter()
{
return(bauarbeiter);
}


func RundenErfahrung()
{
// gibt die in dieser Runde bereits erhaltene Erfahrung zum Zeitpunkt der Abfrage aus
return(GetObjectInfoCoreVal("Experience", 0, 0)-starterfahrung);
}

func Stufenanstieg(zahl)
{
if(zahl==1) {stufe==5; return(0);} // für den Aufstieg von Clonk zu Hazardclonk die Stufen übernehmen
++stufe;
}

func AktuelleStufe()
{
return(stufe);
}

*/



// wenn keine angepasste Physics, dann gibts nur diese Stufenaufwertung mit hazard oder Bauergrafik und allen fähigkeiten und Werten des Hazardclonks
func Stufe5keineWerte(idImage, clonk)
{
//instant und keine Kosten und keine Erfahrung nötig

if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

//Für Umschulung:
if(S42MigrationEqual(idImage, CLNK)) {if(!(clonk->~IstBauarbeiter())) idImage=X_12; if(clonk->~IstBauarbeiter()) {idImage=S_XY;derclonk->~NunBauarbeiter(10);} }
if(S42MigrationEqual(idImage, X_12)) {derclonk->~NunBauarbeiter(6);}
 //arbeiterstufe6

if (!(S42MigrationEqual(GetAction(), "AusbildungStufe5"))) SetAction("AusbildungStufe5");
aktiveausbildung=1;

//kurz vorher Item aus der Hand nehmen, damit "Item in der hand" keine Probleme macht
if((derclonk->~StandTimerAusbildung() ?? 0)>=4) {
var inhalt=Contents(nil,derclonk);
if(inhalt) {Exit(inhalt,nil,nil);Enter(this,inhalt);}}

if((derclonk->~StandTimerAusbildung() ?? 0)>=5) {
SetAction("Idle");aktiveausbildung=nil;

var owner=GetOwner(derclonk);
ChangeDef(HZCK,derclonk);
derclonk->~Initialize();
derclonk->~Construction();
RemoveEffect("InventarCheck",derclonk); // entfernt das "item in der hand tragen" (weil keine Actmap dafür in hazard und außerdem zu kompliziert in kombi mit waffen und mehrern items)

derclonk->~Stufenanstieg(20); // wird sofort Stufe 20
var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); 
derclonk->~TimerAusbildungNull();
  
if(S42MigrationEqual(idImage, S_XY))
 {SetGraphics("",derclonk);    // normale Hazardgrafik 
SetPortrait("random", derclonk, HZCK, nil, nil);
 }

if(S42MigrationEqual(idImage, X_12))
 {SetGraphics("Bauer",derclonk); SetPortrait("random", derclonk, X_12); 
 }

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);
}
derclonk->~TimerAusbildung();

}




func Stufe1bis4(idImage,clonk)
{
// Grundausbildung für alle Clonks, nicht spezialisiert
// setzt 3000Frames lang alle 6 Frames die HP um 20 hoch, sodass dann letzlich 10.000 mehr Lebenspunkte rauskommen. ->Delay6  ... HP verdoppelt, also nun +40HP alle 6 Frames.
//Anwendung bei Stufe1 bis 4. Stufe 5 ändert nur Fähigkeiten und ab stufe 5 dann neue funktion


//Erfahrungsabfrage, 100 pro stufe gebruacht und 20 Gold, sind die Standardwerte kosten1 und erfahrung1.

if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
// Erfahrung muss für stufe 1:100 sein, für 2:200, für 3:300 usw.
if((erfahrung ?? 0) < (erfahrung1 ?? 0)*((aktuellestufe ?? 0)+1) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten1 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (erfahrung1 ?? 0)*((aktuellestufe ?? 0)+1),kosten1),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten1 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

if (!(S42MigrationEqual(GetAction(), "Ausbildung1"))) SetAction("Ausbildung1");
derclonk->~TimerAusbildung();
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd1 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), nil))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +40, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +15, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +15, PHYS_StackTemporary, derclonk);
}

//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(40*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(15*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(10*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(10*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(15*(stzeitd1 ?? 0)), PHYS_StackTemporary, derclonk);
}

// halbe Zeit, also doppelte werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +80, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +30, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +10, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +30, PHYS_StackTemporary, derclonk);
}
derclonk->~TimerAusbildung();

}

func Stufe5(idImage, clonk)
{

//Erfahrungsabfrage, 500 gebraucht
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
// Erfahrung muss für stufe 1:100 sein, für 2:200, für 3:300 usw.
if((erfahrung ?? 0) < (erfahrung2 ?? 0)*((aktuellestufe ?? 0)+1) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten2 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (erfahrung2 ?? 0)*((aktuellestufe ?? 0)+1),kosten2),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten2 ?? 0));
  Sound("Cash");}

if(clonk) //nur beim ersten durchlauf. Da der clonk bei wenig Energie ab und zu stirbt...:D
DoEnergy(30000,clonk,true);
if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

//Für Umschulung:
if(S42MigrationEqual(idImage, CLNK)) {if(!(clonk->~IstBauarbeiter())) idImage=X_12; if(clonk->~IstBauarbeiter()) {idImage=S_XY;derclonk->~NunBauarbeiter(10);} }
if(S42MigrationEqual(idImage, X_12)) {derclonk->~NunBauarbeiter();}
 //arbeiterstufe1

if (!(S42MigrationEqual(GetAction(), "AusbildungStufe5"))) SetAction("AusbildungStufe5");
aktiveausbildung=1;

//kurz vorher Item aus der Hand nehmen, damit "Item in der hand" keine Probleme macht
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd2 ?? 0)-5) {
var inhalt=Contents(nil,derclonk);
if(inhalt) {Exit(inhalt,nil,nil);Enter(this,inhalt);}}

if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd2 ?? 0)) {
SetAction("Idle");aktiveausbildung=nil;CloseMenu(derclonk);

var owner=GetOwner(derclonk);
ChangeDef(HZCK,derclonk);
derclonk->~Initialize();
derclonk->~Construction();
RemoveEffect("InventarCheck",derclonk); // entfernt das "item in der hand tragen" (weil keine Actmap dafür in hazard und außerdem zu kompliziert in kombi mit waffen und mehrern items)

derclonk->~Stufenanstieg(1); // wird sofort Stufe 5
var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht und kann nun die hier erklärten Skills einsetzen!", stuufe),derclonk); 
derclonk->~TimerAusbildungNull();
  
SetGraphics("Normal",derclonk);  //Hazardclonk mit Clonkgrafik
SetPortrait("random", derclonk, CLNK, nil, nil);

// die Physics an aktuelle Werte anpassen:
 SetPhysical("Energy",100000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Breath",50000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Walk",60000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Jump",40000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Scale",30000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Hangle",30000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Dig",30000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Swim",50000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Throw",50000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Push",40000 , PHYS_StackTemporary, derclonk);
 SetPhysical("Fight",50000 , PHYS_StackTemporary, derclonk);

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);
}
derclonk->~TimerAusbildung();

}




func Stufe6bis10(idImage, clonk)
{

//soll 5000 Frames dauern und minimal +1000 geben können ->Delay5


//Erfahrungsabfrage, 200 pro Stufe ist erfahrung3
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
if((erfahrung ?? 0) < (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*((aktuellestufe ?? 0)-4) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten3 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*((aktuellestufe ?? 0)-4),kosten3),derclonk));}
}


//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten3 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

if (!(S42MigrationEqual(GetAction(), "Ausbildung2"))) SetAction("Ausbildung2");
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd3 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);
if(S42MigrationEqual(stuufe, 10)) {SetGraphics("SG1",derclonk);SetPortrait("random", derclonk, S_XX); } //Hazardclonk mit SG1 Grafik

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +1, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);
}


//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(1*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);
}


//Halbe Zeit, doppelte Werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(1*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);
}


derclonk->~TimerAusbildung();
}



func Stufe6bis10Arbeiter(idImage, clonk)
{

//soll 5000 Frames dauern, alles wie soldat nur graben mehr. ->Delay5

//Erfahrungsabfrage, 200 pro Stufe, also erfahrung3
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
if((erfahrung ?? 0) < (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*((aktuellestufe ?? 0)-4) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten3 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*((aktuellestufe ?? 0)-4),kosten3),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten3 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

if (!(S42MigrationEqual(GetAction(), "AusbildungArbeiter2"))) SetAction("AusbildungArbeiter2");
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd3 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);
 if(S42MigrationEqual(stuufe, 10)) { SetGraphics("Bauer",derclonk); SetPortrait("random", derclonk, X_12);
derclonk->~NunBauarbeiter();} //arbeiterstufe2, Hazardclonk mit Arbeiter Grafik

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), nil))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);
}


//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd3 ?? 0)), PHYS_StackTemporary, derclonk);
}


//Halbe Zeit, doppelte Werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);
}

derclonk->~TimerAusbildung();
}



func Stufe11bis15(idImage, clonk)
{

//Erfahrungsabfrage, 300 pro Stufe, also erfahrung4
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
if((erfahrung ?? 0) < (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*((aktuellestufe ?? 0)-9) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten4 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*((aktuellestufe ?? 0)-9),kosten4),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten4 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

//soll 7000 Frames dauern und minimal +1000 geben können -> Delay 7
//Werte so wie 5bis10, aber Zeit anders

if (!(S42MigrationEqual(GetAction(), "Ausbildung3"))) SetAction("Ausbildung3");
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd4 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);
 if(S42MigrationEqual(stuufe, 15)) {SetGraphics("",derclonk);    // normale Hazardgrafik
SetPortrait("random", derclonk, HZCK, nil, nil);}

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), nil))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +1, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);
}


//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(1*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);
}

//Halbe Zeit, doppelte Werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(1*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);
}

derclonk->~TimerAusbildung();
}




func Stufe11bis15Arbeiter(idImage, clonk)
{

//soll 7000 Frames dauern und minimal +1000 geben können -> Delay 7, wieder wie soldate, nur graben noch mehr

//Erfahrungsabfrage, 300 pro Stufe, also erfahrung4
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
if((erfahrung ?? 0) < (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*((aktuellestufe ?? 0)-9) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten4 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*((aktuellestufe ?? 0)-9),kosten4),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten4 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

if (!(S42MigrationEqual(GetAction(), "AusbildungArbeiter3"))) SetAction("AusbildungArbeiter3");
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd4 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);
if(S42MigrationEqual(stuufe, 13))derclonk->~NunBauarbeiter(); //arbeiterstufe3
if(S42MigrationEqual(stuufe, 15))derclonk->~NunBauarbeiter(); //arbeiterstufe4

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), nil))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +5, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +3, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);
}


//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd4 ?? 0)), PHYS_StackTemporary, derclonk);
}


//Halbe Zeit, doppelte Werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(5*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(3*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);
}

derclonk->~TimerAusbildung();
}




func Stufe16bis20(idImage, clonk)
{

//soll 10000 Frames dauern  -> Delay 10
//Werte so wie 5bis10, aber Zeit anders

//Erfahrungsabfrage, 500 pro Stufe, also erfahrung5
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
if((erfahrung ?? 0) < (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*5+(erfahrung5 ?? 0)*((aktuellestufe ?? 0)-14) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten5 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und %v Clunker!", (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*5+(erfahrung5 ?? 0)*((aktuellestufe ?? 0)-14),kosten5),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten5 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

if (!(S42MigrationEqual(GetAction(), "Ausbildung4"))) SetAction("Ausbildung4");
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd5 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);
if(S42MigrationEqual(stuufe, 20)) {SetGraphics("Future",derclonk); SetPortrait("FutureP", derclonk, HZCK);}

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), nil))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +15, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +14, PHYS_StackTemporary, derclonk);
}

//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(15*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(14*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);
}


//Halbe Zeit, doppelte Werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(15*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(14*2), PHYS_StackTemporary, derclonk);
}

derclonk->~TimerAusbildung();
}



func Stufe16bis20Arbeiter(idImage, clonk)
{

//soll 10000 Frames dauern  -> Delay 10
//Werte so wie 5bis10, aber Zeit anders

//Erfahrungsabfrage, 500 pro Stufe, also erfahrung5
if(clonk)
{
var aktuellestufe = derclonk->~AktuelleStufe();
var erfahrung=derclonk->~RundenErfahrung();
if((erfahrung ?? 0) < (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*5+(erfahrung5 ?? 0)*((aktuellestufe ?? 0)-14) || ((GetWealth(GetOwner(derclonk)) ?? 0)< (kosten5 ?? 0))) {AusbildungsMenu(idImage,derclonk); return(PlayerMessage(GetOwner(derclonk),Format("Für diese Stufe brauchst du mindestens %v Erfahrung und 500 Clunker!", (4*(erfahrung1 ?? 0)+(erfahrung2 ?? 0))+(erfahrung3 ?? 0)*5+(erfahrung4 ?? 0)*5+(erfahrung5 ?? 0)*((aktuellestufe ?? 0)-14),kosten5),derclonk));}
}

//Geldabzug, wenn timer==0, weil dann das erste mal begonnen:
if(S42MigrationEqual(derclonk->~StandTimerAusbildung(), nil))
{ SetWealth(GetOwner(derclonk),(GetWealth(GetOwner(derclonk)) ?? 0)- (kosten5 ?? 0));
  Sound("Cash");}

if(clonk) {aktiveausbildung=1;HauptMenu(clonk,1);}  // sobald action gestartet wird clonk=0, daher wirds nur einmal ausgeführt

if (!(S42MigrationEqual(GetAction(), "AusbildungArbeiter4"))) SetAction("AusbildungArbeiter4");
aktiveausbildung=1;
if((derclonk->~StandTimerAusbildung() ?? 0)>=(zeitd5 ?? 0)) {derclonk->~Stufenanstieg();var stuufe=derclonk->~AktuelleStufe();PlayerMessage(GetOwner(derclonk),Format("Dein Clonk hat Stufe %v erreicht!", stuufe),derclonk); SetAction("Idle");derclonk->~TimerAusbildungNull();aktiveausbildung=nil;CloseMenu(derclonk);
if(S42MigrationEqual(stuufe, 17))derclonk->~NunBauarbeiter(); //arbeiterstufe5
if(S42MigrationEqual(stuufe, 20))derclonk->~NunBauarbeiter(); //arbeiterstufe6

if ((GetEnergy(derclonk) ?? 0) < (GetPhysical("Energy",nil,derclonk) ?? 0)/1000)
  {
var lebenspunkteheilen = (((GetPhysical("Energy",nil,derclonk) ?? 0)/1000)-(GetEnergy(derclonk) ?? 0));
DoEnergy(+(lebenspunkteheilen ?? 0),derclonk);
  }

 return(nil);}

if(S42MigrationEqual(ObjectCount(PP2P), nil))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +20, PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +15, PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +2, PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +4, PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +0, PHYS_StackTemporary, derclonk);
}

//Nur ein Durchlauf, daher alles aufeinmal
if(S42MigrationEqual(ObjectCount(PP2P), 1))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(15*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(2*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(0*(stzeitd5 ?? 0)), PHYS_StackTemporary, derclonk);
}


//Halbe Zeit, doppelte Werte
if(S42MigrationEqual(ObjectCount(PP2P), 2))
{
 SetPhysical("Energy", (GetPhysical("Energy", PHYS_Current, derclonk) ?? 0) +(20*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Breath", (GetPhysical("Breath", PHYS_Current, derclonk) ?? 0) +(15*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Walk", (GetPhysical("Walk", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Jump", (GetPhysical("Jump", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Scale", (GetPhysical("Scale", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Hangle", (GetPhysical("Hangle", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Dig", (GetPhysical("Dig", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Swim", (GetPhysical("Swim", PHYS_Current, derclonk) ?? 0) +(2*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Throw", (GetPhysical("Throw", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Push", (GetPhysical("Push", PHYS_Current, derclonk) ?? 0) +(4*2), PHYS_StackTemporary, derclonk);

 SetPhysical("Fight", (GetPhysical("Fight", PHYS_Current, derclonk) ?? 0) +(0*2), PHYS_StackTemporary, derclonk);
}


derclonk->~TimerAusbildung();
}





func InfoMenu(idImage, clonk)
{
  CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
  AddMenuItem("Spezialisierungen", "Spezialisierungsmenu", RSR4, clonk, nil, clonk,"Informationen zu den Ausbildungsstufen");
  AddMenuItem("Fähigkeiten", "SkillInfo", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
}

func Spezialisierungsmenu(idImage, clonk,zahl)
{
  CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
  SelectMenuItem(zahl,derclonk);
  AddMenuItem("Soldat", "SpezialisierungsInfo", S_XY, clonk, nil, clonk,"Informationen zu den Ausbildungsstufen");
  AddMenuItem("Arbeiter", "SpezialisierungsInfo", X_12, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
}


func SpezialisierungsInfo(idImage, clonk)
{
if(S42MigrationEqual(idImage, S_XY))
{
  MessageWindow("Der Soldat ist für den Kampf, aber weniger fürs Bauen geeignet. Während der Ausbildung steigen seine Kampffähigkeiten stark an und er kann die Hazardwaffen benutzten. Dafür gräbt, baut und baumfällt er relativ langsam. Außerdem kann er nur maximal 2 Items tragen (Ausnahme wenn Waffen dabei sind). Der Soldat kann auf doppellinks/rechts Lebewesen kicken. Ansonsten unterscheidet er sich nicht vom Arbeiterclonk.", GetOwner(clonk));
//Spezialisierungsmenu(idImage, clonk,0);
  return(1);}

if(S42MigrationEqual(idImage, X_12))
{
  MessageWindow("Der Arbeiter ist fürs Bauen, aber weniger fürs Kämpfen geeignet. Er kann bis Stufe 15 keine Munition aufnehmen, ab Stufe 15 kann er nur mit der Ausrüstung Gürtel für Munition ein wenig Munition tragen und seine Kampffähigkeiten werden nicht weiter gesteigert. Dafür gräbt er deutlich schneller als ein Soldat, fällt Bäume schneller, gibt Baustellen einen Fertigstellungs-Boost und kann bis zu 5 Items tragen (je nach aktueller Stufe, aber nur 2 Munitionskisten). Ansonsten unterscheidet er sich nicht vom Soldat.", GetOwner(clonk));
//Spezialisierungsmenu(idImage, clonk,1);
  return(1);}

}

func SkillInfo(idImage, clonk,zahl)
{
  CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
  SelectMenuItem(zahl,derclonk);
  AddMenuItem("Backflip", "DerBackflip", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
  AddMenuItem("Ausrüstung", "DieGear", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
  AddMenuItem("Munition", "DieMunition", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
  AddMenuItem("Waffen", "DieWaffen", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
  AddMenuItem("Upgrades", "DieUpgrades", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
  AddMenuItem("Drohne", "DieDrohne", RSR4, clonk, nil, clonk,"Informationen zu neuen Fähigkeiten des Clonks");
}

func DerBackflip(idImage, clonk)
{
  MessageWindow("Ab Stufe5. Während der Clonk springt, kannst du einen weiteren Sprung in der Luft ausführen, indem du doppelt nach oben/link/rechts drückst. Dadurch kannst du höhere Hindernisse überspringen, weiter springen oder doch schnell wieder zurückspringen", GetOwner(derclonk));
//SkillInfo(idImage, derclonk,0);
  return(1);
}

func DieGear(idImage, clonk)
{
  MessageWindow("Ab Stufe5. Es gbit Ausrüstungsgegenstände, die über doppelgraben oder dem Contextmenü angelegt werden können. Diese verschwinden dann aus dem Inventar und der Clonk hat sie angelegt. Von jeder Art, z.b Rüstung, kann nur ein Gegenstand gleichzeitig angelegt werden. Manche Rüstungen haben außerdem die Möglichkeit sie zu aktiveren. Dies und auch das Ablegen der Ausrüstung funktioniert über das Kontextmenü.", GetOwner(clonk));
//SkillInfo(idImage, clonk,1);
  return(1);
}

func DieMunition(idImage, clonk)
{
  MessageWindow("Ab Stufe5. Munitionskisten können über doppelgraben entpackt und die Munition angelegt werden. Dadurch verschwindet die Kiste aus dem Inventar und die Munition wird für jeden Clonk einzeln links angezeigt. Diese wird dann für Waffen verwendet. Über das Kontextmenü erreicht man den Munitionsgürtel. Dort kann die Munition wieder in Munitionskisten verpackt werden, wodurch andere Clonks sie aufnehmen können, oder sie verkauft werden kann.", GetOwner(clonk));
//SkillInfo(idImage, clonk,2);
  return(1);
}

func DieWaffen(idImage, clonk)
{
  MessageWindow("Ab Stufe5, abfeuern nur Soldaten. Waffen brauchen Munition, die immer erst eingeladen werden muss. Je nach Waffe dauert das einige Zeit und die Menge ist auch unterschiedlich. Mit doppelgraben kann man den Modus der Waffe wechseln, sofern mehr als einer vorhanden ist. Über Upgrades können je nach Waffe neue Modi hinzugefügt werden.", GetOwner(clonk));
//SkillInfo(idImage, clonk,3);
  return(1);
}

func DieUpgrades(idImage, clonk)
{
  MessageWindow("Ab Stufe5. Es gibt 4 Upgrades, welche auch (Upgrade) im Namen tragen: Laser, Waffenteil, Bioaufsatz und Partikelkonverter. Meist steht bei den Waffen dabei, womit sie aufgerüstet werden können. Hat man ein Upgrade und eine Waffe dabei, wechselt man das Inventar, sodass das Upgrade vorne ist und drückt doppelgraben. In dem erscheinenden Menü wählt man die Waffe aus und fertig. Nun kann man den neuen Modus verwenden. ", GetOwner(clonk));
//SkillInfo(idImage, clonk,4);
  return(1);
}

func DieDrohne(idImage, clonk)
{
  MessageWindow("Ab Stufe5. Wenn du eine Drohne gebaut hast, kannst du sie einsammeln. Um sie zu aktivieren, musst du hier erst eine Waffe geben. Dazu musst du sowohl die Drohne, als auch eine Waffe im Inventar haben und die Drohne an vorderste Stelle wechseln. Drücke nun doppelgraben und rüste so die Drohne mit der Waffe aus. Nun sollte sie bereits aktiviert sein und verfolgt den Clonk, der sie aktiviert hat und bekämpft seine Feinde. Über das Contextmenü lässt sie sich wieder deaktivieren. Wenn der Clonk sich in einem Gebäude befindet, ist die Drohne zurzeit noch überfordert und sie fliegt weg... sollte aber wiederkommen, wenn der Clonk das Gebäude verlässt.", GetOwner(clonk));
//SkillInfo(idImage, clonk,5);
  return(1);
}


func ClonkInfoMenu(idImage, clonk, zahl)
{
  CreateMenu(HZCK, clonk, this(),nil,nil,nil,1);
  SelectMenuItem(zahl,derclonk);
   AddMenuItem("Erfahrung","Experience", S__3, clonk, nil, clonk, "Die Erfahrung, die du allein in diesem Spiel gesammelt hast. Etwas Erfahrung ist Voraussetzung für weitere Ausbildungen. Du kannst deine Erfahrung durch fast jede Tätigkeit erhöhen, Bäume fällen, werfen, schwimmen...");
  AddMenuItem("Energie", "Energieanzeigen", S__8, clonk, nil, clonk,"Energie ist die maximale Lebenspunktanzahl");
  AddMenuItem("Atem", "Atemanzeigen", S__2, clonk, nil, clonk,"Wie lange ein Clonk die Luft anhalten kann, bis er Schaden bekommt.");
  AddMenuItem("Laufen", "Laufenanzeigen", S__5, clonk, nil, clonk,"Wie schnell ein Clonk läuft");
  AddMenuItem("Springen", "Springenanzeigen", S_23, clonk, nil, clonk,"Wie hoch und weit der Clonk springt");
  AddMenuItem("Klettern", "Kletternanzeigen", S_10, clonk, nil, clonk,"Wie schnell ein Clonk klettert");
  AddMenuItem("Hangeln", "Hangelnanzeigen", S_11, clonk, nil, clonk,"Wie schnell ein Clonk hangelt");
  AddMenuItem("Graben", "Grabenanzeigen", S_20, clonk, nil, clonk,"Wie schnell ein Clonk graben kann");
  AddMenuItem("Schwimmen", "Schwimmenanzeigen", S__1, clonk, nil, clonk,"Wie schnell ein Clonk schwimmen kann");
  AddMenuItem("Werfen", "Werfenanzeigen", S_21, clonk, nil, clonk,"Wie weit ein Clonk wirft");
  AddMenuItem("Schieben/Drücken", "Schiebenanzeigen", S_22, clonk, nil, clonk,"z.b fürs Schieben von schweren Loren wichtig");
  AddMenuItem("Kämpfen", "Kampfanzeigen", S_12, clonk, nil, clonk,"Wieviel Schaden der Clonk im Nahkampf macht");
  AddMenuItem("Spezialisierung und Stufe", "Spezialanzeigen", CLNK, clonk, nil, clonk,"Zeigt dir Spezialisierung und Stufe dieses Clonks");
  SelectMenuItem(zahl,clonk); //damit menü wieder an derselben stelle aufgerufen wird
}

func Spezialanzeigen(idImage, clonk)
{ 
var aktuellestufe = derclonk->~AktuelleStufe();
if(clonk->~IstBauarbeiter())
{PlayerMessage(GetOwner(clonk),Format("{{X_12}} Arbeiterclonk Stufe %v",aktuellestufe),clonk);ClonkInfoMenu(idImage, clonk,12);return(1);}
if((aktuellestufe ?? 0)>=5) // dann muss er Soldat sein
{PlayerMessage(GetOwner(clonk),Format("{{S_XY}} Soldatclonk Stufe %v",aktuellestufe),clonk);ClonkInfoMenu(idImage, clonk,12);return(1); }
//ansonsten noch keine Spezialisierung
PlayerMessage(GetOwner(clonk),Format("{{CLNK}} Clonk Stufe %v, noch keine Spezialisierung",aktuellestufe),clonk);ClonkInfoMenu(idImage, clonk,12);return(1); 
}

func Energieanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Energy", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S__8}} Energie: %v | Die maximale Lebenspunktanzahl",hp),clonk);ClonkInfoMenu(idImage, clonk,1);return(1);
}

func Atemanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Breath", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S__2}} Atem: %v | Wie lange ein Clonk die Luft anhalten kann, bis er Schaden bekommt.",hp),clonk);ClonkInfoMenu(idImage, clonk,2);return(1);
}

func Laufenanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Walk", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S__5}} Laufen: %v | Wie schnell ein Clonk läuft",hp),clonk);ClonkInfoMenu(idImage, clonk,3);return(1);
}

func Springenanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Jump", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_23}} Springen: %v | Wie hoch und weit der Clonk springt",hp),clonk);ClonkInfoMenu(idImage, clonk,4);return(1);
}

func Kletternanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Scale", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_10}} Klettern: %v | Wie schnell ein Clonk klettert",hp),clonk);ClonkInfoMenu(idImage, clonk,5);return(1);
}

func Hangelnanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Hangle", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_11}} Hangeln: %v | Wie schnell ein Clonk hangelt",hp),clonk);ClonkInfoMenu(idImage, clonk,6);return(1);
}

func Grabenanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Dig", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_20}} Graben: %v | Wie schnell ein Clonk graben kann",hp),clonk);ClonkInfoMenu(idImage, clonk,7);return(1);
}

func Schwimmenanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Swim", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S__1}} Schwimmen: %v | Wie schnell ein Clonk schwimmen kann",hp),clonk);ClonkInfoMenu(idImage, clonk,8);return(1);
}

func Werfenanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Throw", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_21}} Werfen: %v | Wie weit ein Clonk wirft",hp),clonk);ClonkInfoMenu(idImage, clonk,9);return(1);
}

func Schiebenanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Push", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_22}} Schieben: %v | z.b fürs Schieben von schweren Loren wichtig",hp),clonk);ClonkInfoMenu(idImage, clonk,10);return(1);
}

func Kampfanzeigen(idImage, clonk) 
{
var hp = GetPhysical("Fight", nil, clonk); PlayerMessage(GetOwner(clonk),Format("{{S_12}} Kampf: %v | Wieviel Schaden der Clonk im Nahkampf macht",hp),clonk);ClonkInfoMenu(idImage, clonk,11);return(1);
}

public func Experience(idImage, clonk)
{
  PlayerMessage(GetOwner(clonk),"{{S__3}} Erfahrung : | %d ",clonk, clonk->~RundenErfahrung());ClonkInfoMenu(idImage, clonk,nil);
return(1);
}






//Forschungsvoraussetzung: stattdessen wie bei allen anderen in forschungsbaum skripten
//public func GetResearchBase() { return(HUT3); }
//public func GetResearchBase1() { return(CNKT); }
//public func GetResearchBase2() { return(ANVL); }
//public func GetResearchBase3() { return(FACK); }
//public func GetResearchBase4() { return(LNKT); }
//public func GetResearchBase5() { return(FRGE); }
//public func GetResearchBase6() { return(TENP); }
//public func GetResearchBase7() { return(FDRS); }
//public func GetResearchBase8() { return(AEHN); }
//public func GetResearchBase9() { return(_BMR); }
//public func GetResearchBase10() { return(WDBR); }
//public func GetResearchBase11() { return(SIGN); }
//public func GetResearchBase12() { return(ELEV); }
//public func GetResearchBase13() {if(!FindObject2(Find_ID(PPP2))) return(KA_S); return(0); }

