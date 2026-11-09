/*-- Spielzielauswahl, basierend auf RdW--*/

#strict 3
// dieses Skript ruft in szenario und allen regel/ziel skripten ChooserFinished() auf, wenn settings das erste mal abgeschlossen wurden
// evlt besser durch eine globale variable ersetzen, damit auch objekte die danach erst hinzukommen, bescheid wissen, dass settings bereits fertig sind.


// noch zufügen: 
// Infobereich, wie funktioniert und dass es standard gibt
// Standard wieder herstellen
// Kurzübersicht über alle variablen Einstellungen
// eigenen Bereich für verschmutzungseinstellungen und AntiLag

//C4D_Rule
static const Chooser_Cat_Rule = 524288;
static const Chooser_Cat_Environment = 64;
static const Chooser_Cat = 524384;


func IsNotChooseable() {return(1);}

/* Initialisierung */

local aRules,aEnvironment, pGoal, iTechLevel, aGoals, Death,aEffekte;
local idDifficulty;
local aTempGoalSave;
local iStartGold;
local wissenvorher;
static regelkontrolle,Difficulty;
static hashSpielRegeln;
static hashExplosivErdschaden;
static gForschungverbotenlisteVariable;
static gForschungerforschtlisteVariable;
static gSzenarioStandardForschungFreiwilligVariable;

local revdauer, techlevel, nahrung, forschung,verschmutzung;  // mit standardwerten
global func GetRevTime()      {if(!GetDifficultyObj()) return(100);  return GetDifficultyObj()->~GetRevTime();      } 
global func GetScienceCost()  {if(!GetDifficultyObj()) return( 50);  return GetDifficultyObj()->~GetScienceCost();  } // alle 10x 1 € abziehen
global func GetTechLevel()    {if(!GetDifficultyObj()) return(  3);  return GetDifficultyObj()->~GetGetTechLevel(); }
global func GetFood()         {if(!GetDifficultyObj()) return(  2);  return GetDifficultyObj()->~GetFood();         }	
global func GetPollutionPerc(){if(!GetDifficultyObj()) return(100);  return GetDifficultyObj()->~GetPollutionPerc();}
global func GetDifficulty()   {if(!GetDifficultyObj()) return(  3);  return GetDifficultyObj()->~IsDifficultyLevel();}

/* Nur eine Mitteilung fnr Neugierige */
func Activate(iPlr)
{
  if(And(Death, (FrameCounter() ?? 0)>1000)) return   (MessageWindow(GetDesc(),iPlr));
  if(Equal(iPlr, GetPlayerByIndex(nil,C4PT_User))) return(OpenMenu());
  MessageWindow(GetDesc(),iPlr);
}

global func GetDifficultyObj()
{
  if(Difficulty) return Difficulty;
  return Difficulty=FindObject2(Find_Func("IsDifficultyLevel"));
}

// Timercall
func Timer()
{
if (And(Inside(ScriptCounter(),2,10), !Death))
ScriptGo(nil);
}

func Initialize()
{
  if(!GetLength(hashExplosivErdschaden))
  hashExplosivErdschaden = CreateHash(); //funktionsweise: HashPut(hashSpielRegeln, key, value) bzw. HashGet(hashSpielRegeln,key)
  if(!GetLength(hashSpielRegeln))
  hashSpielRegeln = CreateHash(); //funktionsweise: HashPut(hashSpielRegeln, key, value) bzw. HashGet(hashSpielRegeln,key)
  if(Death) return(1);
  regelkontrolle=this(); // das in allen regeln, environment usw. abfragen, ob regelkontrolle active ist. So lange dürfen sie nichts machen und erst danach initialisieren usw.
  SetPosition();
  aRules = CreateArray();
  aEnvironment = CreateArray();
  aEffekte = CreateArray();
  aGoals = CreateArray();
  aTempGoalSave = CreateArray();
  if(!CampaignMode())// Spielziele bald entfernen
    ScheduleCall(this(), "RemoveGoals", 2);
  iTechLevel=GameCall("ForcedTechLevel");
  if(!iTechLevel)
    iTechLevel=1;
ScheduleCall(this(), "Initialized", 1);
// jetzt noch defaultwerte geben
}

func Initialized()
{
  // Spielgeschwindigkeit verlangsamen
 SetGameSpeed(10);
 SetStandardEinstellungen();

// Umweltschutzobjekt erstellen
  if(!FindObject(_Z2A))
    CreateObject(_Z2A,nil,nil,-1);
}

func SetExplosivStandard()
{
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i) ; S42MigrationStep(i, 1, true))
  {
   if(DefinitionCall(idR, "IsExplosiv") )
   {
    if(!GetLength(hashExplosivErdschaden)) Log("ExplosivHash noch nicht erstellt!");
    else{
    HashPut(hashExplosivErdschaden, Format("Erdschaden %v",idR), nil);
    if(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(And(!Equal(idR, SSPR), !Equal(idR, SP02)), !Equal(idR, SB52)), !Equal(idR, JB4K)), !Equal(idR, ST_3)), !Equal(idR, AMIN)), !Equal(idR, P__2)), !Equal(idR, PTBL)), !Equal(idR, CEH2)), !Equal(idR, CENM)), !Equal(idR, CVLA)), !Equal(idR, SBST)), !Equal(idR, CAN_)), !Equal(idR, CCD_)), !Equal(idR, CGC_)), !Equal(idR, MAM_)), !Equal(idR, PRE_)), !Equal(idR, SCR_)), !Equal(idR, TSP_)), !Equal(idR, TCN_)), !Equal(idR, ALN4)), !Equal(idR, MINE)), !Equal(idR, LMIN)), !Equal(idR, SMI1)), !Equal(idR, SWRM)), !Equal(idR, ABMB)), !Equal(idR, DRSU)), !Equal(idR, MISS)), !Equal(idR, MIS_)), !Equal(idR, IIII)), !Equal(idR, CGRN)), !Equal(idR, CGFR)), !Equal(idR, GREN)), !Equal(idR, TBUL)), !Equal(idR, SAM1)), !Equal(idR, SAM2)), !Equal(idR, SAML)), !Equal(idR, SAM3)), !Equal(idR, STR0)), !Equal(idR, MROK)), !Equal(idR, SHRF)), !Equal(idR, GRN2)), !Equal(idR, TAUC)), !Equal(idR, TAUV)), !Equal(idR, EFL6)), !Equal(idR, SFLF)), !Equal(idR, SFLI)), !Equal(idR, C_S7)), !Equal(idR, CMM_)), !Equal(idR, HMI_)), !Equal(idR, HMIS)), !Equal(idR, NGRN)), !Equal(idR, CBB_)))
    HashPut(hashExplosivErdschaden, Format("Erdschaden %v",idR), 1);} // all die gelisteten ids sind von beginn an erdzerstörend
   }
  }
}


func SetStandardEinstellungen(dummy,iSelection) // ein teil wird auch weiter unten in after initialize player gemacht, weil es auch bei Laufzeitbetritt gemacht werden muss
{

    var SzenarioNoDefaultListe = GameCall("ScenarioNoDefault"); // eine liste in der alle IDs von regeln und umgebungsobjeten drin ist, die in diesem szenario nicht als standard eingestellt sein sollen.
    if (!SzenarioNoDefaultListe) SzenarioNoDefaultListe=[];
    
   SetExplosivStandard();
// Standardwerte für die statischen Dinge. Auch vor Spielstart eingestelle Regeln, werden aufgelistet und markiert. Zusätzlich werden bei Finish alle Regeln entfernt, die vorhanden aber nicht markiert sind.
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat_Rule) ; S42MigrationStep(i, 1, true))
  {
   if(Or((And(DefinitionCall(idR, "IsDefaultRule"), (!InArray(SzenarioNoDefaultListe, idR)))), FindObject2(Find_ID(idR),Find_Exclude(this()))) ) // IsDefaultRule wird in einer system datei den regeln zugeordnet.
    aRules[(i ?? 0)]=true;
   else
    aRules[(i ?? 0)]=nil;
  }
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat_Environment) ; S42MigrationStep(i, 1, true))
  {
   if(Or((And(DefinitionCall(idR, "IsDefaultEnvironment"), (!InArray(SzenarioNoDefaultListe, idR)))), FindObject2(Find_ID(idR),Find_Exclude(this()))) )
    aEnvironment[(i ?? 0)]=true;
   else
    aEnvironment[(i ?? 0)]=nil;
  }

// Standardwerte für die variablen Dinge

var SzenarioStandard = GameCall("ScenarioStandardEinstellungen"); // eine liste der Form [["Bausatz",100],["Baustoffe",0],["Ausbildungserfahrung",1],["Ausbildungskosten",1],["StartZeitalter",22]]
if (!SzenarioStandard) SzenarioStandard=[];

var Standardhier = [["Bausatz",50],["Baustoffe",nil],["Ausbildungserfahrung",200],["Ausbildungskosten",100],["Ausbildungszeit",10],["Forschungskosten",50],["Forschungszeit",50],["SichtradiusTagHimmel",400],["SichtradiusNachtHimmel",200,
"SichtradiusNachtTunnel",100],["SichtradiusTagTunnel",140],["SichtradiusVerhalten",1],["Verdunstennormal",nil],["Verdunstenwarm",nil],["Verdunstenheiß",nil],["Umweltbelastbarkeit",1500],["StartZeitalter",nil],["Produktionsgeschwindigkeit",100],
["Baustellenboost",nil],["Grundwasser",10],["Produktionskosten",nil],["Baustellenkosten",nil],["Baustellenkostenerstattung",nil],["Dunkelheitsgrad",100],["EnergieverbrauchNeue",100],["BlitzeModifier",100],["Barriere",nil],
["AbbauEreignisse",[["Ashes","Acid","Alumina","Coal","Crystal","Earth","Gold","Lava","Oil","Ore","Rock","Sand","Snow","Sulphur","TitaniumOre","Uranore","Water"],[ASHS,ABRL,CALO,COAL,CRYS,ERTH,GOLD,DBRL,OBRL,ORE1,ROCK,SAND,SNWB,SPHR,CTIO,SURO,WBRL],[nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil,nil],nil]]
];

var Standardlist = [];
var enthalten = nil;
for(var teilhier in Standardhier)
{
    enthalten = nil;
    for(teilszen in SzenarioStandard)
    {
        if(Equal(teilhier[0], teilszen[0]))
            enthalten = teilszen ;
    }
    if(enthalten) // wenn es in szenariolist enthalten ist, dann das vom szenario nehmen
        ArrayAdd(Standardlist, enthalten, 1); // die 1 verhindert doppelte einträge, auch wenn es hier eig nicht moeglich sein sollte
    else
        ArrayAdd(Standardlist, teilhier, 1);
}

// Log(Format("%v",Standardlist));

for(var regel in Standardlist)
{
    HashPut(hashSpielRegeln,regel[0],regel[1]);
}

// nun die Standard Forschungsdinge:
var SzenarioStandardForschung = GameCall("ScenarioForschungsEinstellungen"); // eine liste der Form [["Verboten",["leer"]],["Freiwillig",["leer"]],["Erforscht",["leer"]]], wobei "leer" eig eine auflistung von IDs ist. wenn "leer" oder nicht existent, dann standard verwenden
if (!SzenarioStandardForschung) SzenarioStandardForschung=[["Verboten",["leer"]],["Freiwillig",["leer"]],["Erforscht",["leer"]]]; // nicht existent, standard nehmen

