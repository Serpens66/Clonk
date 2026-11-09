/*-- Clonk --*/

#strict 3
#appendto CLNK
#include CLNK

local AutoCollect,counnnter;
local intensi, rot, gruen, blau,iH,iS,iL,plusiH,plusiS,plusiL;
local AIM_precisely;
local aExplosiv;

static DerGameSpeed;
static Lichtersindaus;
static MLicht1,MLicht2,MLicht3,MLicht4,MLicht5,MLicht6;
static AntiFussel;

//transfer via Uplink
public func IsTransferabl() {
  return (1);
}

func ContextKletternHangeln(caller,iSelection) // man kann wählen, ob man will dass der Clonk klettert/hangelt. Zb gut, wenn man schnell nach unten will, der clonk also schnell fallen soll und sich nicht ständig überall festhalten
{
  [Können ändern|Image=S_10]
  CreateMenu(S_10,nil,nil,nil,"Können ändern",nil,1,nil);
    if(GetPhysical("CanScale")) 
        AddMenuItem("Klettern deaktivieren","ChangeKoennen",S_10,nil,nil,nil,"Der Clonk wird sich dann nicht mehr an allen Wänden usw. festhalten. Ganz praktisch, wenn man schnell irgendwo runter will.");
    else
        AddMenuItem("Klettern aktivieren","ChangeKoennen",S_10,nil,nil,nil,"Der Clonk wird wieder normal klettern.");
    if(GetPhysical("CanHangle")) 
        AddMenuItem("Hangeln deaktivieren","ChangeKoennen",S_11,nil,nil,1,"Der Clonk wird sich dann nicht mehr an allen Decken usw. festhalten.");
    else
        AddMenuItem("Hangeln aktivieren","ChangeKoennen",S_11,nil,nil,1,"Der Clonk wird wieder normal hangeln.");
    if(GetPhysical("CanDig")) 
        AddMenuItem("Graben deaktivieren","ChangeKoennen",S_20,nil,nil,2,"Der Clonk kann nicht mehr graben. Sinnvoll, wenn man z.b etwas aktivieren will und oft ausversehen gräbt.");
    else
        AddMenuItem("Graben aktivieren","ChangeKoennen",S_20,nil,nil,2,"Der Clonk kann wieder graben.");
    SelectMenuItem(iSelection, nil);
}

func ChangeKoennen(ID,izahl)
{
    if(Equal(ID, S_10))
    {
        if(GetPhysical("CanScale")) SetPhysical("CanScale", nil, PHYS_StackTemporary, this());
        else SetPhysical("CanScale", 1, PHYS_StackTemporary, this());
    }
    if(Equal(ID, S_11))
    {
        if(GetPhysical("CanHangle")) SetPhysical("CanHangle", nil, PHYS_StackTemporary, this());
        else SetPhysical("CanHangle", 1, PHYS_StackTemporary, this());
    }
    if(Equal(ID, S_20))
    {
        if(GetPhysical("CanDig")) SetPhysical("CanDig", nil, PHYS_StackTemporary, this());
        else SetPhysical("CanDig", 1, PHYS_StackTemporary, this());
    }
    ContextKletternHangeln(this(),izahl);
}



func ContextAntiFussel(caller,iSelection) // Aktiviert im TimerCall eine Freerect funktion im hauptclonkskript, welche festes material hinter dem clonk alle x frames entfernt. dadurch lassen sich fussel in der landschaft durch gegenspringen entfernen, ohne sonstige negative auswirkungen. dennoch besser nur aktivieren, wenn fussel entfernt werden sollen. 
{
[AntiFusselModus|Image=ERTH|Condition=ErsterSpieler]
  CreateMenu(ERTH,nil,nil,nil,"AntiFusselModus ändern",nil,1,nil);
    if(AntiFussel) 
        AddMenuItem("AntiFussel deaktivieren","SetAntiFussel",ERTH,nil,nil,nil,"Normales Clonk");
    if(!AntiFussel)    
        AddMenuItem("AntiFussel aktivieren","SetAntiFussel",ERTH,nil,nil,1,"Festes Material hinter dem Clonk wird entfernt. Auf diese Weise können nervige MaterialFussel in der Landschaft durch Gegenspringen entfernt werden.");
}

func SetAntiFussel(objID,izahl)
{
    AntiFussel = izahl;
    return(ContextAntiFussel(nil,izahl));
}

func ContextGameSettingss()
{
[GameSettings|Image=BOOM|Condition=ErsterSpieler]
  CreateMenu(GOOO,nil,nil,nil,"Gamespeed ändern",nil,1,nil);
    AddMenuItem("Gamespeed ändern","GameSpeedChange",GOLD,nil,nil,nil,"Passe die maximale FPS Zahl an");
    AddMenuItem("Explosionsverhalten ändern","OpenExplosivMenu",BOOM,nil,nil,1,"Soll die Erde bei Explosionen zerstört werden?");
    AddMenuItem("Tageszeit ändern","ChangeDayTimeMenu",TIME,nil,nil,nil);
    AddMenuItem("Settings ändern","Choosersettings",GOLD,nil,nil,nil);
}

func ChangeDayTimeMenu(dummy,iSelection)
{
  CreateMenu(TIME,nil,nil,nil,"Tageszeit ändern",nil,1,nil);
    AddMenuItem(Format("Erhöhen %v/100",HashGet(hashSpielRegeln, "Tageszeit")),"ChangeDayTime",PFEI,nil,nil,1,"0 bis 100",2,nil);
    AddMenuItem(Format("Vermindern %v/100",HashGet(hashSpielRegeln, "Tageszeit")),"ChangeDayTime",PFEI,nil,nil,-1,"0 bis 100",2,1);
    AddMenuItem("Eingestellte Tageszeit setzen","ChangeDayTime",TIME,nil,nil,2,"0-100 = Mittag bis Mittag");
SelectMenuItem(iSelection, nil);
}

func ChangeDayTime(dummy,iSelection)
{
if(Or(Equal(iSelection, 1), Equal(iSelection, -1)))
HashPut(hashSpielRegeln, "Tageszeit", BoundBy((HashGet(hashSpielRegeln, "Tageszeit") ?? 0)+(iSelection ?? 0),nil,100));
if(Equal(iSelection, 1)) iSelection=nil;
if(Equal(iSelection, -1)) iSelection=1;
  if(Equal(iSelection, 2))
  {
  var timeobj;
  timeobj = FindObject(TIME);
  if(timeobj)
  timeobj->SetTimeTIME(HashGet(hashSpielRegeln, "Tageszeit"));
  }
ChangeDayTimeMenu(dummy,iSelection);
}


func Choosersettings()
{
var obj; // den regelchooser finden und darin die fkt aufrufen
obj = FindObject(CHO6);
obj->OpenMenu();
}

func OpenExplosivMenu(id dummy, int iSelection)
{
iSelection ??= 0;
if(!GetLength(aExplosiv)) aExplosiv = CreateArray();
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i) ; S42MigrationStep(i, 1, true))
  {
   if(DefinitionCall(idR, "IsErdschaden") )
    aExplosiv[(i ?? 0)]=true;
   else
    aExplosiv[(i ?? 0)]=nil;
  }
  var pClonk = GetCursor(GetPlayerByIndex(nil,C4PT_User));
  // Menn aufmachen
  CreateMenu(BOOM, pClonk,nil,nil,"Markiert->Erdschaden");
  // Explosiv einfngen
  for(var i=nil, idR, def, j, check ; idR = GetDefinition(i) ; S42MigrationStep(i, 1, true))
    if(DefinitionCall(idR, "IsExplosiv"))
      {
      def = TransformObject(idR,aExplosiv[(i ?? 0)]);
      AddMenuItem("%s", "ChangeExplosivConf", idR, pClonk, nil, i, nil, 4, def);
      if(Equal(i, iSelection))
        check = true;
      if(!check)
        S42MigrationStep(j, 1, true);
      RemoveObject(def);
      }
  // Fertig
  AddMenuItem("Fertig", "ContextGameSettingss", CNCL, pClonk);
  AddMenuItem("Auf Standard zurücksetzen", "ExploStandard", BOOM, pClonk,nil,j);
  // Menneintrag auswShlen
  SelectMenuItem(j, pClonk);
}

func ExploStandard(dummy, iSelection)
{
var obj; // den regelchooser finden und darin die fkt aufrufen
obj = FindObject(CHO6);
obj->SetExplosivStandard();
OpenExplosivMenu(nil,iSelection);
}

func ChangeExplosivConf(id idExplosiv, int i)
{
  // Explosiv wurde ausgewShlt
  i ??= 0;
  if(!aExplosiv[(i ?? 0)])
  {
    aExplosiv[(i ?? 0)] = true;
    idExplosiv->~ ChangeErdschaden(1);
  }
  // Explosiv wurde abgewShlt
  else
  {
    aExplosiv[(i ?? 0)] = nil;
    idExplosiv->~ChangeErdschaden(nil);
  }
  // GerSusch
  Sound("Grab", 1,nil,nil,1);
  // Regelmenn wieder aufmachen
  OpenExplosivMenu(nil, i);
}

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


public func GameSpeedChange(pCaller,iSelection)
{
// nun davon ausgehen, dass der aktuelle Gamespeed 38, also Standard ist, sofern die Variable DerGameSpeed noch nicht gesetzt oder 0 ist
if(!DerGameSpeed) DerGameSpeed=38;
  CreateMenu(GOLD,nil,nil,nil,"Gamespeed ändern",nil,1,nil);
    AddMenuItem(Format("Um 1 FPS erhöhen (%d/50)",DerGameSpeed),"SendDenSpeed",PFEI,nil,nil,1,"1 bis 50",2,nil);
    AddMenuItem(Format("Um 1 FPS vermindern (%d/50)",DerGameSpeed),"SendDenSpeed",PFEI,nil,nil,-1,"1 bis 50",2,1);
    AddMenuItem("Auf Standard zurückstellen","SendDenSpeed",GOLD,nil,nil,nil);
    SelectMenuItem(iSelection);
}

func SendDenSpeed(dummy, izahl)
{
var Selection;
if(Equal(dummy, GOLD))
{ChangeDenSpeed(nil);Selection=2; DerGameSpeed=38; return(GameSpeedChange(nil,Selection));}
ChangeDenSpeed(BoundBy((DerGameSpeed ?? 0)+(izahl ?? 0),1,50));
DerGameSpeed=BoundBy((DerGameSpeed ?? 0)+(izahl ?? 0),1,50);
if(Equal(izahl, 1)) Selection=nil;
if(Equal(izahl, -1)) Selection=1;
GameSpeedChange(nil,Selection);
}

func ChangeDenSpeed(izahl)
{
SetGameSpeed(izahl); return(nil);
}

func ErsterSpieler()
{
  // Spieler 1? 
  if(Equal(GetPlayerByIndex(nil,C4PT_User), GetOwner()))
  return(1);
return(nil);
}