// freiwilligenliste ergänzen
if (Equal(!SzenarioStandardForschung[1][1][0], "leer"))
    gSzenarioStandardForschungFreiwilligVariable = SzenarioStandardForschung[1][1]; // wird im forschungsskript zusammen mit den standardfreiwilligen forschungen returnd
else
    gSzenarioStandardForschungFreiwilligVariable = [];

if (Equal(SzenarioStandardForschung[0][1][0], "leer"))
    gForschungverbotenlisteVariable = [];
else
    gForschungverbotenlisteVariable = SzenarioStandardForschung[0][1];
if (Equal(SzenarioStandardForschung[2][1][0], "leer"))
    gForschungerforschtlisteVariable = [];
else
    gForschungerforschtlisteVariable = SzenarioStandardForschung[2][1]; // wird nur verwendet, wenn regel fuer szenario forschungsbaum aktiv ist, sonst wird normale start zeitalter verwendet.


// Regelmenn wieder aufmachen
if(iSelection)  
  OpenMenu();
}

func RemoveGoals()
{
  if(!FindObject(SZGO)) // wenn diese regel aktiv (muss also in szenario.txt eingestellt worden sein), dann die ziele nicht loeschen und mit diesem chooser nicht veränderbar machen!
  {
      // Spielziele entfernen
      for(var goal in FindObjects(Find_Category(C4D_Goal)))
        goal->RemoveObject();
      for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, C4D_Goal) ; S42MigrationStep(i, 1, true))
        if(DefinitionCall(idR, "IsChooseableGoal"))
          aGoals[(GetLength(aGoals) ?? 0)] = idR;
  }
  
  OpenMenu();
  // Menn wird einfach nochmal geöffnet, diesmal richtig!
}

/* Spielerinitialisierung */

func InitializePlayer(int iPlr)
{
  iPlr ??= 0;
  ScheduleCall(this(), "AfterInitializedNowPlayer", 8,nil,iPlr); // 5 frames warten, damit stadnards auch schon gesetzt sind
}

func AfterInitializedNowPlayer(int iPlr)
{
    // Log("AfterInit");
    // folgende Dinge immer fuer jeden Spieler machen, der Beitritt, auch Laufzeitbeitritt
    iPlr ??= 0;
    var aSzenarioAdditionalBuys = GameCall("ScenarioHomeBase"); // szenariospezifische zusätzliche Kaufobjekte, wenn nicht in szenario.txt eingestellt werden soll
    if (!aSzenarioAdditionalBuys) aSzenarioAdditionalBuys=[];
    for(var entry in aSzenarioAdditionalBuys)
    {
        MakeWare(entry[0],entry[1],1,(iPlr ?? 0)+1); // iPlr + 1 ,damit es immer wahr ist und erkannt wird, wann es fuer alle und wann nur fuer einen gemacht werden soll
    }
    var aSzenarioAdditionalBuysProd = GameCall("ScenarioHomeBaseProduction"); // szenariospezifische zusätzliche Kaufobjekte, wenn nicht in szenario.txt eingestellt werden soll
    if (!aSzenarioAdditionalBuysProd) aSzenarioAdditionalBuysProd=[];
    for(var entry in aSzenarioAdditionalBuysProd)
    {
        MakeWareProduction(entry[0],entry[1],1,(iPlr ?? 0)+1);
    }
    
    // Die forschungen verteilen
    // diese dinge koennen in settings eh nicht verändert werden, also im init machen und nicht im finisher.
    // auch das zufügen der Regel während des Spiels wäre zu umständlich. Man muesste alles löschen und neu vergeben.. und man müsste einen call an scenario.txt machen um die urpsürnglich eingestellten Dinge zu bekommen.
    if (FindObject(SZRE)) // wenn es da ist, dann die im szenario skript erwähnten standardforschungen erforschen. eig nur verwenden, wenn kein forschungslabor in mission gebaut werden kann, aber das muss dann halt manuell berücksichtigt werden.
    {
           for(var element in gForschungerforschtlisteVariable)
           {
                SetPlrKnowledge(iPlr, element); // gibt jedem spieler zu den szenario.txt eingestellten Forschungen noch welche obendrauf, welche im szenario skript definiert werden
           }    
    }
    

  if(Death)
  {
    // das Startzeitalter wird im setting finisher eh einmal gesetzt, da es in den einstellungen ja noch verändert werden kann. Daher muss das nur gemacht werden, wenn laufzeitbeitritt
    if (And(!FindObject(SZRE), !FindObject(TKNW))) // das startzeitalter kann aber noch verändert werden, daher muss es zusätzlich auch im finisher der regeln einmal gemacht werden
    {  // auch bei Teamknowledge Regel bei laufzeitbeitritt nichts verändern!
        var num, id;
          num = nil;
          while (id = GetDefinition(S42MigrationStep(num, 1, true))) SetPlrKnowledge(iPlr, id,1); // das gesamte wissen entfernen und danach neu verteilen
        GlobalStartingEra(HashGet(hashSpielRegeln, "StartZeitalter")); // die Funktionen für das StartZeitalter aufrufen, welche sich in den forschungsbaum skripten befinden
    }
    // auch diese Regel kann noch aktiviert werden
    if(FindObject(CHHH)) // wenn Regel, dass alle Chemiedinge immer erforscht sein sollen, dann diese Forschungen auch noch vergeben. Kann nicht rückgängig gemacht werden, zu kompliziert
    {
        var Chemieliste = FindObject(CHHH)->ChemieForschungen();
        for(var element in Chemieliste)
       {
            SetPlrKnowledge(iPlr, element);
       }
    }
  }
  if(!Death) // nur beim ersten mal machen
  {
      // Alle Clonks des Spielers verstauen
      for(var i=nil, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; S42MigrationStep(i, 1, true))
        {
        tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
        SetCategory((GetCategory(tmp) ?? 0) | (C4D_Foreground ?? 0),tmp);
        SetGraphics(nil, tmp, GetID(pCrew), 1, 5, nil, 1, pCrew);
        Enter(tmp, pCrew);
        Eastern(tmp);
        }

      // Spieler 1? Dann Menn öffnen
      if(Equal(GetPlayerByIndex(nil,C4PT_User), iPlr))
      {
        Log("$ChoosingPlayer$", GetPlayerName(iPlr));
        return(OpenMenu());
      }
  }
}

/* Hauptmen */

func OpenMenu()
{
  wissenvorher = HashGet(hashSpielRegeln, "StartZeitalter"); 
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  if(!pClonk) return(ScheduleCall(this(), "OpenMenu", 1));
  //if(GetLength(aGoals)) return(OpenGoalChooseMenu());
  if(CampaignMode()) return OpenDifficultyMenu();

  if(GetMenu(pClonk))
    CloseMenu(pClonk);

  CreateMenu(GetID(), pClonk, nil, nil, nil, nil, 1);
  // Infos
  AddMenuItem("Infos", "Informationen", CHO6, pClonk,nil,nil,"Infos");
  // Spielregeln
  AddMenuItem("$CreateRules$", "Wahlmenu", RUSC, pClonk, nil,nil, "$RuleInfo$");
  // Umgebungsobjekte
  AddMenuItem("$CreateEnvironment$", "Wahlmenu", EVSC, pClonk, nil,nil, "$EnvironmentInfo$");
  // Spielziele
  if(!FindObject(SZGO))   AddMenuItem("$GoalChose$", "OpenGoalChooseMenu", GOSC, pClonk,nil,nil,"$GoalChose$"); // ziele nur veränderbar, wenn diese regel nicht da,regel sorgt dafuer, dass szenario ziele verwendet werden.
  // Effekte reduzieren
  AddMenuItem("Effekte reduzieren", "SetEffekte", ANL_, pClonk,nil,nil,"Hier können die grafischen Effekte reduziert werden, damit das Spiel eventuell flüssiger läuft");
  // Kaufbare Dinge manipulieren
  AddMenuItem("Kaufbare Dinge einstellen", "BuyableMenu", CNMT, pClonk,nil,nil,"Stelle ein was kaufbar sein soll und wieviel");
  // Standard
  AddMenuItem("Alles zurück auf Standard", "SetStandardEinstellungen", CHO6, pClonk,nil,1,"Setzt alle Einstellungen wieder zurück auf die Standardwerte");
  // Fertig
  AddMenuItem("$Finished$", "ConfigurationFinished", CNCL, pClonk,nil,nil,"$Finished$");
}

func Informationen(dummy, iSelection)
{
// ...
return(OpenMenu());
}

func SetEffekte(dummy, iSelection)
{
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk);
  // Regeln einfngen
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i,Chooser_Cat_Rule) ; S42MigrationStep(i, 1, true))
    if(DefinitionCall(idR, "IsAntiLagChooseable"))
      {
      def = TransformObject(idR,aEffekte[(i ?? 0)]);
      AddMenuItem("%s", "ChangeEffekteConf", idR, pClonk, nil, i, nil, 4, def);
      if(Equal(i, iSelection))
        check = true;
      if(!check)
        S42MigrationStep(j, 1, true);
      RemoveObject(def);
      }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func ChangeEffekteConf(id idEffekte, int i)
{
  // Regel wurde ausgewShlt
  i ??= 0;
  if(!aEffekte[(i ?? 0)])
  {
    aEffekte[(i ?? 0)] = true;
    if(Or(idEffekte->~CreateOnChoose(), Death))
      CreateObject(idEffekte,nil,nil,-1);
  }
  // Regel wurde abgewShlt
  else
  {
    aEffekte[(i ?? 0)] = nil;
    if(Or(idEffekte->~CreateOnChoose(), Death))
      if(FindObject(idEffekte))
        RemoveObject(FindObject(idEffekte));
  }
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  // Regelmenn wieder aufmachen
  SetEffekte(nil, i);
}

func BuyableMenu(dummy,iSelection)
{
var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk, nil, nil, nil, nil, 1);
AddMenuItem("Kaufbare Objektmengen verändern","SetBuyable",CNMT, pClonk,nil,nil,"Stelle ein was kaufbar sein soll und wieviel. Betrifft nur Spieler die aktuell im Spiel sind!");
AddMenuItem("Nachlieferungen aus Heimatbasis verändern","SetProduction",CNMT, pClonk,nil,nil,"Stelle ein wovon wie stark nachgeliefert werden soll. Betrifft nur Spieler die aktuell im Spiel sind!");
AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
}

private func SetProduction(idItem,izahl)
{
 var pClonk=GetCursor(GetPlayerByIndex(nil,C4PT_User));
  CreateMenu(GetID(),pClonk,nil,nil,"nichts einstellbar");
  var Typ,i,iSMI= (-1);
while(Typ= GetDefinition(S42MigrationStep(i, 1, true),C4D_SelectHomebase()))
    {
    S42MigrationStep(iSMI, 1, true);
      {
      S42MigrationStep(iSMI, 1, true);
      AddMenuItem("%s vermindern","MakeWareProduction",Typ,pClonk,GetHomebaseProduction(GetOwner(pClonk),Typ),-1);
      AddMenuItem("%s vermehren","MakeWareProduction",Typ,pClonk,GetHomebaseProduction(GetOwner(pClonk),Typ),1);
      }
    if(And(Equal(idItem, Typ), Equal(izahl, -1))) SelectMenuItem((iSMI ?? 0)-1,pClonk);
    if(And(Equal(idItem, Typ), Equal(izahl, 1))) SelectMenuItem(iSMI,pClonk);
    }
  // Fertig
  AddMenuItem("$Finished$", "BuyableMenu", CNCL, pClonk,nil,1);
}