// präziser zielen mit der lokalen variablen AIM_precisely. Diese steht im canaim 
//skript und somit auch in jedem hazardclonk. Nur wenn klassische Steuerung
func WeaponDabei()
{
if (Contents(nil))
if (And(And(Contents(nil)->~IsWeapon(), !GetPlrCoreJumpAndRunControl(GetOwner())), Equal(GetID(), HZCK)))
return(1);
return(nil);
}

public func ContextZielmodus(pCaller) {
  [Zielmodus ändern|Image=HCRH|Condition=WeaponDabei]
  CreateMenu(GetID(Contents(nil)),nil,nil,nil,"Zielmodus ändern",nil,1,nil);
    if(!AIM_precisely) 
        AddMenuItem("Genaueres Zielen aktivieren","Zielmodus",PFEI,nil,nil,1,"Langsames, aber exakteres Zielen",2,nil);
    if(AIM_precisely)    
        AddMenuItem("Normales Zielen aktivieren","Zielmodus",PFEI,nil,nil,2,"Schnelles, aber ungenaues Zielen",2,1);
}

func Zielmodus(iImage,ichange)
{
if (Equal(ichange, 1)) AIM_precisely = 1;
if (Equal(ichange, 2)) AIM_precisely = nil;
}


// Granatwerferzündzeit ändern

func FindGrenadeLauncher()
{
if (Contents(nil))
if (Contents(nil)->~IsGrenadeLauncher() )
return(1);
return(nil);
}

public func ContextGranatzeit(pCaller) {
  [Zündzeit ändern|Image=GREN|Condition=FindGrenadeLauncher]
  CreateMenu(GREN,nil,nil,nil,"Zündzeit ändern",nil,1,1);
    AddMenuItem("Um 100 Milisekunden erhöhen","SendDelay",PFEI,nil,nil,1,"100 bis 10.000",2,nil);
    AddMenuItem("Um 100 Milisekunden vermindern","SendDelay",PFEI,nil,nil,2,"100 bis 10.000",2,1);
    AddMenuItem("Auf Standard zurückstellen","SendDelay",GREN,nil,nil,nil);
}

func SendDelay(iImage,ichange)
{
if (Contents(nil))
if (Contents(nil)->~IsGrenadeLauncher() )
Contents(nil)->~EditExplodedelay(ichange);
}

//Lichter über Contextmenü verstellen

// dazu müssen in die Lichtobjekte ein paar Funktionen, diese sind bisher zu finden in
// Gebäudemanager von General SpaceEmpire (daher alle gebäude mit Licht)
// StandLampe, Flutlicht, Deckenlicht, Hintergrundlicht


public func ContextLichtanpassen(pCaller) {
  [Licht ändern|Image=LIGH|Condition=FindLichtObject]
  if(!FindLichtObject()) return nil;
  CreateMenu(LIGH,nil,nil,nil,"Licht ändern",nil,1);
    AddMenuItem("Durchsichtigkeit verändern","Lichtaendernmenu",LIGH,nil,nil,1);
    AddMenuItem("Farbe verändern","Lichtaendernmenu",LIGH,nil,nil,2);
    AddMenuItem("Standardlicht wiederherstellen","Lichtaendern",LIGH,nil,nil,11);
    AddMenuItem("Alle eigenen Lichter ausschalten/einschalten (unsichtbar)","AlleLichter",LIGH,nil,nil,1);
    AddMenuItem("durch große lichter ersetzen (test)","AlleLichter",LIGH,nil,nil,2);
    //AddMenuItem("Alle eigenen Lichter entfernen (nur testzwecke, nicht rückgängig machbar)","AlleLichter",LIGH,0,0,3);   
AddMenuItem("$Nothing$","No",MCMX);
}



func AlleLichter(dummy, izahl)
{
if(Equal(Lichtersindaus, nil))
if(Equal(izahl, 1))
 {
Lichtersindaus=1;
for (var licht in FindObjects(Find_Func("IsLight"), Find_Owner(GetOwner()) ) )
{licht->~TurnOff();}
return;
 }

if(Equal(Lichtersindaus, 1))
if(Equal(izahl, 1))
 {
Lichtersindaus=nil;
for (var licht in FindObjects(Find_Func("IsLight"), Find_Owner(GetOwner())) )
{licht->~TurnOn();}
return;
 }

if(Equal(izahl, 3))
if(FindObject2(Find_Func("IsLight"), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER))) )
{for (var licht in FindObjects(Find_Func("IsLight"), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER))) )
{RemoveObject(licht);}
return;}

if(Equal(Lichtersindaus, nil))
if(Equal(izahl, 2))
 {
Lichtersindaus=1;
for (var licht in FindObjects(Find_Func("IsLight"), Find_Owner(GetOwner()) ) )
{licht->~TurnOff();}
var ziel1,ziel2,ziel3,ziel4,ziel5,ziel6;
ziel1=CreateObject(BLG3,780,820,GetOwner());
ziel2=CreateObject(BLG3,1140,820,GetOwner());
ziel3=CreateObject(BLG3,1550,820,GetOwner());
ziel4=CreateObject(BLG3,780,450,GetOwner());
ziel5=CreateObject(BLG3,1140,450,GetOwner());
ziel6=CreateObject(BLG3,1550,450,GetOwner());
SetPosition(780,820,ziel1);
SetPosition(1140,820,ziel2);
SetPosition(1550,820,ziel3);
SetPosition(780,450,ziel4);
SetPosition(1140,450,ziel5);
SetPosition(1550,450,ziel6);
MLicht1=AddLight(3500,RGBa(235,230,230,50),ziel1);
MLicht2=AddLight(3500,RGBa(235,230,230,50),ziel2);
MLicht3=AddLight(3500,RGBa(235,230,230,50),ziel3);
MLicht4=AddLight(3500,RGBa(235,230,230,50),ziel4);
MLicht5=AddLight(3500,RGBa(235,230,230,50),ziel5);
MLicht6=AddLight(3500,RGBa(235,230,230,50),ziel6);
return;
 }


if(Equal(Lichtersindaus, 1))
if(Equal(izahl, 2))
 {
Lichtersindaus=nil;
for (var licht in FindObjects(Find_Func("IsLight"), Find_Owner(GetOwner())) )
{licht->~TurnOn();}
if(MLicht1) RemoveObject(MLicht1);
if(MLicht2) RemoveObject(MLicht2);
if(MLicht3) RemoveObject(MLicht3);
if(MLicht4) RemoveObject(MLicht4);
if(MLicht5) RemoveObject(MLicht5);
if(MLicht6) RemoveObject(MLicht6);
return;
 }
}


func Lichtaendernmenu(objId,izahl)
{
  CreateMenu(LIGH,nil,nil,nil,"Licht ändern",nil,1,1);
if(Equal(izahl, 1))
    {
AddMenuItem("Durchsichtigkeit um 1 vermindern","Lichtaendern",PFEI,nil,nil,1,"0 bis 255",2,1);
AddMenuItem("Durchsichtigkeit um 1 erhöhen","Lichtaendern",PFEI,nil,nil,2,"0 bis 255",2,nil);
    }            

if(Equal(izahl, 2))
            {
AddMenuItem("Grünlich","Lichtaendern",LIGH,nil,nil,3);
AddMenuItem("Rötlich","Lichtaendern",LIGH,nil,nil,4);
AddMenuItem("Weißlich","Lichtaendern",LIGH,nil,nil,5);
AddMenuItem("Orangelich","Lichtaendern",LIGH,nil,nil,6);
AddMenuItem("Bläulich","Lichtaendern",LIGH,nil,nil,7);
AddMenuItem("Gelblich","Lichtaendern",LIGH,nil,nil,8);
AddMenuItem("Zufällig","Lichtaendern",LIGH,nil,nil,9);  // siehe knicklicht
AddMenuItem("Individuell","Lichtaendern",LIGH,nil,nil,10);
            }
}



func Lichtaendern(objId,izahl)
{
for (var lichtobj in FindObjects(Find_Func("IsLightObject"), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)) )  )
{
if(Equal(izahl, 1)) {intensi=-1;lichtobj->~LichtAnpassen(nil,nil,nil,intensi);}
if(Equal(izahl, 2)) {intensi=1;lichtobj->~LichtAnpassen(nil,nil,nil,intensi);}
if(Equal(izahl, 3)) {rot=41;gruen=181;blau=26;return(lichtobj->~LichtAnpassen(rot,gruen,blau));} // gruenlich
if(Equal(izahl, 4)) {rot=202;gruen=4;blau=4;return(lichtobj->~LichtAnpassen(rot,gruen,blau));} //rötlich
if(Equal(izahl, 5)) {rot=255;gruen=255;blau=255;return(lichtobj->~LichtAnpassen(rot,gruen,blau));} //weißlich
if(Equal(izahl, 6)) {rot=223;gruen=117;blau=1;return(lichtobj->~LichtAnpassen(rot,gruen,blau));} // oranglich
if(Equal(izahl, 7)) {rot=1;gruen=39;blau=222;return(lichtobj->~LichtAnpassen(rot,gruen,blau));} //Bläulich
if(Equal(izahl, 8)) {rot=255;gruen=255;blau=1;return(lichtobj->~LichtAnpassen(rot,gruen,blau));} //Gelblich

if(Equal(izahl, 9)) {
//HSL Farbgebung!
 iH = Random(256);
  iS = 255;
  iL = 128;
return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,iH,iS,iL));
             }
if(Equal(izahl, 10)) {CloseMenu();CreateMenu(LIGH,nil,nil,nil,"Individuelle Farbe",nil,1,1);
AddMenuItem("Farbenwert erhöhen","LichtIndivaendern",PFEI,nil,nil,1,"0 ist Rot, 85 ist Grün, 170 Blau und 255 wieder Rot",2,nil);
AddMenuItem("Farbenwert vermindern","LichtIndivaendern",PFEI,nil,nil,2,"0 ist Rot, 85 ist Grün, 170 Blau und 255 wieder Rot",2,1);
AddMenuItem("Sättigung erhöhen","LichtIndivaendern",PFEI,nil,nil,3,"Dies ist der Farbkontrast, 0 bis 255",2,nil);
AddMenuItem("Sättigung vermindern","LichtIndivaendern",PFEI,nil,nil,4,"Dies ist der Farbkontrast, 0 bis 255",2,1);
AddMenuItem("Helligkeit erhöhen","LichtIndivaendern",PFEI,nil,nil,5,"Helligkeit gleich null entspräche RGB(0,0,0), 0 bis 255",2,nil);
AddMenuItem("Helligkeit vermindern","LichtIndivaendern",PFEI,nil,nil,6,"Helligkeit gleich null entspräche RGB(0,0,0), 0 bis 255",2,1);
return;
              }
// standardlicht, wenn alle 3 RGB Werte 1000
if(Equal(izahl, 11)) lichtobj->~return(lichtobj->~LichtAnpassen(1000,1000,1000,nil,nil,nil,nil,nil,nil,nil));  }
//return(lichtobj->~LichtAnpassen(rot,gruen,blau,intensi,iH,iS,iL,plusiH,plusiS,plusiL)); 

}