func MakeWareProduction(ID,izahl,fMenu,iPlrplus1)
{
if(iPlrplus1) // nur fuer einen bestimmten player, wird in afterinitialzie genutzt, zb laufzeitbeitritt. plus1 damit es wenn es null ist fuer alle gemacht wird.
{
    DoHomebaseProduction((iPlrplus1 ?? 0)-1, ID, izahl);
     if(Equal(GetHomebaseProduction((iPlrplus1 ?? 0)-1,ID), -1)) DoHomebaseProduction((iPlrplus1 ?? 0)-1, ID, 1); // nicht negativ
}
else
{
    for(var i=nil; (i ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
     {
     DoHomebaseProduction(GetPlayerByIndex(i), ID, izahl);
     if(Equal(GetHomebaseProduction(GetPlayerByIndex(i),ID), -1)) DoHomebaseProduction(GetPlayerByIndex(i), ID, 1); // nicht negativ
     }
}

 if(!fMenu)
    SetProduction(ID,izahl);
}

private func SetBuyable(idItem,izahl)
{
 var pClonk=GetCursor(GetPlayerByIndex(nil,C4PT_User));
  CreateMenu(GetID(),pClonk,nil,nil,"nichts einstellbar");
  var Typ,i,iSMI= (-1);
while(Typ= GetDefinition(S42MigrationStep(i, 1, true),C4D_SelectHomebase()))
    {
    S42MigrationStep(iSMI, 1, true);
      {
      S42MigrationStep(iSMI, 1, true);
      AddMenuItem("%s vermindern","MakeWare",Typ,pClonk,GetHomebaseMaterial(GetOwner(pClonk),Typ),-1);
      AddMenuItem("%s vermehren","MakeWare",Typ,pClonk,GetHomebaseMaterial(GetOwner(pClonk),Typ),1);
      }
    if(And(Equal(idItem, Typ), Equal(izahl, -1))) SelectMenuItem((iSMI ?? 0)-1,pClonk);
    if(And(Equal(idItem, Typ), Equal(izahl, 1))) SelectMenuItem(iSMI,pClonk);
    }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
}

func MakeWare(ID,izahl,fMenu,iPlrplus1)
{
if(iPlrplus1) // nur fuer einen bestimmten player, wird in afterinitialzie genutzt, zb laufzeitbeitritt
{
    DoHomebaseMaterial((iPlrplus1 ?? 0)-1, ID, izahl);
     if(Equal(GetHomebaseMaterial(iPlrplus1,ID), -1)) DoHomebaseMaterial((iPlrplus1 ?? 0)-1, ID, 1); // nicht negativ
}
else
{
    for(var i=nil; (i ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
     {
     DoHomebaseMaterial(GetPlayerByIndex(i), ID, izahl);
     if(Equal(GetHomebaseMaterial(GetPlayerByIndex(i),ID), -1)) DoHomebaseMaterial(GetPlayerByIndex(i), ID, 1); // nicht negativ
     }
}

 if(!fMenu)
    SetBuyable(ID,izahl);
}

func Wahlmenu(id dummy, int iSelection)
{
iSelection ??= 0;
var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  CreateMenu(GetID(), pClonk, nil, nil, nil, nil, 1);
if(Equal(dummy, EVSC))
 {
  // Spielumgebung
  AddMenuItem("Statische Umgebungsobjekte", "OpenEnvironmentMenu", BRDS, pClonk, nil,nil, "Ein- und Ausschaltbare Objekte");
  AddMenuItem("Variable Umgebungsobjekte", "VariabelEnvironmentMenu", BRDS, pClonk, nil,nil, "Variabel einstellbare Objekte");
 }
if(Equal(dummy, RUSC))
 {
  // Spielregeln
  AddMenuItem("Statische Regeln", "OpenRuleMenu", CTFL, pClonk, nil,nil, "Ein- und Ausschaltbare Regeln");
  AddMenuItem("Variable Regeln", "VariabelRuleMenu", CTFL, pClonk, nil,nil, "Variabel einstellbare Regeln");
 }
  // Fertig
    AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
}

/* Regelmenn */

func OpenRuleMenu(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk);
  // Regeln einfngen
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat_Rule) ; S42MigrationStep(i, 1, true))
    if(And(!DefinitionCall(idR, "IsNotChooseable"), !DefinitionCall(idR, "IsVariabelChooseable2"))) // notchoosable soll verhindern, dass bestimmte regeln in den statischen regeln auftauchen, zb angriffsregel usw.
      {
      def = TransformObject(idR,aRules[(i ?? 0)]);
      AddMenuItem("%s", "ChangeRuleConf", idR, pClonk, nil, i, nil, 4, def);
      if(Equal(i, iSelection))
        check = true;
      if(!check)
        S42MigrationStep(j, 1, true);
      RemoveObject(def);
      }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func ChangeRuleConf(id idRule, int i)
{
  // Regel wurde ausgewShlt
  i ??= 0;
  if(!aRules[(i ?? 0)])
  {
    aRules[(i ?? 0)] = true;
    if(Or(idRule->~CreateOnChoose(), Death))
      CreateObject(idRule,nil,nil,-1);
  }
  // Regel wurde abgewShlt
  else
  {
    aRules[(i ?? 0)] = nil;
    if(Or(idRule->~CreateOnChoose(), Death))   //
      if(FindObject(idRule))
        RemoveObject(FindObject(idRule));
  }
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  // Regelmenn wieder aufmachen
  OpenRuleMenu(nil, i);
}

func VariabelRuleMenu(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk,nil,nil,"Variable Regeln",nil,1,nil);
  // Regeln einfngen
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat_Rule) ; S42MigrationStep(i, 1, true))
    if(DefinitionCall(idR, "IsVariabelChooseable2")) // !DefinitionCall(idR, "IsNotChooseable") && 
      {
      AddMenuItem("%s", "EditVariabelRule", idR, pClonk);
      }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
}