func LichtIndivaendern(objID,izahl)
{
  var lichtobj;
lichtobj = FindObject2(Find_AtRect(-5,-5,10,20), Find_Func("IsLightObject"), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)) );
if(Equal(izahl, 1)) {plusiH=1;return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,nil,nil,nil,plusiH,nil,nil));}
if(Equal(izahl, 2)) {plusiH=-1;return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,nil,nil,nil,plusiH,nil,nil));}
if(Equal(izahl, 3)) {plusiS=1;return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,nil,nil,nil,nil,plusiS,nil));}
if(Equal(izahl, 4)) {plusiS=-1;return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,nil,nil,nil,nil,plusiS,nil));}
if(Equal(izahl, 5)) {plusiL=1;return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,nil,nil,nil,nil,nil,plusiL));}
if(Equal(izahl, 6)) {plusiL=-1;return(lichtobj->~LichtAnpassen(nil,nil,nil,nil,nil,nil,nil,nil,nil,plusiL));}
//return(lichtobj->~LichtAnpassen(0,0,0,0,0,0,0,plusiH,plusiS,plusiL));
}







public func FindLichtObject()
{
  var lichtobj;
lichtobj = FindObject2(Find_AtRect(-5,-5,10,20), Find_Func("IsLightObject"), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)) );
      return(lichtobj);
}


public func ContextTaucherhelmAblegen(pCaller)
{
  [$DepositHelm$|Image=THLM|Condition=FindHelm]
  var line= FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",FindHelm()),helmet;
  if(!(helmet= FindHelm())) return(nil);
  if(line) RemoveObject(line);
  helmet->~ Extract(this());
}

public func ContextTaucherhelmAnschliessen(pCaller)
{
  [$ConnectHelm$|Image=LUPU|Condition=FindPump]
  ObjectSetAction(CreateObject(PWRL,nil,nil,GetOwner()),"Connect",FindHelm(),FindPump());
  Sound("Connect");
}

//deaktiviert, weil Abriss von Space Empire verwendet wird
/*
public func ContextAbriss(pCaller)
{
  [$TearOffBuilding$|Image=CXCN|Condition=FindConstruction]
  var building;
  if(!(building= FindConstruction())) return(0);
  CreateMenu(CXCN,0,0,0,Format("$TearOff$",GetName(building)),0,1);
  AddMenuItem("$Yes$","DoDestruct",GetID(building),0,0,building,0,0,4,building);
  AddMenuItem("$No$","No",MCMX);
}

private func DoDestruct(idObj,pObj)
{
  if(FindObject(0,0,0,0,0,OCF_Collectible(),0,0,pObj) && (GetOCF(pObj) & OCF_Container()))
    return(Message("$MustBeEmpty$",this(),GetName(pObj)),Sound("CommandFailure1"));
  var content;
  while(content= FindObject(0,0,0,0,0,OCF_Collectible(),0,0,pObj,content))
   RemoveObject(content);
  ScheduleCall(this(),"Abriss",1,GetCon(pObj) +5,pObj);
  var wing;
  if(idObj== WMIL)
   if(wing= FindObject(WWNG,0,0,0,0,0,"Turn",pObj))
    ScheduleCall(this(),"Abriss",1,GetCon(wing) +5,wing);  
}

private func Abriss(pObj)
{
  if(!pObj) return(0);
  DoCon(-1,pObj);
  if(!Random(25)) Sound("Build*",0,pObj);
}


*/

public func ContextReperatur(pCaller) {
  [$RepairObject$|Image=CXCN|Condition=FindDamagedObject]
  if(!FindDamagedObject()) return nil;
  CreateMenu(CXCN,nil,nil,nil,"$WhatIsToRepair$",nil,1);
  for(var obj in FindObjects(Find_AtRect(-5,-5,10,20), Find_NoContainer(), Find_Or(Find_Func("GetDefCoreVal", "BlastIncinerate", "DefCore"), Find_Func("GetMaxDamage")), Find_Or(Find_Func("GetDefCoreVal", "Construction", "DefCore"), Find_Func("IsRepa")),Find_Not(Find_Func("IsBasement")), Find_Func("GetDamage"))) {
    AddMenuItem(Format("%s (%d C.)",GetName(obj),(GetDamage(obj) ?? 0) /2),"Repair",GetID(obj),nil,nil,obj,nil,nil,4,obj);
  }
  AddMenuItem("$Nothing$","No",MCMX);
}



public func ContextSchuttDrehen(pCaller) {
  ["Schutt drehen"|Image=DE_5|Condition=FindSchuttObject]
  if(!FindSchuttObject()) return nil;
  CreateMenu(DE_5,nil,nil,nil,"Schutt drehen",nil,1);
  for(var obj in FindObjects(Find_AtRect(-5,-5,10,20), Find_ID(DE_5), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)) )) {
    AddMenuItem("Schutt drehen","EinmalDrehen",DE_5,nil,nil,obj);
  }
  AddMenuItem("$Nothing$","No",MCMX);
}

public func FindSchuttObject()
{
  var obj;
obj = FindObject2(Find_AtRect(-5,-5,10,20), Find_ID(DE_5), Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)) );
      return(obj);
}

func EinmalDrehen(objID, objekt)
{
//Log(Format("das Objekt ist %v und die Phase ist %v",objekt,GetPhase(objekt)));
if(Equal(GetPhase(objekt), nil)) {objekt->~SetDirr(1);return;}
if(Equal(GetPhase(objekt), 1)) {objekt->~SetDirr(nil);return;}
}


func FindBuildinggabgeschlossen()
// gebäude finden, welches abgeschlossen ist
{
var abgeschlobjekt;
if (abgeschlobjekt = FindObject2(Find_Or(Find_OCF(OCF_Entrance),Find_ID(SLTW),Find_ID(CGDT),Find_ID(CDFT)),Find_AtRect(-5,-5,10,20),Find_NoContainer()))
if((abgeschlobjekt->~Abgeschlossenn())) return(1);
return(nil);
}

//abgeschlossene Gebäude knacken
public func ContextKnackenn(pCaller)
{
  [Schloss knacken|Image=SHM6|Condition=FindBuildinggabgeschlossen]
ScheduleCall(nil, "Schloossknacken", 5, nil, pCaller);counnnter=nil;PlayerMessage(GetOwner(pCaller),"Schloss wird geknackt...",this());
}

func Schloossknacken(pCaller)
{
if(Or(!Equal(GetComDir(this()), nil),
 ( And(And(!SEqual(GetAction(this()), "Walk"), !SEqual(GetAction(this()), "HoldWalk")), !SEqual(GetAction(this()), "WalkArmed")))) ) // nur weitermachen, wenn Clonk stillsteht
{PlayerMessage(GetOwner(pCaller),"Schloss knacken abgebrochen",this());counnnter=nil;return;}

var abgeschlobjekt = FindObject2(Find_Or(Find_OCF(OCF_Entrance),Find_ID(SLTW),Find_ID(CGDT),Find_ID(CDFT)),Find_AtRect(-5,-5,10,20),Find_NoContainer(),Find_Func("Abgeschlossenn") );

S42MigrationStep(counnnter, 1);
// wenn feindlich, dauerts länger
if(!Equal(GetOwner(abgeschlobjekt), GetOwner(pCaller))) if(Hostile(GetOwner(abgeschlobjekt),GetOwner(pCaller)))
  {
// wenn stabilere Schlösser LOOC erforscht, dauerts sogar 4000 Frames
if(GetPlrKnowledge(GetOwner(abgeschlobjekt),LOOC))
   {
var counnnterhundert = (counnnter ?? 0)/40;
if((counnnter ?? 0)<4000)                     // dauert 4000Frames, in der Zeit muss der Clonk still stehen.
 {ScheduleCall(nil, "Schloossknacken", 1, nil, pCaller);
PlayerMessage(GetOwner(pCaller),"%v%",this(),counnnterhundert); return;
 }
if(Equal(counnnter, 4000))
        {
if(!abgeschlobjekt) {PlayerMessage(GetOwner(pCaller),"Aufzubrechendes Objekt weg?!",this());return;}

if(Equal(LocalN("zuu",abgeschlobjekt), 1)) LocalN("zuu",abgeschlobjekt)=nil;
counnnter=nil;
PlayerMessage(GetOwner(pCaller),"Schloss geknackt!",this()); return;
         }
   }
// wenn noch nicht erforscht, dann nur doppelt so lang, wie verbündete  
else {
var counnnterhundert = (counnnter ?? 0)/20;
if((counnnter ?? 0)<2000)                     // dauert 2000Frames, in der Zeit muss der Clonk still stehen.
 {ScheduleCall(nil, "Schloossknacken", 1, nil, pCaller);
PlayerMessage(GetOwner(pCaller),"%v%",this(),counnnterhundert); return;
 }
if(Equal(counnnter, 2000))
 {
if(!abgeschlobjekt) {PlayerMessage(GetOwner(pCaller),"Aufzubrechendes Objekt weg?!",this());return;}

if(Equal(LocalN("zuu",abgeschlobjekt), 1)) LocalN("zuu",abgeschlobjekt)=nil;
counnnter=nil;
PlayerMessage(GetOwner(pCaller),"Schloss geknackt!",this()); return;
 }
  }
   }

// wenn nicht feindlich
var counnnterhundert = (counnnter ?? 0)/10;
if((counnnter ?? 0)<1000)                     // dauert 1000Frames, in der Zeit muss der Clonk still stehen.
 {ScheduleCall(nil, "Schloossknacken", 1, nil, pCaller);
PlayerMessage(GetOwner(pCaller),"%v%",this(),counnnterhundert); return;
 }

if(Equal(counnnter, 1000))
 {
if(!abgeschlobjekt) {PlayerMessage(GetOwner(pCaller),"Aufzubrechendes Objekt weg?!",this());return;}

if(Equal(LocalN("zuu",abgeschlobjekt), 1)) LocalN("zuu",abgeschlobjekt)=nil;
counnnter=nil;
PlayerMessage(GetOwner(pCaller),"Schloss geknackt!",this()); return;
 }
}



func FindBuildingg()
// gebäude finden und schlüssel dabei haben
{
if (And(And(FindObject2(Find_Or(Find_OCF(OCF_Entrance),Find_ID(SLTW),Find_ID(CGDT),Find_ID(CDFT)),Find_AtRect(-5,-5,10,20),Find_NoContainer()), FindContents(SHM6,this())), Contained(this())) ) return(1);
return(nil);
}

//damit man den Schlüssel auch innerhalb von Gebäuden verwenden und von innen auf-/abschließen kann
public func ContextKey(pCaller)
{
  [Schlüssel verwenden|Image=SHM6|Condition=FindBuildingg]
var key= FindContents(SHM6,this());
if(key) {
//Log("Key gefunden");
key->Activate(pCaller);};
}