func EditVariabelRule(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  wissenvorher = HashGet(hashSpielRegeln, "StartZeitalter");   
  // Menü aufmachen
  CreateMenu(GetID(), pClonk,nil,nil,"Variable Einstellung",nil,1,nil);
// nun für jede variable Regel die es gibt, einen Menüeintrag schreiben, denn diese müssen individuell sein...
// Bausatz und Baustoffe bei Abriss
if(Equal(dummy, PPT7))
  {
  AddMenuItem(Format("Chance für Bausatz zurück erhöhen (%v/100%)",HashGet(hashSpielRegeln, "Bausatz")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"Bausatz",1,100),CNKT, pClonk, nil, nil,"Beim Abriss eines Gebäudes gibts mit x% Chance einen Bausatz wieder");
  AddMenuItem(Format("Chance für Bausatz zurück vermindern (%v/100%)",HashGet(hashSpielRegeln, "Bausatz")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"Bausatz",-1,100),CNKT, pClonk, nil, nil,"Beim Abriss eines Gebäudes gibts mit x% Chance einen Bausatz wieder");
  AddMenuItem(Format("Prozentsatz der Baustoffe erhöhen (%v/100%)",HashGet(hashSpielRegeln, "Baustoffe")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,2,"Baustoffe",10,100),CNMT, pClonk, nil, nil,"Beim Abriss eines Gebäudes gibt es x% der eingesetzten Baustoffe wieder. Jede einzelne Komponente wird mit Chance x% erstellt. Nur wenn Baumaterialregel an.");
  AddMenuItem(Format("Prozentsatz der Baustoffe vermindern (%v/100%)",HashGet(hashSpielRegeln, "Baustoffe")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,3,"Baustoffe",-10,100),CNMT, pClonk, nil, nil,"Beim Abriss eines Gebäudes gibt es x% der eingesetzten Baustoffe wieder. Jede einzelne Komponente wird mit Chance x% erstellt. Nur wenn Baumaterialregel an.");
  AddMenuItem(Format("Prozentsatz erstatteter Kosten erhöhen (%v/100%)",HashGet(hashSpielRegeln, "Baustellenkostenerstattung")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,4,"Baustellenkostenerstattung",10,100),BBGG, pClonk, nil, nil,"Beim Abriss eines Gebäudes gibt es x% der eingesetzten Clunker wieder. Nur wenn Baustellenkosten aktiv ist.");
  AddMenuItem(Format("Prozentsatz erstatteter Kosten vermindern (%v/100%)",HashGet(hashSpielRegeln, "Baustellenkostenerstattung")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,5,"Baustellenkostenerstattung",-10,100),BBGG, pClonk, nil, nil,"Beim Abriss eines Gebäudes gibt es x% der eingesetzten Clunker wieder. Nur wenn Baustellenkosten aktiv ist.");
  }

// Ausbildungsregeln
if(Equal(dummy, PP2P))
  {
  AddMenuItem("Info: Angepasste Physics muss aktiviert sein!", "EditVariabelRule",PP2P, pClonk, nil, nil,"Clonks lassen sich dann ausbilden und verbessern, was Zeit, Geld und Erfahrung benötigt. In den statischen Regeln kann noch mit 0, 0.5 und 1 multipliziert werden.");
  AddMenuItem(Format("Standardwert für nötige Erfahrung erhöhen (%v/1000)",HashGet(hashSpielRegeln, "Ausbildungserfahrung")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Ausbildungserfahrung",10,1000,1),PP2P, pClonk, nil, nil,"Wert der nötigen Erfahrung für von Stufe 5 auf Stufe 6. Die anderen Werte werden in 5 Stufenschritten anhand dessen berechnet.");
  AddMenuItem(Format("Standardwert für nötige Erfahrung vermindern (%v/1000)",HashGet(hashSpielRegeln, "Ausbildungserfahrung")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,2,"Ausbildungserfahrung",-10,1000,1),PP2P, pClonk, nil, nil,"Wert der nötigen Erfahrung für von Stufe 5 auf Stufe 6. Die anderen Werte werden in 5 Stufenschritten anhand dessen berechnet.");
  AddMenuItem(Format("Standardwert für Kosten erhöhen (%v/1000)",HashGet(hashSpielRegeln, "Ausbildungskosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,3,"Ausbildungskosten",10,1000,1),PP2P, pClonk, nil, nil,"Wert der Geldkosten für von Stufe 5 auf Stufe 6. Die anderen Werte werden in 5 Stufenschritten anhand dessen berechnet.");
  AddMenuItem(Format("Standardwert für Kosten vermindern (%v/1000)",HashGet(hashSpielRegeln, "Ausbildungskosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,4,"Ausbildungskosten",-10,1000,1),PP2P, pClonk, nil, nil,"Wert der Geldkosten für von Stufe 5 auf Stufe 6. Die anderen Werte werden in 5 Stufenschritten anhand dessen berechnet.");
  AddMenuItem(Format("Multiplikationsfaktor der Ausbildungszeit erhöhen (%v/10)",HashGet(hashSpielRegeln, "Ausbildungszeit")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,5,"Ausbildungszeit",5,10),PP2P, pClonk, nil, nil,"Der Wert wird noch durch 10 geteilt! D.h. es sind zurzeit nur 3 Möglichkeiten vorhanden: instant, halbe Zeit, volle Zeit. Wobei die Standardzeit voreingestellt ist");
  AddMenuItem(Format("Multiplikationsfaktor der Ausbildungszeit vermindern (%v/10)",HashGet(hashSpielRegeln, "Ausbildungszeit")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,6,"Ausbildungszeit",-5,10),PP2P, pClonk, nil, nil,"Der Wert wird noch durch 10 geteilt! D.h. es sind zurzeit nur 3 Möglichkeiten vorhanden: instant, halbe Zeit, volle Zeit. Wobei die Standardzeit voreingestellt ist");
 }
// Forschung
if(Equal(dummy, FFF3))
  {
  AddMenuItem(Format("Multiplikationsfaktor der Forschungskosten erhöhen (%v/200)",HashGet(hashSpielRegeln, "Forschungskosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Forschungskosten",1,200,nil),FFF3, pClonk, nil, nil,"Der Wert wird noch durch 10 geteilt! Die individuellen Forschungskosten je Objekt, werden mit diesem Faktor multipliziert");
  AddMenuItem(Format("Multiplikationsfaktor der Forschungskosten vermindern (%v/200)",HashGet(hashSpielRegeln, "Forschungskosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Forschungskosten",-1,200,nil),FFF3, pClonk, nil, nil,"Der Wert wird noch durch 10 geteilt! Die individuellen Forschungskosten je Objekt, werden mit diesem Faktor multipliziert");
  AddMenuItem(Format("Multiplikationsfaktor der Forschungszeit erhöhen (%v/200)",HashGet(hashSpielRegeln, "Forschungszeit")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,2,"Forschungszeit",1,200,nil),FFF3, pClonk, nil, nil,"Der Wert wird noch durch 10 geteilt! Die individuelle Forschungszeit je Objekt, werden mit diesem Faktor multipliziert");
  AddMenuItem(Format("Multiplikationsfaktor der Forschungszeit vermindern (%v/200)",HashGet(hashSpielRegeln, "Forschungszeit")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,3,"Forschungszeit",-1,200,nil),FFF3, pClonk, nil, nil,"Der Wert wird noch durch 10 geteilt! Die individuelle Forschungszeit je Objekt, werden mit diesem Faktor multipliziert");
  }
// StartZeitalter 
if(Equal(dummy, PPT8))
  {
  AddMenuItem(Format("Start-Wissensstand erhöhen (%v/22)",HashGet(hashSpielRegeln, "StartZeitalter")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"StartZeitalter",1,22),PPT8, pClonk, nil, nil,"Je höher dieser Werte, desto mehr Baupläne und fertige Forschungen stehen allen Spielern zur Verfügung. Deaktviert, wenn Regel Szenario-Forschung aktiv!!");
  AddMenuItem(Format("Start-Wissensstand vermindern (%v/22)",HashGet(hashSpielRegeln, "StartZeitalter")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"StartZeitalter",-1,22),PPT8, pClonk, nil, nil,"Je höher dieser Werte, desto mehr Baupläne und fertige Forschungen stehen allen Spielern zur Verfügung. Deaktviert, wenn Regel Szenario-Forschung aktiv!!");
  }
// Produktionsgeschwindigkeit 
if(Equal(dummy, PRTI))
  {
  AddMenuItem(Format("Produktionsgeschwindigkeit erhöhen (%v/500%)",HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"Produktionsgeschwindigkeit",5,500),dummy, pClonk, nil, nil,"x % der Standard Produktionszeit in Chemiefabrik, Amboss, Werkstatt und Co.");
  AddMenuItem(Format("Produktionsgeschwindigkeit vermindern (%v/500%)",HashGet(hashSpielRegeln, "Produktionsgeschwindigkeit")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"Produktionsgeschwindigkeit",-5,500),dummy, pClonk, nil, nil,"x % der Standard Produktionszeit in Chemiefabrik, Amboss, Werkstatt und Co.");
  } 
// Baustellenboost 
if(Equal(dummy, SBUI))
  {
  AddMenuItem(Format("Baustellenboost erhöhen (%v/100%)",HashGet(hashSpielRegeln, "Baustellenboost")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"Baustellenboost",1,100),dummy, pClonk, nil, nil,"Jede Baustelle bekommt einen Fertigstellungs-Boost von x%. Dies betrifft also sowohl Bauzeit, als auch Baustoffe.");
  AddMenuItem(Format("Baustellenboost vermindern (%v/100%)",HashGet(hashSpielRegeln, "Baustellenboost")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"Baustellenboost",-1,100),dummy, pClonk, nil, nil,"Jede Baustelle bekommt einen Fertigstellungs-Boost von x%. Dies betrifft also sowohl Bauzeit, als auch Baustoffe.");
  } 
if(Equal(dummy, GGGG)) // ereignisse beim Materialabbau
  {
      for(var inumber=nil; (inumber ?? 0)<(GetLength(HashGet(hashSpielRegeln,"AbbauEreignisse")[0]) ?? 0);S42MigrationStep(inumber, 1, true))
      {
          AddMenuItem(Format("% von noetigem Material %s erhoehen (%v/100)",HashGet(hashSpielRegeln, "AbbauEreignisse")[0][(inumber ?? 0)],HashGet(hashSpielRegeln, "AbbauEreignisse")[2][(inumber ?? 0)]), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,inumber,"AbbauEreignisse",1,100,nil),HashGet(hashSpielRegeln, "AbbauEreignisse")[1][(inumber ?? 0)], pClonk, nil, nil,"Wenn 0, dann deaktiviert. Ansonsten wird alle x% von aus der Karte entferntem Material ein Ereignis ausgelöst. Z.b ein Angriff, oder Naturkatastrophen.");
          AddMenuItem(Format("% von noetigem Material %s vermindern (%v/100)",HashGet(hashSpielRegeln, "AbbauEreignisse")[0][(inumber ?? 0)],HashGet(hashSpielRegeln, "AbbauEreignisse")[2][(inumber ?? 0)]), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,inumber,"AbbauEreignisse",-1,100,nil),HashGet(hashSpielRegeln, "AbbauEreignisse")[1][(inumber ?? 0)], pClonk, nil, nil,"Wenn 0, dann deaktiviert. Ansonsten wird alle x% von aus der Karte entferntem Material ein Ereignis ausgelöst. Z.b ein Angriff, oder Naturkatastrophen.");
      }
      AddMenuItem(Format("Ereignis Katastrophen und/oder Monster? (%v/2)",HashGet(hashSpielRegeln, "AbbauEreignisse")[3]), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,inumber,"AbbauEreignisse",1,2,nil),MONS, pClonk, nil, nil,"Wenn 0, dann Katastrophen und Monster gleich häufig. Wenn 1, dann nur Katastrophen, wenn 2 dann nur Monster.");
  }
// Produktionskosten, glaube nur aktiv, wenn baumaterial fuer prodktion deaktiviert.
if(Equal(dummy, PPGG))
  {
  AddMenuItem(Format("Produktionskosten erhöhen (%v/1000%)",HashGet(hashSpielRegeln, "Produktionskosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"Produktionskosten",5,1000),dummy, pClonk, nil, nil,"x % der Standard Produktionskosten in Chemiefabrik, Amboss, Werkstatt und Co. Der Standardwert sind die halben Kaufkosten.");
  AddMenuItem(Format("Produktionskosten vermindern (%v/1000%)",HashGet(hashSpielRegeln, "Produktionskosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"Produktionskosten",-5,1000),dummy, pClonk, nil, nil,"x % der Standard Produktionskosten in Chemiefabrik, Amboss, Werkstatt und Co. Der Standardwert sind die halben Kaufkosten.");
  } 
// Baustellenkosten 
if(Equal(dummy, BBGG))
  {
  AddMenuItem(Format("Baustellenkosten erhöhen (%v/1000%)",HashGet(hashSpielRegeln, "Baustellenkosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"Baustellenkosten",5,1000),dummy, pClonk, nil, nil,"x % der Standard Baustellenkosten. Der Standardwert ist Gewicht/10.");
  AddMenuItem(Format("Baustellenkosten vermindern (%v/1000%)",HashGet(hashSpielRegeln, "Baustellenkosten")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"Baustellenkosten",-5,1000),dummy, pClonk, nil, nil,"x % der Standard Baustellenkosten. Der Standardwert ist Gewicht/10.");
  } 
  // EnergieverbrauchNeue Gebäude.
if(Equal(dummy, ENVE))
  {
  AddMenuItem(Format("Energieverbrauch für Fabrik Zukunftsgebäude erhöhen (%v/500%)",HashGet(hashSpielRegeln, "EnergieverbrauchNeue")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,nil,"EnergieverbrauchNeue",5,500),dummy, pClonk, nil, nil,"x % des Standard-Energieverbrauchs.");
  AddMenuItem(Format("Energieverbrauch für Fabrik Zukunftsgebäude vermindern (%v/500%)",HashGet(hashSpielRegeln, "EnergieverbrauchNeue")), Format("ChangeVariableRuleConfig(%i,%d,\"%s\",%d,%d)",dummy,1,"EnergieverbrauchNeue",-5,500),dummy, pClonk, nil, nil,"x % des Standard-Energieverbrauchs.");
  }
  // Fertig
    AddMenuItem("$Finished$", "VariabelRuleMenu", CNCL, pClonk);
  SelectMenuItem(iSelection, pClonk);
}

func ChangeVariableRuleConfig(id dummy, iSelection, iname, ichange, imaximum, iminimum)
{
  if(Equal(iname, "AbbauEreignisse")) // spezialbehandlung, weil es aus mehreren Arrays besteht und nicht bloß ein wert ist.
  {
          var aAbbau = [[],[],[]]; // HashGet(hashSpielRegeln, "AbbauEreignisse")
          for(var izahl;(izahl ?? 0)<3;S42MigrationStep(izahl, 1, true))
              for(var inumber=nil; (inumber ?? 0)<(GetLength(HashGet(hashSpielRegeln,"AbbauEreignisse")[(izahl ?? 0)]) ?? 0);S42MigrationStep(inumber, 1, true))
              {
                  if(And(Equal(inumber, iSelection), Equal(izahl, 2))) ArrayAdd(aAbbau[(izahl ?? 0)],BoundBy((HashGet(hashSpielRegeln, "AbbauEreignisse")[(izahl ?? 0)][(inumber ?? 0)] ?? 0)+(ichange ?? 0),iminimum,imaximum)); // wenn es das gewählte, Wert verändern
                  else ArrayAdd(aAbbau[(izahl ?? 0)],HashGet(hashSpielRegeln, "AbbauEreignisse")[(izahl ?? 0)][(inumber ?? 0)]);   // alle anderen beibehalten und so übernehmen
              }
          
          if(!Equal(HashGet(hashSpielRegeln, "AbbauEreignisse")[3], 2)) // den Monster oder Katastrophe eintrag noch zufügen
             ArrayAdd(aAbbau,S42MigrationAssign(HashGet(hashSpielRegeln, "AbbauEreignisse")[3], "+=", 1));
          else
             ArrayAdd(aAbbau,nil);
          HashPut(hashSpielRegeln, iname, aAbbau);
          // Geräusch
          Sound("Grab", 1,nil,nil,1);
          // Environmentmenü wieder aufmachen
          iSelection = (iSelection ?? 0)*2;
          if(Equal(ichange, -1)) iSelection = (iSelection ?? 0)+1;
          return(EditVariabelRule(dummy,iSelection));
  }
  
  //Log("dummy=%v,iSelection=%v,iname=%v,ichange=%v",dummy,iSelection,iname,ichange);
  // iname ist der Key
  HashPut(hashSpielRegeln, iname, BoundBy((HashGet(hashSpielRegeln, iname) ?? 0)+(ichange ?? 0),iminimum,imaximum));
  
// Ausnahmen behandeln, wie z.b ausbildungszeit, welche noch über anzahl der regelobjekte geregelt wird
  if(Equal(iname, "Ausbildungszeit"))
   {// hier noch Regeln erstellen, die im Zuge der variablen Einstellungen erzeugt werden müssen

var zeitregel;
 // Log(Format("objekte vorher: %v . Wert: %v",ObjectCount2(Find_ID(PP2P)),HashGet(hashSpielRegeln, "Ausbildungszeit")));
if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), nil))
 {
while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel);  // !!! funktioniert so nicht, weil regeln die check nicht true haben, werden vom regelchooser wieder entfernt
CreateObject(PP2P,1,1,-1); // -> instant
 }
if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), 5))
 {
while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel);
CreateObject(PP2P,1,1,-1); 
CreateObject(PP2P,1,1,-1); // -> halbe Zeit
 }
if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), 10))
 {
while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel); // -> standard Zeit
 }
  // Log(Format("objekte nachher: %v . Wert: %v",ObjectCount2(Find_ID(PP2P)),HashGet(hashSpielRegeln, "Ausbildungszeit")));
   }

  // Geräusch
  Sound("Grab", 1,nil,nil,1);

// Log(Format("ID: %v, Spieler: %v, Playercount: %v",id,GetPlayerByIndex(i),GetPlayerCount()));
// die gewählten startforschungen einstellen

if (And(!Equal(wissenvorher, HashGet(hashSpielRegeln, "StartZeitalter")), (!FindObject(SZRE)))) // nur wenn StartZeitalter geändert wurde, dann das gnaze wissen löschen und neu setzen. und wenn mein forschungsbaum verwendet werden soll, und nicht der des szenarios
{
    var num, id;
    num = nil;
    for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
  num = nil;
  while (id = GetDefinition(S42MigrationStep(num, 1, true)))
    {
    if (GetPlrKnowledge(GetPlayerByIndex(i), id)) 
      {
      SetPlrKnowledge(GetPlayerByIndex(i), id,1);
      }
    } // das gesamte wissen entfernen und danach neu verteilen
  }
  GlobalStartingEra(HashGet(hashSpielRegeln, "StartZeitalter")); // die Funktionen für das StartZeitalter aufrufen, welche sich in den forschungsbaum skripten befinden
}
  // Regelmenü wieder aufmachen
EditVariabelRule(dummy,iSelection);
}


func OpenEnvironmentMenu(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk);
  // Regeln einfngen
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat_Environment) ; S42MigrationStep(i, 1, true))
    if(And(!DefinitionCall(idR, "IsNotChooseable"), !DefinitionCall(idR, "IsVariabelChooseable2")))
      {
      def = TransformObject(idR,aEnvironment[(i ?? 0)]);
      AddMenuItem("%s", "ChangeEnvironmentConf", idR, pClonk, nil, i, nil, 4, def);
      if(Equal(i, iSelection))
        check = true;
      if(!check)
        S42MigrationStep(j, 1, true);
      RemoveObject(def);
      }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func EditVariabelEnvironment(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menü aufmachen
  CreateMenu(GetID(), pClonk,nil,nil,"Variable Einstellung",nil,1,nil);
// nun für jede variable Environment die es gibt, einen Menüeintrag schreiben, denn diese müssen individuell sein...
// Sichtradius, direkt im clnk skript geregelt
if(Equal(dummy, TS9Z))
  {
  AddMenuItem(Format("Sichtradius bei Tag an der Oberfläche erhöhen (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusTagHimmel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"SichtradiusTagHimmel",10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk tagsüber an der Oberfläche hat");
  AddMenuItem(Format("Sichtradius bei Tag an der Oberfläche vermindern (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusTagHimmel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"SichtradiusTagHimmel",-10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk tagsüber an der Oberfläche hat");
  AddMenuItem(Format("Sichtradius bei Tag unter der Erde erhöhen (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusTagTunnel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,2,"SichtradiusTagTunnel",10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk tagsüber unter der Erde hat");
  AddMenuItem(Format("Sichtradius bei Tag unter der Erde vermindern (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusTagTunnel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,3,"SichtradiusTagTunnel",-10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk tagsüber unter der Erde hat");
  AddMenuItem(Format("Sichtradius bei Nacht an der Oberfläche erhöhen (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusNachtHimmel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,4,"SichtradiusNachtHimmel",10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk nachts an der Oberfläche hat");
  AddMenuItem(Format("Sichtradius bei Nacht an der Oberfläche vermindern (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusNachtHimmel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,5,"SichtradiusNachtHimmel",-10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk nachts an der Oberfläche hat");
  AddMenuItem(Format("Sichtradius bei Nacht unter der Erde erhöhen (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusNachtTunnel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,6,"SichtradiusNachtTunnel",10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk nachts unter der Erde hat");
  AddMenuItem(Format("Sichtradius bei Nacht unter der Erde vermindern (%v/1000)",HashGet(hashSpielRegeln, "SichtradiusNachtTunnel")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,7,"SichtradiusNachtTunnel",-10,1000,20),dummy, pClonk, nil, nil,"Der Sichtradius, den jeder Clonk nachts unter der Erde hat");
  AddMenuItem(Format("Verhalten des Kriegsnebels ändern + (siehe Beschreibgung) (%v/3)",HashGet(hashSpielRegeln, "SichtradiusVerhalten")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,8,"SichtradiusVerhalten",1,3,nil),dummy, pClonk, nil, nil,"Der Sichtradius ändert sich unter folgenden Bedingungen: 0:Garkein Kriegsnebel.__ 1:Tag/Nacht/Himmel/Tunnel.__2:nur Tag/Nacht(Wert von Oberfläche).__3:Immer derselbe Sichtradius(der von Tag+Oberfläche).");
  AddMenuItem(Format("Verhalten des Kriegsnebels ändern - (siehe Beschreibgung) (%v/3)",HashGet(hashSpielRegeln, "SichtradiusVerhalten")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,9,"SichtradiusVerhalten",-1,3,nil),dummy, pClonk, nil, nil,"Der Sichtradius ändert sich unter folgenden Bedingungen: 0:Garkein Kriegsnebel.__ 1:Tag/Nacht/Himmel/Tunnel.__2:nur Tag/Nacht(Wert von Oberfläche).__3:Immer derselbe Sichtradius(der von Tag+Oberfläche).");
  }
// Tagesdauer
if(Equal(dummy, TIME))
  {
  AddMenuItem(Format("Tagegeschwindigkeit erhöhen %v/20",dayspeed),"ChangeDaySpeed",dummy,pClonk,nil,1,"Multiplikationsfakter für die Geschwindigket des Tag/Nachtwechsels");
  AddMenuItem(Format("Tagegeschwindigkeit verringern %v/20",dayspeed),"ChangeDaySpeed",dummy,pClonk,nil,-1,"Multiplikationsfakter für die Geschwindigket des Tag/Nachtwechsels");
  }
// Verdunstung
if(Equal(dummy, ECLD))
  {
  AddMenuItem("Info: Verdunstet auch große Seen/Meere!", "EditVariabelRule",PP2P, pClonk, nil, nil,"Um überflutetes Land zb durch Regen zu verhindern, besser Grundwasser verwenden!");
  AddMenuItem(Format("Verdunstung tagsüber zwischen 10 und 30°C erhöhen (%v/20)",HashGet(hashSpielRegeln, "Verdunstennormal")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Verdunstennormal",1,20,nil),dummy, pClonk, nil, nil,"Wenn z.b 6 eingestellt ist, heißt dies mit einer Chance von 1/6 wird alle 3Frames(fest) ein Partikel wasser verdunsten. Wenn 0, dann keine Verdunstung.");
  AddMenuItem(Format("Verdunstung tagsüber zwischen 10 und 30°C vermindern (%v/20)",HashGet(hashSpielRegeln, "Verdunstennormal")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Verdunstennormal",-1,20,nil),dummy, pClonk, nil, nil,"Wenn z.b 6 eingestellt ist, heißt dies mit einer Chance von 1/6 wird alle 3Frames(fest) ein Partikel wasser verdunsten. Wenn 0, dann keine Verdunstung.");
  AddMenuItem(Format("Verdunstung tagsüber zwischen 30 und 40°C erhöhen (%v/20)",HashGet(hashSpielRegeln, "Verdunstenwarm")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,2,"Verdunstenwarm",1,20,nil),dummy, pClonk, nil, nil,"Wenn z.b 3 eingestellt ist, heißt dies mit einer Chance von 1/3 wird alle 3Frames(fest) ein Partikel wasser verdunsten. Wenn 0, dann keine Verdunstung.");
  AddMenuItem(Format("Verdunstung tagsüber zwischen 30 und 40°C (%v/20)",HashGet(hashSpielRegeln, "Verdunstenwarm")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,3,"Verdunstenwarm",-1,20,nil),dummy, pClonk, nil, nil,"Wenn z.b 3 eingestellt ist, heißt dies mit einer Chance von 1/3 wird alle 3Frames(fest) ein Partikel wasser verdunsten. Wenn 0, dann keine Verdunstung.");
  AddMenuItem(Format("Verdunstung tagsüber ab 40°C erhöhen (%v/20)",HashGet(hashSpielRegeln, "Verdunstenheiß")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,4,"Verdunstenheiß",1,20,nil),dummy, pClonk, nil, nil,"Wenn z.b 1 eingestellt ist, heißt dies mit einer Chance von 1/1 wird alle 3Frames(fest) ein Partikel wasser verdunsten. Wenn 0, dann keine Verdunstung.");
  AddMenuItem(Format("Verdunstung tagsüber ab 40°C vermindern (%v/20)",HashGet(hashSpielRegeln, "Verdunstenheiß")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,5,"Verdunstenheiß",-1,20,nil),dummy, pClonk, nil, nil,"Wenn z.b 1 eingestellt ist, heißt dies mit einer Chance von 1/1 wird alle 3Frames(fest) ein Partikel wasser verdunsten. Wenn 0, dann keine Verdunstung.");
  }
// Umweltbelastbarkeit
if(Equal(dummy, SN_A))
  {
  AddMenuItem(Format("Umweltbelastbarkeit pro Spieler erhöhen (%v/100000)",HashGet(hashSpielRegeln, "Umweltbelastbarkeit")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Umweltbelastbarkeit",100,100000,300),dummy, pClonk, nil, nil,"Dies regelt, wie belastbar die Umwelt ist und ab wieviel Abgasen/Müll usw. die höchste Verschmutzungsstufe erreicht ist und Katastrophen usw. kommen (es gibt 5 Verschmutzungsstufen).");
  AddMenuItem(Format("Umweltbelastbarkeit pro Spieler vermindern (%v/100000)",HashGet(hashSpielRegeln, "Umweltbelastbarkeit")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Umweltbelastbarkeit",-100,100000,300),dummy, pClonk, nil, nil,"Dies regelt, wie belastbar die Umwelt ist und ab wieviel Abgasen/Müll usw. die höchste Verschmutzungsstufe erreicht ist und Katastrophen usw. kommen (es gibt 5 Verschmutzungsstufen).");
  }
if(Equal(dummy, R_EG))
  {
  AddMenuItem(Format("UNFERTIG Wahrscheinlichkeit für Regenbeginn erhöhen (%v/10)",HashGet(hashSpielRegeln, "Regen")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Regen",1,10,1),dummy, pClonk, nil, nil,"Über 0°C die Chance für Niederschlag");
  AddMenuItem(Format("Regen",HashGet(hashSpielRegeln, "Regen")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Regen",-1,10,1),dummy, pClonk, nil, nil,"Über 0°C die Chance für Niederschlag");
  AddMenuItem(Format("Regen",HashGet(hashSpielRegeln, "Regen")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,2,"Regen",1,10,1),dummy, pClonk, nil, nil,"Unter 0°C die Chance für Schnee");
  AddMenuItem(Format("Regen",HashGet(hashSpielRegeln, "Regen")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,3,"Regen",-1,10,1),dummy, pClonk, nil, nil,"Unter 0°C die Chance für Schnee");
  }
if(Equal(dummy, GRLV))
{
  AddMenuItem(Format("Grundwasser Versickerungsgeschwindigkeit erhöhen (%v/100)",HashGet(hashSpielRegeln, "Grundwasser")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Grundwasser",5,100,nil),dummy, pClonk, nil, nil,"Regelt wie stark Wasser in der Erde versickern soll. Verhindert Überschwemmungen (zumindest an der Oberflaeche) und gilt nicht fuer von Beginn an platzierte Gewässer.");
  AddMenuItem(Format("Grundwasser Versickerungsgeschwindigkeit vermindern (%v/100)",HashGet(hashSpielRegeln, "Grundwasser")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Grundwasser",-5,100,nil),dummy, pClonk, nil, nil,"Regelt wie stark Wasser in der Erde versickern soll. Verhindert Überschwemmungen (zumindest an der Oberflaeche) und gilt nicht fuer von Beginn an platzierte Gewässer.");
}
if(Equal(dummy, DAR2)) // Dunkelheitsgrad, hazard düsternis environment Symbol
{
  AddMenuItem(Format("Dunkelheitsgrad erhöhen (%v/100)",HashGet(hashSpielRegeln, "Dunkelheitsgrad")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Dunkelheitsgrad",50,100,50),dummy, pClonk, nil, nil,"Regelt wie dunkel es nachts werden soll. Bei 100% werden meist Lichtobjekte benötigt, da man sonst nichts sieht. Bei 50% sollte es zur Not auch ohne Lichtobjekte gehen, wodurch es zwar dunkler wird, aber man keine Lichter braucht und somit evlt Laggs verhindert (In TIME ändert sich nichts, aber das Objekt DUUU wird nicht erstellt). Wenn es garnicht dunkel werden soll, dann die statische Regel konstante Helligkeit aktivieren.");
  AddMenuItem(Format("Dunkelheitsgrad vermindern (%v/100)",HashGet(hashSpielRegeln, "Dunkelheitsgrad")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Dunkelheitsgrad",-50,100,50),dummy, pClonk, nil, nil,"Regelt wie dunkel es nachts werden soll. Bei 100% werden meist Lichtobjekte benötigt, da man sonst nichts sieht. Bei 50% sollte es zur Not auch ohne Lichtobjekte gehen, wodurch es zwar dunkler wird, aber man keine Lichter braucht und somit evlt Laggs verhindert (In TIME ändert sich nichts, aber das Objekt DUUU wird nicht erstellt). Wenn es garnicht dunkel werden soll, dann die statische Regel konstante Helligkeit aktivieren.");
}
if(Equal(dummy, BLIM)) // Wird in dem Wolkenskript eingesetzt und modifiziert die gesamtwahrscheinlichkeit, dass eine Wolke einen Blitz loslässt. 100==100% ist eine Multiplikation der Wahrscheinlichkeit mit 1.
{
  AddMenuItem(Format("BlitzeModifier erhöhen (%v%/5000%)",HashGet(hashSpielRegeln, "BlitzeModifier")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"BlitzeModifier",2,5000,nil),dummy, pClonk, nil, nil,"Diese Prozentzahl wird mit der Gesamtwahrscheilichkeit, dass eine Wolke einen Blitz loslässt multipliziert. Bringt nichts, wenn SzenarioBlitze auf 0 sind! (wobei diese sich aus den im szenario eingestelltem wert und einem im Wolken skript definiertem standardwert ergibt. Wenn Endprodukt 10 ist, dann alle 200 sekunden ein Blitz pro Wolke)");
  AddMenuItem(Format("BlitzeModifier vermindern (%v%/5000%)",HashGet(hashSpielRegeln, "BlitzeModifier")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"BlitzeModifier",-2,5000,nil),dummy, pClonk, nil, nil,"Diese Prozentzahl wird mit der Gesamtwahrscheilichkeit, dass eine Wolke einen Blitz loslässt multipliziert. Bringt nichts, wenn SzenarioBlitze auf 0 sind! (wobei diese sich aus den im szenario eingestelltem wert und einem im Wolken skript definiertem standardwert ergibt. Wenn Endprodukt 10 ist, dann alle 200 sekunden ein Blitz pro Wolke)");
}
if(Equal(dummy, _BAR)) // Barriere
{
  AddMenuItem(Format("Barriere Dauer erhöhen (%v/3600)",HashGet(hashSpielRegeln, "Barriere")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,nil,"Barriere",2,3600,nil),dummy, pClonk, nil, nil,"Dauer der Barriere in Sekunden, welche wenn größer 0 in der Mitte der Karte platziert wird und niemanden hindurch läasst.");
  AddMenuItem(Format("Barriere Dauer vermindern (%v/3600)",HashGet(hashSpielRegeln, "Barriere")), Format("ChangeVariableEnvironmentConfig(%i,%d,\"%s\",%d,%d,%d)",dummy,1,"Barriere",-2,3600,nil),dummy, pClonk, nil, nil,"Dauer der Barriere in Sekunden, welche wenn größer 0 in der Mitte der Karte platziert wird und niemanden hindurch läasst.");
}


  // Fertig 
    AddMenuItem("$Finished$", "VariabelEnvironmentMenu", CNCL, pClonk);
  SelectMenuItem(iSelection, pClonk);
}


func ChangeDaySpeed(idummy,izahl)
{
dayspeed = BoundBy((dayspeed ?? 0)+(izahl ?? 0),1,20);
if(Equal(izahl, 1)) izahl=nil;
if(Equal(izahl, -1)) izahl=1;
EditVariabelEnvironment(idummy, izahl);
}

func ChangeVariableEnvironmentConfig(id dummy, iSelection, iname, ichange, imaximum, iminimum)
{
   //Log("dummy=%v,iSelection=%v,iname=%v,ichange=%v",dummy,iSelection,iname,ichange);
  // iname ist der Key
  HashPut(hashSpielRegeln, iname, BoundBy((HashGet(hashSpielRegeln, iname) ?? 0)+(ichange ?? 0),iminimum,imaximum));
  // Geräusch
  Sound("Grab", 1,nil,nil,1);
  // Environmentmenü wieder aufmachen
EditVariabelEnvironment(dummy,iSelection);
}

func ChangeEnvironmentConf(id idEnvironment, int i)
{
  // Regel wurde ausgewShlt
  i ??= 0;
  if(!aEnvironment[(i ?? 0)])
  {
    aEnvironment[(i ?? 0)] = true;
    if(Or(idEnvironment->~CreateOnChoose(), Death))
      CreateObject(idEnvironment,nil,nil,-1);
  }
  // Regel wurde abgewShlt
  else
  {
    aEnvironment[(i ?? 0)] = nil;
    if(Or(idEnvironment->~CreateOnChoose(), Death))
      if(FindObject(idEnvironment))
        RemoveObject(FindObject(idEnvironment));
  }
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  // Regelmenn wieder aufmachen
  OpenEnvironmentMenu(nil, i);
}

func VariabelEnvironmentMenu(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk,nil,nil,"Variable Umweltobjekte",nil,1,nil);
  // Umweltobjekte einfngen
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat_Environment) ; S42MigrationStep(i, 1, true))
    if(DefinitionCall(idR, "IsVariabelChooseable2")) // !DefinitionCall(idR, "IsNotChooseable") && 
      {
      AddMenuItem("%s", "EditVariabelEnvironment", idR, pClonk);
      }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
}

/* Spielzielmenn */

func OpenGoalChooseMenu(iSelection)
{
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  if(!pClonk) return(ScheduleCall(this(), "OpenMenu", 1));
  if(!GetLength(aGoals)) return(ScheduleCall(this(), "OpenMenu", 1));

  CloseMenu(pClonk);

  if(Equal(GetLength(aGoals), 1))
    return(CreateGoal(aGoals[0], aTempGoalSave[0]));

  CreateMenu(GetID(), pClonk);

  var i,j,check,def;
  for(var goal in aGoals)
  {
    def = TransformObject(goal,FindObject2(Find_ID(goal),Find_Exclude(def)));
    AddMenuItem("%s", "CreateGoal", goal, pClonk, nil, i,nil,4,def);
    if(Equal(i, iSelection))
      check = true;
    if(!check)
      S42MigrationStep(j, 1, true);
    RemoveObject(def);
    S42MigrationStep(i, 1, true);
  }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
  
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func CreateGoal(id idGoal,iSelection)
{
  var goal;
  if(goal=FindObject2(Find_ID(idGoal)))
    RemoveObject(goal);
  else
  {
    goal=CreateObject(idGoal, nil,nil, -1);
    for(var i = nil; (i ?? 0) < (GetPlayerCount(C4PT_User) ?? 0); S42MigrationStep(i, 1, true))
      ProtectedCall(goal,"InitializePlayer",GetPlayerByIndex(i,C4PT_User),nil,nil,FindBase(GetPlayerByIndex(i,C4PT_User)),GetPlayerTeam(GetPlayerByIndex(i,C4PT_User)));
  }
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  //Menn öffnen
  OpenGoalChooseMenu(iSelection);
}

func GoalIsCompatible()
{
  // Schon eines gefunden?
  if(pGoal) return(1);
  // Gut
  pGoal = FindObject2(Find_Category(C4D_Goal));
  return(1);
}

/* Schwierigkeitsmenn */

func OpenDifficultyMenu(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk);
  // Regeln einfngen
  for(var i=1, idR, def, j, check ; idR = GetDifficultyInt(i) ; S42MigrationStep(i, 1, true))
    if(!DefinitionCall(idR, "IsNotChooseable"))
      {
      def = TransformObject(idR,Equal(idR, idDifficulty));
      AddMenuItem("%s", "ChangeDifficulty", idR, pClonk, nil, i, nil, 4, def);
      if(Equal(i, iSelection))
        check = true;
      if(!check)
        S42MigrationStep(j, 1, true);
      RemoveObject(def);
      }
  if(!CampaignMode())
    AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
  else
  {
    S42MigrationStep(i, 1, true);
    if(!check)
      S42MigrationStep(j, 1, true);
    def = TransformObject(SPDG,FindObject(SPDG));
    AddMenuItem("%s", "ChangeRuleEx", SPDG, pClonk, nil, i, nil, 4, def);
    RemoveObject(def);
    S42MigrationStep(i, 1, true);
    if(!check)
      S42MigrationStep(j, 1, true);
    def = TransformObject(ATLG,FindObject(ATLG));
    AddMenuItem("%s", "ChangeRuleEx", ATLG, pClonk, nil, i, nil, 4, def);
    RemoveObject(def);
    AddMenuItem("$Finished$", "ConfigurationFinished", CNCL, pClonk);
  }
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func ChangeRuleEx(idR,i)
{
  var obj;
  if(obj=FindObject2(Find_ID(idR)))
    RemoveObject(obj);
  else
    CreateObject(idR,nil,nil,-1)->~IsChoosen();
  OpenDifficultyMenu(nil,i);
  return true;
}

func ChangeDifficulty(id Difficulty, int i)
{
  i ??= 0;
  idDifficulty=Difficulty;
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  // Regelmenn wieder aufmachen
  OpenDifficultyMenu(nil, i);
}

func GetDifficultyInt(iInt)
{
  var temp=CreateArray();
  for(var i=nil, idR, diff; idR = GetDefinition(i, Chooser_Cat_Rule) ; S42MigrationStep(i, 1, true))
   if(Equal(iInt, DefinitionCall(idR,"IsDifficultyLevel")))
    return idR;
  return nil;
}

/* Zeitalter-Menn */
func OpenTechLevelMenu(id dummy, int iSelection)
{
  iSelection ??= 0;
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(GetID(), pClonk);
  // WShlbare Technologiestufen einfngen
  for(var i=1, def, j, check, sAge, idAge ; sAge = GetAges(i); S42MigrationStep(i, 1, true))
  {
    idAge=GetAges(i,1);
    def = TransformObject(idAge,Equal(iTechLevel, i));
    AddMenuItem(sAge, "SetAge", idAge, pClonk, nil, i, nil, 4, def);
    if(Equal(i, iSelection))
       check = true;
    if(!check)
       S42MigrationStep(j, 1, true);
    RemoveObject(def);
  }
  // Fertig
  AddMenuItem("$Finished$", "OpenMenu", CNCL, pClonk);
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func SetAge(id dummy,int i)
{
  i ??= 0;
  iTechLevel=i;
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  // Menn wieder aufmachen
  OpenTechLevelMenu(nil, i);
}

func GetAges(i,fDef)
{
  S42MigrationStep(i, -1, true);
  if(!S42MigrationStep(i, -1, true)){if(fDef) return ROCK; return "Steinzeit";}// Keine Technologien
  if(!S42MigrationStep(i, -1, true)){if(fDef) return METL; return "Mittelalter";}// Ein paar Technologien
  if(!S42MigrationStep(i, -1, true)){if(fDef) return STGR; return "Industriezeit";}// Viele Technologien
  if(!S42MigrationStep(i, -1, true)){if(fDef) return BRST; return "Neuzeit";}// Alle Technologien
}

/* Konfiguration abschlie_en */

func ConfigurationFinished()  // beachten, dass einstellungen auch während spiel änderbar sein sollen und dort wird diese fkt nicht aufgerufen!
{
  if(Death)   // dies nur machen, wenn spaeter nochmal die settings aufgerufen werden
 {
// var i = 0, j, pCrew, tmp,idDef,obj;   // das hier sollte nur einmal zu anfang und nicht danach gemacht werden, weil sonst anderweiztig zugefügte regeln entfernt werden!
  // for(var check in aRules)
    // {
    // idDef=GetDefinition(i, Chooser_Cat_Rule);
    // if(check)
      // {
      // if(!FindObject2(Find_ID(idDef)))
      // {obj=CreateObject(idDef, 10,10, -1);
      // obj->~IsChoosen();
      // log = Format("%s, %s", log, GetName(0, idDef));}
      // }
    // else
      // {
      // obj=FindObject2(Find_ID(idDef),Find_Exclude(this())); // wenn etwas da, obwohl nicht gewählt, dann entfernen
      // if(obj) if(!obj->~IsNotChooseable()) RemoveObject(obj);
      // idDef->~IsNotChoosen();
      // }
    // i++;
    // }

// var i = 0, j, pCrew, tmp,idDef,obj;
  // for(var check in aEffekte)
    // {
    // idDef=GetDefinition(i, Chooser_Cat_Rule);
    // if(check)
      // {
      // if(!FindObject2(Find_ID(idDef)))
      // {obj=CreateObject(idDef, 10,10, -1);
      // obj->~IsChoosen();
      // log = Format("%s, %s", log, GetName(0, idDef));}
      // }
    // else
      // {
      // obj=FindObject2(Find_ID(idDef),Find_Exclude(this())); // wenn etwas da, obwohl nicht gewählt, dann entfernen
      // if(obj) if(!obj->~IsNotChooseable()) RemoveObject(obj);
      // idDef->~IsNotChoosen();
      // }
    // i++;
    // }

// var i = 0, j, pCrew, tmp,idDef,obj;
  // for(var check in aEnvironment)
    // {
    // idDef=GetDefinition(i, Chooser_Cat_Environment);
    // if(check)
      // {
      // if(!FindObject2(Find_ID(idDef)))
      // {obj=CreateObject(idDef, 10,10, -1);
      // obj->~IsChoosen();
      // log = Format("%s, %s", log, GetName(0, idDef));}
      // }
    // else
      // {
      // obj=FindObject2(Find_ID(idDef),Find_Exclude(this())); // wenn etwas da, obwohl nicht gewählt, dann entfernen
      // if(obj) if(!obj->~IsNotChooseable()) RemoveObject(obj);
      // idDef->~IsNotChoosen();
      // }
    // i++;
    // }
    
    // die gewählten startforschungen einstellen, falls es iwie verstellt wurde, ohne die change fkt aufzurufen
    if (And(!Equal(wissenvorher, HashGet(hashSpielRegeln, "StartZeitalter")), (!FindObject(SZRE)))) // nur wenn StartZeitalter geändert wurde, dann das gnaze wissen löschen und neu setzen
  {
    var num, id;
    for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
      {
      num = nil;
      while (id = GetDefinition(S42MigrationStep(num, 1, true))) SetPlrKnowledge(GetPlayerByIndex(i), id,1); // das gesamte wissen entfernen und danach neu verteilen
      }
    GlobalStartingEra(HashGet(hashSpielRegeln, "StartZeitalter")); // die Funktionen für das StartZeitalter aufrufen, welche sich in den forschungsbaum skripten befinden
  }
  if(FindObject(CHHH)) // wenn Regel, dass alle Chemiedinge immer erforscht sein sollen, dann diese Forschungen auch noch vergeben. Kann nicht rückgängig gemacht werden, zu kompliziert
    {
        for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
       {
            var Chemieliste = FindObject(CHHH)->ChemieForschungen();
            for(var element in Chemieliste)
           {
                SetPlrKnowledge(GetPlayerByIndex(i), element);
           }
        }
    }
    // wird im init der spieler gemacht, da in settings nicht veränderbar
  // if(FindObject(SZRE)) // szenarioforschungen verteilen. Darf nur einmal zu beginn aufgerufen werden, 
    // {
        // for(var i = 0; i < GetPlayerCount(); i++)
           // {
               // for(var element in gForschungerforschtlisteVariable)
               // {
                    // SetPlrKnowledge(GetPlayerByIndex(i), element);
               // }    
           // }
    // }
  
  if(!FindObject(GGGG))
  {
      var muss=nil;
      for(var inumber=nil; (inumber ?? 0)<(GetLength(HashGet(hashSpielRegeln,"AbbauEreignisse")[0]) ?? 0);S42MigrationStep(inumber, 1, true))
      {
          if(HashGet(hashSpielRegeln,"AbbauEreignisse")[2][(inumber ?? 0)]) muss = 1;  // wenn eines davon gesetzt ist, muss regel erstellt werden
      }
      if(muss) CreateObject(GGGG,1,1,-1); 
  }

    // hier noch Regeln erstellen, die im Zuge der variablen Einstellungen erzeugt werden müssen
    var zeitregel;  
     // Log(Format("objekte vorher: %v. Wert: %v",ObjectCount2(Find_ID(PP2P)),HashGet(hashSpielRegeln, "Ausbildungszeit")));
    if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), nil))
     {
    while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel);
    CreateObject(PP2P,1,1,-1); // -> instant
     }
    if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), 5))
     {
    while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel);
    CreateObject(PP2P,1,1,-1); 
    CreateObject(PP2P,1,1,-1); // -> halbe Zeit
     }
    if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), 10))
     {
    while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel); // -> standard Zeit
     }
     // Log(Format("objekte nachher: %v . Wert: %v",ObjectCount2(Find_ID(PP2P)),HashGet(hashSpielRegeln, "Ausbildungszeit")));
    
    // verdunsten erstellen
    var verdunsten;
    if (Or(Or(HashGet(hashSpielRegeln, "Verdunstennormal"), HashGet(hashSpielRegeln, "Verdunstenwarm")), HashGet(hashSpielRegeln, "Verdunstenheiß")))
    {
        while(verdunsten=FindObject2(Find_ID(ECLD))) RemoveObject(verdunsten);
        CreateObject(ECLD,1,1,-1); 
    }
    else  while(verdunsten=FindObject2(Find_ID(ECLD))) RemoveObject(verdunsten);
    
    // Grundwasser GRLV
    if(HashGet(hashSpielRegeln, "Grundwasser"))
    {
        if(!FindObject(GRLV))
        {
            CreateObject(GRLV,1,1,-1);
        }
    }
    else
    {
        if(FindObject(GRLV))
        {
            RemoveAll(GRLV);
        }
    }
    
    // Barriere
    if (HashGet(hashSpielRegeln, "Barriere"))
    {
        if(!FindObject(_BAR))
        {
            CreateObject(_BAR,1,1,-1);
        }
    }
    else
    {
        if(FindObject(_BAR))
        {
            RemoveAll(_BAR);
        }
    }
    
    // noch die verlinkten Forschungen erforschen:
    for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
    {
          var def_num=nil, def;
          while (def = GetDefinition(S42MigrationStep(def_num, 1, true)))
          {
            var link;
            if (link = DefinitionCall(def, "GetResearchLink"))
            {
                  // if(link==BBUN) Log("BBUN vorhanden!");
                  if(GetPlrKnowledge(GetPlayerByIndex(i), link))  // wenn die verlinkte technologie bekannt ist, dann erforscht
                  {
                      SetPlrKnowledge(GetPlayerByIndex(i), def);
                      // Log(Format("link: %v -> erforscht: %v",link,def));
                  }
            }

          }
    }
  
    _Hash_DebugDump(hashSpielRegeln);
    return;
 }
 
 // folgendes NUR machen, wenn das erste mal !!!!!!!!!!!!!!!!!!!!
 
 _Hash_DebugDump(hashSpielRegeln);

  Death = true;
  ScriptGo(1);
  //SetCategory(1);

    
    // die gewählten startforschungen einstellen, hier gibts kein wissenvorher, es muss immer eingestellt werden, außer es sollen die dinge vom szenario genmmen werden
    
    if (!FindObject(SZRE)) // da startzeitalter in den einstellungen verändert werden kann, muss es hier einmal gemacht werden
    {
        var num, id;
        for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
          {
          num = nil;
          while (id = GetDefinition(S42MigrationStep(num, 1, true))) SetPlrKnowledge(GetPlayerByIndex(i), id,1); // das gesamte wissen entfernen und danach neu verteilen
          }
        GlobalStartingEra(HashGet(hashSpielRegeln, "StartZeitalter")); // die Funktionen für das StartZeitalter aufrufen, welche sich in den forschungsbaum skripten befinden
    }
    // diese Forschungen stattdessen fuer jeden Spieler im initialzie machen, da es in settings eh nicht geändert werden kann
    // !!!!
    // else // wenn es da ist, dann die im szenario skript erwähnten standardforschungen erforschen. eig nur verwenden, wenn kein forschungslabor in mission gebaut werden kann, aber das muss dann halt manuell berücksichtigt werden.
    // {
        // for(var i = 0; i < GetPlayerCount(); i++)
           // {
               // for(var element in gForschungerforschtlisteVariable)
               // {
                    // SetPlrKnowledge(GetPlayerByIndex(i), element);
               // }    
           // }
    // }
    // kann geändert werden, also nur bei laufzeitbeitritt im init und sonst hier einmal machen
    if(FindObject(CHHH)) // wenn Regel, dass alle Chemiedinge immer erforscht sein sollen, dann diese Forschungen auch noch vergeben. Kann nicht rückgängig gemacht werden, zu kompliziert
    {
        for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
       {
            var Chemieliste = FindObject(CHHH)->ChemieForschungen();
            for(var element in Chemieliste)
           {
                SetPlrKnowledge(GetPlayerByIndex(i), element);
           }
        }
    }
    
    
    
    
    
    if(!FindObject(GGGG))
  {
      var muss=nil;
      for(var inumber=nil; (inumber ?? 0)<(GetLength(HashGet(hashSpielRegeln,"AbbauEreignisse")[0]) ?? 0);S42MigrationStep(inumber, 1, true))
      {
          if(HashGet(hashSpielRegeln,"AbbauEreignisse")[2][(inumber ?? 0)]) muss = 1;  // wenn eines davon gesetzt ist, muss regel erstellt werden
      }
      if(muss) CreateObject(GGGG,1,1,-1); 
  }
    
    // hier noch Regeln erstellen, die im Zuge der variablen Einstellungen erzeugt werden müssen
    var zeitregel;  
     // Log(Format("objekte vorher: %v. Wert: %v",ObjectCount2(Find_ID(PP2P)),HashGet(hashSpielRegeln, "Ausbildungszeit")));
    if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), nil))
     {
    while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel);
    CreateObject(PP2P,1,1,-1); // -> instant
     }
    if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), 5))
     {
    while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel);
    CreateObject(PP2P,1,1,-1); 
    CreateObject(PP2P,1,1,-1); // -> halbe Zeit
     }
    if(Equal(HashGet(hashSpielRegeln, "Ausbildungszeit"), 10))
     {
    while(zeitregel=FindObject2(Find_ID(PP2P))) RemoveObject(zeitregel); // -> standard Zeit
     }
     // Log(Format("objekte nachher: %v . Wert: %v",ObjectCount2(Find_ID(PP2P)),HashGet(hashSpielRegeln, "Ausbildungszeit")));
    
    // verdunsten erstellen
    var verdunsten;
    if (Or(Or(HashGet(hashSpielRegeln, "Verdunstennormal"), HashGet(hashSpielRegeln, "Verdunstenwarm")), HashGet(hashSpielRegeln, "Verdunstenheiß")))
    {
        while(verdunsten=FindObject2(Find_ID(ECLD))) RemoveObject(verdunsten);
        CreateObject(ECLD,1,1,-1); 
    }
    else  while(verdunsten=FindObject2(Find_ID(ECLD))) RemoveObject(verdunsten);
    
    // Grundwasser GRLV
    if(HashGet(hashSpielRegeln, "Grundwasser"))
    {
        if(!FindObject(GRLV))
        {
            CreateObject(GRLV,1,1,-1);
        }
    }
    else
    {
        if(FindObject(GRLV))
        {
            RemoveAll(GRLV);
        }
    }
    
    // Barriere
    if (HashGet(hashSpielRegeln, "Barriere"))
    {
        if(!FindObject(_BAR))
        {
            CreateObject(_BAR,1,1,-1);
        }
    }
    else
    {
        if(FindObject(_BAR))
        {
            RemoveAll(_BAR);
        }
    }
    
    // noch die verlinkten Forschungen erforschen:
    for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
    {
          var def_num=nil, def;
          while (def = GetDefinition(S42MigrationStep(def_num, 1, true)))
          {
            var link;
            if (link = DefinitionCall(def, "GetResearchLink"))
            {
                  // if(link==BBUN) Log("BBUN vorhanden!");
                  if(GetPlrKnowledge(GetPlayerByIndex(i), link))  // wenn die verlinkte technologie bekannt ist, dann erforscht
                  {
                      SetPlrKnowledge(GetPlayerByIndex(i), def);
                      // Log(Format("link: %v -> erforscht: %v",link,def));
                  }
            }

          }
    }
    

  if(iTechLevel)
  {
    var idRes,res;
    i=j=nil;
    while(idRes=GetDefinition(S42MigrationStep(i, 1, true),1))
    {
      if(DefinitionCall(idRes,"IsTech"))
      {
        if(res=DefinitionCall(idRes,"IsStartTech"))
          if((res ?? 0)<=(iTechLevel ?? 0))
            for(j = nil; (j ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(j, 1, true))
              DefinitionCall(idRes,"Researched",GetPlayerByIndex(j),nil,true);
      }
    }
  }
  
  
  var i = nil, j, pCrew, tmp,idDef,obj,log = "Wahl abgeschlossen, Regeln: ";
  if(!FindObject2(Find_Category(C4D_Goal))) Message("Hinweis: Kein Ziel gewählt");
    //GameCallEx("PreConfig");
    //Spielziele melden
    Log("<c 0000ff>$Goals$</c>");
    for(var goal in FindObjects(Find_Category(C4D_Goal)))
     Log("{{%i}}::<c 0000ff>%s</c>",GetID(goal),GetName(goal));
    //GameCallEx("ConfigurateGoals");
    // Regeln erzeugen
    //Log("<c ff9900>$Rules$</c>");
  for(var check in aRules)
    {
    idDef=GetDefinition(i, Chooser_Cat_Rule);
    if(check)
      {
          if(!FindObject2(Find_ID(idDef)))
          {
              obj=CreateObject(idDef, 10,10, -1);
              obj->~IsChoosen();
          }
          log = Format("%s, %s", log, GetName(nil, idDef));
      }
    else
      {
          obj=FindObject2(Find_ID(idDef),Find_Exclude(this())); // wenn etwas da, obwohl nicht gewählt, dann entfernen
          if(!Equal(idDef, PP2P)) if(obj) if(!obj->~IsNotChooseable())
          {
            RemoveObject(obj);  // PP2P nicht entfernen. Auch notchoosable dinge da lassen, da diese von anderen dingen gesteuert werden
            idDef->~IsNotChoosen();
          }
          if(obj) log = Format("%s, %s", log, GetName(nil, idDef)); // wenn noch da, dann soll es auch da sein
      }
    S42MigrationStep(i, 1, true);
    }
  log = Format("%s || UMWELTOBJEKTE: ", log);
  
  var i = nil, j, pCrew, tmp,idDef,obj;
  for(var check in aEnvironment)
    {
    idDef=GetDefinition(i, Chooser_Cat_Environment);
    if(check)
      {
      if(!FindObject2(Find_ID(idDef)))
          {
          obj=CreateObject(idDef, 10,10, -1);
          obj->~IsChoosen();
          }
          log = Format("%s, %s", log, GetName(nil, idDef));
      }
    else
      {
          obj=FindObject2(Find_ID(idDef),Find_Exclude(this())); // wenn etwas da, obwohl nicht gewählt, dann entfernen
          if(obj) if(!obj->~IsNotChooseable()) 
          {
            RemoveObject(obj); // notchoosable dinge da lassen, da diese von anderen dingen gesteuert werden
            idDef->~IsNotChoosen();
          }
          if(obj) log = Format("%s, %s", log, GetName(nil, idDef)); // wenn noch da, dann soll es auch da sein
      }
    S42MigrationStep(i, 1, true);
    }
  Log(log);

  var i = nil, j, pCrew, tmp,idDef,obj;
  for(var check in aEffekte)
    {
    idDef=GetDefinition(i, Chooser_Cat_Rule);                    
    if(check)
      {
          if(!FindObject2(Find_ID(idDef)))
          {
                obj=CreateObject(idDef, 10,10, -1);
                obj->~IsChoosen();
          }
      }
    else
      {
          obj=FindObject2(Find_ID(idDef),Find_Exclude(this()));// wenn etwas da, obwohl nicht gewählt, dann entfernen
          if(!Equal(idDef, PP2P)) if(obj) if(!obj->~IsNotChooseable())
          {
            RemoveObject(obj); // notchoosable dinge da lassen, da diese von anderen dingen gesteuert werden
            idDef->~IsNotChoosen();
          }
      }
    S42MigrationStep(i, 1, true);
    }
  
  
  
  
  
  //Log("<c ff0000>$Difficulty$</c>::{{%i}}<c %x>%s</c>",idDifficulty,DefinitionCall(idDifficulty,"Color"),GetName(0,idDifficulty));
  //CreateObject(idDifficulty,0,0,-1);
  //SetPlrExtraData(GetPlayerByIndex(),"RdW_Last_Difficulty",idDifficulty->~IsDifficultyLevel());
  if(CampaignMode())
  {
    Log("Campaign Mode ?!");
  }
  // ein schneller GameCall fnr Einstellungen
  GameCallEx("ChooserFinished");

  // Spieler freilassen
  for(i = nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
    {
    for(j = nil ; pCrew = GetCrew(GetPlayerByIndex(i), j) ; S42MigrationStep(j, 1, true))
      {
      tmp = Contained(pCrew);
      if(Equal(GetID(tmp), TIM1))
       RemoveObject(tmp, 1);
      
      pCrew->~Recruitment(pCrew->GetOwner());
      pCrew->~Physicsanpassen(); // Clonk physicsanpassen, damit normale clonks bei angepasste physics regel schlechter sind. Nur wenn Regeln das erste mal festgelegt. Geht also nicht während des spiels für bereits erstellte clonks.
      }

    for(var rule in FindObjects(Find_Category(Chooser_Cat_Rule), Find_Exclude(this())))
      rule->~InitializePlayer(GetPlayerByIndex(i));
    for(var environment in FindObjects(Find_Category(Chooser_Cat_Environment), Find_Exclude(this())))
      environment->~InitializePlayer(GetPlayerByIndex(i));
    }
  // _berschnssiges TIM1-Objekte entfernen (falls Spieler ziwschenzeitlich geflogen sind)
  for(tmp in FindObjects(Find_ID(TIM1)))
    if(!(tmp->Contents()))
      RemoveObject(tmp, 1);

// Spielgeschwindigkeit wieder normal
 SetGameSpeed(nil);
 //GameCall("Spielstartet"); // ein Call an ein eventuelles Szenario skript

  // Selber entfernen
  //RemoveObject();
}

/* Generelles */

public func MenuQueryCancel() { return(1); }

func TransformObject(idDef,yes)
{
  var def = CreateObject(TIM1, nil,nil, -1);
  SetGraphics("",def,idDef,1,3);
  if(yes)
  {
    SetGraphics("", def, CNCL, 2, GFXOV_MODE_Picture);
    SetObjDrawTransform(650,nil,5000,nil,650,5000, def, 2);
  }
  else
  {
    SetClrModulation(RGBa(200,200,200,55),def);
    /*SetGraphics("2", def, CNCL, 1, GFXOV_MODE_Picture);
    SetObjDrawTransform(650,0,5000,0,650,5000, def, 1);*/
  }
  return def;
}

public func Eastern(object P)
{
  var rand = Random(6);
  if(!rand) SetName(GetPlayerName(GetPlayerByIndex(Random(GetPlayerCount()))), P);
  if(Equal(rand, 1)) SetName("$Emptiness$", P);
  if(Equal(rand, 2)) SetName("$YourBrain$", P);
  if(Equal(rand, 3)) SetName("$Nothing$", P);
  if(Equal(rand, 4)) SetName("$Freezer$", P);
  if(Equal(rand, 5)) SetName("$Space$", P);
}

public func IsActive()
{
  return !Death;
}

public func CampaignMode()
{
  return GameCall("IsCampaignMission");
}