public func ContextBarrelSellControl(pCaller)
{
  [$SaleMenu$|Image=SY02|Condition=InBase2]
  ObjectCall(FindObject(ATFU,nil,nil,nil,nil,nil,"Checking",Contained()),"BarrelSellControl",pCaller);
  return(1);
}

public func ContextContainerControl(pCaller) 
{
  [$AutomaticBarrel$|Image=ATFU|Condition=InBase1]
  Sound("KnightConfirm*");
  if(FindObject(ATFU,nil,nil,nil,nil,nil,"Checking",Contained()))
    {
    PlayerMessage(GetOwner(),"$NoSupervising$",Contained());
    RemoveObject(FindObject(ATFU,nil,nil,nil,nil,nil,"Checking",Contained()));
    }
  else
    {
    PlayerMessage(GetOwner(),"$Supervising$",Contained());
    ObjectSetAction(CreateObject(ATFU),"Checking",Contained());
    }
  return(1);
}



public func InBase1()
{
  if(!Contained()) return(nil);
  if(Equal(GetBase(Contained()), -1)) return(nil);
  return(1);
}

public func InBase2()
{
  if(!Contained()) return(nil);
  if(Equal(GetBase(Contained()), -1)) return(nil);
  if(!FindObject(ATFU,nil,nil,nil,nil,nil,"Checking",Contained())) return(nil);
  return(1);
}

// schlafen von der Schlafen Regel im Kontextmenü aufrufbar, damit man nichts ins regelmenue muss
public func ContextSchlafen(pCaller) {
  [Schlafen|Image=SLEP|Condition=FindSchlafregel]
  FindObject(SLEP)->Activate(GetOwner(pCaller)); // menue der schalfen regel selbst aufrufen.
}

func FindSchlafregel()
{
    if(FindObject(SLEP))return(1);
    return(nil);
}

private func Repair(idObj,pObj)
{
  if((GetWealth(GetOwner()) ?? 0)< ((GetDamage(pObj) ?? 0) /2))
    return(S42MigrationFirst(Message("$NotEnoughMoney$",this()),Sound("Error")));
  SetWealth(GetOwner(),(GetWealth(GetOwner()) ?? 0)- ((GetDamage(pObj) ?? 0) /2));
  Sound("Cash");
  ScheduleCall(this(),"Repairing",1,GetDamage(pObj),pObj);
}

private func Repairing(pObj)
{
  DoDamage(-1,pObj);
  if(!Random(15)) Sound("Build*",nil,pObj);
}

private func No() { return; }

public func FindHelm()
{
  if(InLiquid()) return(nil);
  return(FindObject(THLM,nil,nil,nil,nil,nil,"Wear",this()));
}

public func FindPump()
{
  if(!FindHelm()) return(nil);
  if(FindObject(PWRL,nil,nil,nil,nil,nil,"Connect",FindHelm())) return(nil);
  return(FindObject(LUPU,-16,-20,32,40));
}

public func FindConstruction()
{
  var obj;
  while(obj= FindObject(nil,-1,+9,nil,nil,nil,nil,nil,NoContainer(),obj))
   if((GetCategory(obj) ?? 0) & (C4D_Structure ?? 0))
    if(Or(Equal(GetOwner(obj), GetOwner()), Equal(GetOwner(obj), NO_OWNER)))
     return(obj);
}

public func FindDamagedObject()
{
  var obj;
obj = FindObject2(Find_AtRect(-5,-5,10,20), Find_NoContainer(), Find_Or(Find_Func("GetDefCoreVal", "BlastIncinerate", "DefCore"), Find_Func("GetMaxDamage")), Find_Or(Find_Func("GetDefCoreVal", "Construction", "DefCore"), Find_Func("IsRepa")),Find_Not(Find_Func("IsBasement")), Find_Func("GetDamage"),Find_Not(Find_OCF(OCF_OnFire)));
      return(obj);
}

protected func Death(a,b,c)
{
  var beutel;
  var sack;
  var kiste;
  if(beutel= FindObject(BETL,nil,nil,nil,nil,nil,"Belongs",this())) beutel->~ Loose(nil,1);
  if(sack= FindObject(SAK1,nil,nil,nil,nil,nil,"Belongs",this())) sack->~ Loose(nil,1);
  if(kiste= FindObject(KI_T,nil,nil,nil,nil,nil,"Belongs",this())) kiste->~ Loose(nil,1);
  if(!GetCrew(GetOwner())) Geldbeutel();
  return(_inherited(a,b,c));
}

protected func Destruction(a,b,c)
{
  if(Equal(GetCrew(GetOwner()), this()))
   if(Equal(GetCrewCount(GetOwner()), 1))
    if(GetPlayerName(GetOwner()))
     Geldbeutel();
  return(_inherited(a,b,c));
}

private func Geldbeutel()
{
  if(ObjectCount(MORU))
    {
    if(FindBase(GetOwner())) LocalN("Money",CreateContents(GDBT,FindBase(GetOwner())))= GetWealth(GetOwner());
			else LocalN("Money",CreateObject(GDBT))= GetWealth(GetOwner());
    }
}

protected func Departure(pContainer,a,b,c)
{
  var helm= FindObject(THLM,nil,nil,nil,nil,nil,"Wear",this());
  if(helm) SetVisibility(VIS_All,helm);
  return(_inherited(pContainer,a,b,c));
}

protected func Entrance(pContainer,a,b,c)
{
  var helm= FindObject(THLM,nil,nil,nil,nil,nil,"Wear",this());
  if(helm) SetVisibility(VIS_None,helm);
  return(_inherited(pContainer,a,b,c));
}

public func Collection2(pObj,a,b,c,d,e,f,g,h)
{
  BagCheck(pObj);
  BagCheck2(pObj);
  BagCheck3(pObj);
  return(_inherited(pObj,a,b,c,d,e,f,g,h));
}

public func BagCheck(pObj)
{
  if(!AutoCollect) return(nil);
  var beutel;
  if(!(beutel= FindObject(BETL,nil,nil,nil,nil,nil,"Belongs",this()))) return(nil);
  if(!Equal(LocalN("User",beutel), this())) return(nil);
  if(!Equal(GetID(pObj), BETL))
   if(!Contained())
    ObjectCall(beutel,"Put2",pObj);
   else
    ScheduleCall(beutel,"Put2",3,nil,pObj);
}

public func BagCheck2(pObj)
{
  if(!AutoCollect) return(nil);
  var sack;
  if(!(sack= FindObject(SAK1,nil,nil,nil,nil,nil,"Belongs",this()))) return(nil);
  if(!Equal(LocalN("User",sack), this())) return(nil);
  if(!Equal(GetID(pObj), SAK1))
   if(!Contained())
    ObjectCall(sack,"Put2",pObj);
   else
    ScheduleCall(sack,"Put2",3,nil,pObj);
}

public func BagCheck3(pObj)
{
  if(!AutoCollect) return(nil);
  var kiste;
  if(!(kiste= FindObject(KI_T,nil,nil,nil,nil,nil,"Belongs",this()))) return(nil);
  if(!Equal(LocalN("User",kiste), this())) return(nil);
  if(!Equal(GetID(pObj), KI_T))
   if(!Contained())
    ObjectCall(kiste,"Put2",pObj);
   else
    ScheduleCall(kiste,"Put2",3,nil,pObj);
}

protected func Incineration(iByPlayer,a,b,c)
{
  DeathCheck();
  return(_inherited(iByPlayer,a,b,c));
}

private func DeathCheck()
{
  if(!GetAlive())
    {
    SetAlive(1);
    DoEnergy((+(GetPhysical("Energy",1) ?? 0) ?? 0)/ 1000);
    Incinerate();
    SetClrModulation(RGB(240,240,240));
    DeadBurning(240);
    }
  if(OnFire()) return(ScheduleCall(this(),"DeathCheck",5));
}

private func DeadBurning(iRGB)
{
  if(!OnFire())
    {
    SetAlive();
    DoEnergy(-(GetEnergy() ?? 0));
    return;
    }
  DoEnergy((+(GetPhysical("Energy",1) ?? 0) ?? 0)/ 1000);
  if(Random(2)) DoCon(-2);
  if((iRGB ?? 0)>= 130) SetClrModulation(RGB((iRGB ?? 0)-10,(iRGB ?? 0)-10,(iRGB ?? 0)-10));
  if(this()) return(ScheduleCall(this(),"DeadBurning",50,nil,(iRGB ?? 0)-10));
}

public func IncludeClonk() { return(1); }

////////////////////////////////////////////////////////////////////////////////
/* Gebäude-Abriss */

public func ContextDestruction()
{
  [Abreißen|Image=SYF1|Condition=BackAnything]
  var obj = BackAnything();
  if (!obj) return(1);
  CreateMenu(SYF1,nil,nil,nil,"Was soll abgerissen werden?",nil,1);
for (var obj in FindObjects(Find_Not(Find_OCF(OCF_OnFire)),Find_Or(Find_Func("IsBasement"),Find_Func("GetDefCoreVal", "Construction", "DefCore"),Find_Func("IsShield")), Find_AtRect(-5,-5,10,20), Find_Owner(GetOwner())))
if (!FindObject(DSTR,nil,nil,nil,nil,nil,"Destruct",obj))
{{
  AddMenuItem("%s abreißen","DoDestruction",GetID(obj),nil,nil,obj);}}
  AddMenuItem("Zurück","NoDestruction",SYCC);
}

private func BackBuilding()
{
  var obj;
for (var obj in FindObjects(Find_Not(Find_OCF(OCF_OnFire)),Find_AtRect(-5,-5,10,20), Find_Owner(GetOwner()),Find_Func("GetDefCoreVal", "Construction", "DefCore")))
//    if (GetCategory(obj) & C4D_Structure())
//      if (GetCon(obj) >= 100)
//      if (GetDefCoreVal("Construction", "DefCore",GetID(obj)) != 0)
//      if (!FindObject(DSTR,0,0,0,0,0,"Destruct",obj))
          return(obj);
  return(nil);
}


//private func BackConstruct()
//{
//  var obj;
//for (var obj in FindObjects(Find_Distance(60),Find_OCF(OCF_Construct)))
//   if (GetCategory(obj) & C4D_Structure())
//      if (!FindObject(DSTR,0,0,0,0,0,"Destruct",obj))
//        return(obj);
//  obj = 0;
//  while (obj = FindObjectOwner(0,GetOwner(), -1,+9,0,0, OCF_Construct(), 0,0,obj))
//    if (GetCategory(obj) & C4D_Structure())
//      if (!FindObject(DSTR,0,0,0,0,0,"Destruct",obj))
//        return(obj);
//  return(0);
//}

private func BackAnything()
{
  var obj = BackBuilding();
  if (obj) return(obj);
  return(nil);
}

private func DoDestruction(par0, target)
{
  if(target) CreateObject(DSTR)->DoDestruction(target);
}

private func NoDestruction() {}
/////////////////////////////////////////////////////////////////////////////////////