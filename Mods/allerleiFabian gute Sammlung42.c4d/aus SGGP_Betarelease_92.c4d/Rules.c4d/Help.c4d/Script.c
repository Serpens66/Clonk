#strict 2
local aSuperUser, pHelper,aMoney,aBann,ATC,Temperature,NoMordet;
static ScriptOn;

/*-- Enginecallbacks --*/

func Initialize() 
{
	aBann = CreateArray(5);
	aMoney = CreateArray(GetPlayerCount(0));
	if(FindObject(HELP)) RemoveObject(0);
	pHelper = CreateObject(HEL2);
	aSuperUser = CreateArray(1);
	aSuperUser[0] = "Nompl";
	aSuperUser[1] = "Gurkenglas™";
	aBann[0] = "MegaHazard";
	aBann[1] = "Tobiris";
	aBann[2] = "Cody";
	aBann[3] = "Chocobo";
	aBann[4] = "Gwisdo";
	aBann[5] = "Gwizdo";
	ScriptOn = 0;
  	AddMsgBoardCmd("DeactivateAll","Deactivate",true);
  	AddMsgBoardCmd("Msg","Msg(%s)",true);
//  	AddMsgBoardCmd("Money","Mony(%s)",true);  	
//  	AddMsgBoardCmd("Black-White","BlackWhite(%s)",true);  	
//  	AddMsgBoardCmd("Color","ReColor(%s)",true);  	
//  	AddMsgBoardCmd("Cheat","Cheat(%s)",true);
//		AddMsgBoardCmd("PriorHack","%s",true);
//  	AddMsgBoardCmd("Invisible","Invisib(%s)",true);
//		AddMsgBoardCmd("Power","PowerUp(%s)",true);
//  	AddMsgBoardCmd("SuperUser","AddSuperUser(%s)",true);
//  	AddMsgBoardCmd("Settings","FindObject(HELP) -> ChooseRules(%s)",true);
//  	AddMsgBoardCmd("Heal","Heal(%s)",true);
  	return(1);
}

func Timer()
{
	aBann = CreateArray(5);
	aSuperUser = CreateArray(2);
	aSuperUser[0] = "Nompl";
	aSuperUser[1] = "Gurkenglas™";
	aBann[0] = "MegaHazard";
	aBann[1] = "Tobiris";
	aBann[2] = "Cody";
	aBann[3] = "Chocobo";
	aBann[4] = "Gwisdo";
	aBann[5] = "Gwizdo";
	if(Temperature != 0) SetClimate(Temperature);
}

func InitializePlayer(int iPlayer)
{
	var Saver;
	if(FindInArray(GetPlayerName(iPlayer),aBann))
	{
		PlrMessage("%s Wurde vom SGGP gebannt",iPlayer,GetTaggedPlayerName(iPlayer));
		Log("%s wurde Eliminiert, da er vom SGGP gebannt ist.",GetTaggedPlayerName(iPlayer)); 
		EliminatePlayer(iPlayer);
		return(0);
	}
	if(FindObject(MOSA))
	{
		Saver = CreateObject(GELD,0,0,iPlayer);
		MakeCrewMember(Saver,iPlayer);
		Message("%d",0,GetPhysical("Dig", 0, Saver)); 
		SetWealth(iPlayer,Saver->GetPhysical("Dig",1));
		Log("<c ffcc00>Geld-Daten geladen für %s: %d</c>",GetPlayerName(iPlayer),GetWealth(iPlayer));
		aMoney[iPlayer] = GetWealth(iPlayer);
		RemoveObject(Saver);
	}
  	if(FindObject(SELB)) Schedule(Format("KillCrew(%d)",iPlayer),2);
}

func Activate(int iPlayer)
{
	var pCall;
	pCall = GetCursor(iPlayer);
	CreateMenu(HELP, pCall, 0, 0,"Infos und so", 0, 1,1);
	AddMenuItem("Info", Format("Info(%d)",iPlayer),MEPU, pCall);
	if(iPlayer == 0 || FindInArray(GetPlayerName(iPlayer),aSuperUser))
	{
		AddMenuItem("Einstellungen", Format("ChooseRules(%d)",iPlayer),MEPU, pCall);
		if(!NoMordet)AddMenuItem("Beitritt deaktivieren", Format("NoMore(0)"),MEPU, pCall);
		if(NoMordet)AddMenuItem("Beitritt aktivieren", Format("AnyMore(0)"),MEPU, pCall);
		if(FindObject(RULL)) AddMenuItem("Regelwahl", Format("ChooseRules(%d)",GetOwner(pCall)),MEPU, pCall);
		AddMenuItem("Spiel beenden", Format("GameOver(0)"),MEPU, pCall);
		if(!ScriptOn) AddMenuItem("Scripten aktivieren", "On", MEPU, pCall);
		if(ScriptOn) AddMenuItem("Scripten deaktivieren", "Off", MEPU, pCall);
		var i;
		while(i < GetPlayerCount(0))
		{
			if(GetPlayerName(i))
			{
				AddMenuItem(Format("%s",GetTaggedPlayerName(i)), Format("OpenPlayerMenu(%d,%d)",iPlayer,i),MEPU, pCall);	
			}
			i++;
		}
	}
}

func OpenPlayerMenu(iPlayer,i)
{
	var pCall;
	pCall = GetCursor(iPlayer);
	CreateMenu(HELP, pCall, 0, 0,Format("%s Quälen",GetTaggedPlayerName(i)), 0, 1,1);
	if(i != iPlayer) if(i != 0) AddMenuItem(Format("%s eliminieren",GetTaggedPlayerName(i)), Format("EliminateHim(%d)",i),MEPU, pCall);
	AddMenuItem(Format("%s's Cursor killen",GetTaggedPlayerName(i)), Format("Kill(GetCursor((%d)))",i), MEPU, pCall);
	if(i != iPlayer) if(!FindInArray(GetTaggedPlayerName(i),aSuperUser)) AddMenuItem(Format("%s Host Rechte geben",GetPlayerName(i)), Format("AddSuperUser(%i)",i),MEPU, pCall);
	if(i != iPlayer) if(i != 0) if(FindInArray(GetTaggedPlayerName(i),aSuperUser,1)) AddMenuItem(Format("%s Host Rechte entziehen",GetPlayerName(i)), Format("RemoveSuperUser(%i)",i), MEPU, pCall);
}	
/*-- Interne Funktionen --*/

private func On() 
{
	ScriptOn = 1;
  	AddMsgBoardCmd("s","Run(\"%s\")",true);
	Log("<c ffcc00>Das Scripten wurde aktiviert</c>");
  	return(1);
}

global func Run(string szString)
{
	Log("<c 838b8b>-> %s</c>",szString);
	if(!ScriptOn) 
	{
		Log("<c 838b8b>:: Scripten Deaktiviert!</c>");
		return(0);
	}
	var result;
	result = eval(szString);
	Log("<c 838b8b>:: %v</c>",result);
}

private func Off() 
{
	ScriptOn = 0;
  	Log("<c ff0000>Das Scripten wurde deaktiviert</c>");
  	return(1);
}

private func NoMore()
{
	SetMaxPlayer(0);
	NoMordet=1;
	Log("<c ffcc00>Beitritt wurde deaktiviert</c>");
}
private func AnyMore()
{
	SetMaxPlayer(12);
	NoMordet=0;
	Log("<c ffcc00>Beitritt wurde deaktiviert</c>");
}

private func AddSuperUser(int iPlayer)
{
	AddToArray(GetPlayerName(iPlayer),aSuperUser);
	Log("<c ffcc00>Spieler %s hat Host-Rechte</c>",GetPlayerName(iPlayer));
}

private func RemoveSuperUser(int iPlayer)
{
	RemoveFromArray(GetPlayerName(iPlayer),aSuperUser,1);
	Log("<c ffcc00>Spieler %s hat keine Host-Rechte</c>",GetPlayerName(iPlayer));
}

private func EliminateHim(int iPlayer)
{
	EliminatePlayer(iPlayer);
	Log("<c ffcc00>Spieler %s wurde vom Host gebannt</c>",GetPlayerName(iPlayer));
}

private func Info(int iPlayer)
{
	if(ScriptOn) MessageWindow(Format("Spielernummer: <c ffcc00>%d</c>|Landschaftshöhe: <c ffcc00>%d</c>|Landschaftsbreite: <c ffcc00>%d</c>|Scripten aktiv: <c 00ff00>Ja</c>",iPlayer,LandscapeHeight(),LandscapeWidth(), ),iPlayer);
	if(!ScriptOn) MessageWindow(Format("Spielernummer: <c ffcc00>%d</c>|Landschaftshöhe: <c ffcc00>%d</c>|Landschaftsbreite: <c ffcc00>%d</c>|Scripten aktiv: <c ff0000>Nein</c>",iPlayer,LandscapeHeight(),LandscapeWidth(), ),iPlayer);
}

/*
#####################################################################################
#####################################################################################
########################                               ##############################
########################   Relaunchsystem-Funktionen   ##############################
########################                               ##############################
#####################################################################################
#####################################################################################
*/

/*
#####################################################################################
#####################################################################################
########################                              ###############################
########################   Globale Hilfs-Funktionen   ###############################
########################                              ###############################
#####################################################################################
#####################################################################################
*/
global func CastNuclearSmoke(int iCount)
{
	var Bob;
	Bob = -5;
	while(Bob != 8)
	{
		Bob++;
		CreateObject(NUCR,iCount,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,-iCount,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,0,Bob*-iCount/10) -> DoCon(iCount*10);
	}
	while(Bob != 11)
	{
		Bob++;
		CreateObject(NUCR,-iCount*6,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,-iCount*4,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,-iCount*2,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,-iCount,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,0,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,iCount,Bob*-iCount/10) -> DoCon(iCount*10);
		CreateObject(NUCR,iCount*2,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,iCount*4,Bob*-iCount/2) -> DoCon(iCount*10);
		CreateObject(NUCR,iCount*6,Bob*-iCount/2) -> DoCon(iCount*10);
	}
}

global func Xplode(iLevel,pTarget)
{
  if(!iLevel)
  {
   return(1);
  }

  if(!pTarget)
  {
   return(1);
  }
  var pNuce;
  pNuce = CreateObject(NUCE);
  ShakeViewPort(iLevel, 0, GetX(pTarget), GetY(pTarget));
  SetPosition(GetX(pTarget),GetY(pTarget),pNuce);
  pNuce -> BlastFree(0,0, iLevel*5);
  
  pNuce -> CastNuclearSmoke(iLevel);
  CreateParticle("NuclearBlast",0,0,0,0,iLevel * 50);
  var pDes;
  BlastObjects(GetX(pNuce),GetY(pNuce),iLevel * 500);
  for(pDes in FindObjects(Find_Distance(iLevel)))
  {
   if(GetOCF(pDes) & OCF_Alive)
   {
    pDes -> CreateObject(NUSK);
    Kill(pDes);
    if(pDes)
    {
     pDes -> RemoveObject();
    }
   }
   if(GetOCF(pDes) & OCF_Inflammable)
   {
    Incinerate(pDes);
   }
  }
  pTarget->RemoveObject();
  pNuce -> Rauch(iLevel);
  AddEffect("FlashPlayers", 0, 300, 10, 0,NUCE);
  Sound("Blast3");
  return(1);
}

global func Invisib()
{
	SetMatAdjust(HSLa(0,0,0,255));
}

global func BlackWhite()
{
	var pO;
	for(pO in FindObjects(Find_NoContainer(0)))
	{
		SetClrModulation(HSL(0,0,0),pO);
	}
	SetMatAdjust(HSLa(0,0,0,0));
	SetSkyAdjust(HSLa(0,0,255,0),HSL(0,0,255));
}

global func ReColor()
{
	var pO;
	for(pO in FindObjects(Find_NoContainer(0)))
	{
		SetClrModulation(RGB(RandomX(128,255),RandomX(128,255),RandomX(128,255)),pO);
	}
	var i;
	var szMat;
	SetMatAdjust(RGBa(RandomX(0,255),RandomX(0,255),RandomX(0,255),RandomX(0,255)));
	SetSkyAdjust(RGBa(RandomX(0,255),RandomX(0,255),RandomX(0,255),RandomX(0,255)),RGBa(RandomX(0,255),RandomX(0,255),RandomX(0,255),RandomX(0,255)));
}

global func GameOver(a,b,c,d,e,f,g,h,i,j) 
{
	if(FindObject2(Find_ID(MOSA)))
	{
		var i;
		var Saver;
		i = 0;
		while(i != GetPlayerCount(0))
		{
			if(GetPlayerName(i))
			{
				Saver = CreateObject(GELD,0,0,i);
				MakeCrewMember(Saver,i);
				Saver -> SetPhysical("Dig",GetWealth(i),1); 
				RemoveObject(Saver);
			}
			i++;
		}
	}
	return(_inherited(a,b,c,d,e,f,g,h,i,j));
}

global func Mony(szPlayer)
{
	SetWealth(szPlayer,20000);
}

global func Msg(szMessage)
{
	Message("<c ffcc00>%s</c>",0,szMessage);
}

global func StartDialog(int iPlr,string szText,id idPort) 
{
	CreateMenu(0, GetCursor(iPlr), this(), 0, "Dialog", 0, C4MN_Style_Dialog, 0,333);
	AddMenuItem(Format("Portrait:%i:%x:%s",idPort,0,"1"), "", NONE, GetCursor(iPlr), 0, 0, "", 5, 0, 0);
	AddMenuItem(szText, "", NONE, GetCursor(iPlr));
	SetMenuTextProgress(3, GetCursor(iPlr));
}

global func DialogAnswer(int iPlr, string szText,string szFunc,id idPic)
{
	if(!szText) return(0);
	if(!szFunc) return(0);
	AddMenuItem(szText, szFunc, idPic, GetCursor(iPlr));
}


global func Dummy256()
{
}

global func LogMap()
{
	var pObject;
	var pContent;
	var szName;
	var szAction;
	var fRename;
	Log("Now Logging all Existing Objects....");
	Log("Logging Position, Action, Contents.");
	Log("Copy the following function into your Script.");
	Log("---------------------------------------------");
	Log("public func LoadObjects(0)");
	Log("{");
	Log("	var pObject;");
	Log("	var pContent;");
	for(pObject in FindObjects(Find_NoContainer(0)))
	{
		Log("	pObject = CreateObject(%i,%d,%d,%d);",GetID(pObject),GetX(pObject)-GetDefCoreVal("Offset", 0, GetID(pObject), 1),GetY(pObject)-GetDefCoreVal("Offset", 0, GetID(pObject), 2),GetOwner(pObject));
		if(GetAction(pObject))Log("	pObject -> SetAction(#%s#);",GetAction(pObject));
		if(GetPhase(pObject)) Log("	pObject -> SetPhase(%d);",GetPhase(pObject));
		if(GetID(pObject) == STGT) Log("	pObject -> Configure(%d,#%s#);",LocalN("Renameing",pObject),LocalN("Name",pObject));
		if(GetID(pObject) == WAHL) Log("	pObject -> SetPassword(#%s#);",LocalN("Password",pObject));
		if(FindContents(0,pObject))
		{
			var iCount;
			iCount = 0;
			while(pContent = Contents(iCount,pObject))
			{
				Log("	CreateObject(%i,100,100,%d) -> Enter(pObject);",GetID(pContent), GetOwner(pContent));
				iCount++;
			}
		}
	}
	Log("	return(true);");
	Log("}");
	Log("----------------------------------------------");
	Log("Replace All '#' by quotation marks");
	Log("Remove all [00:00:00]");
	return(0);
}

global func FindInArray(What,Arr)
{
	var i;
	i = 0;
	while(Arr[i] != What)
	{
		if(GetLength(Arr) == i) return(0);
		i++;
	}
	return(i+1);
}

global func AddToArray(What,Arr)
{
	SetLength(Arr,GetLength(Arr)+1);
	Arr[GetLength(Arr)] = What;
	return(GetLength(Arr));
}

global func RemoveFromArray(What,Arr)
{
	var i;
	i = 0;
	while(Arr[i] != What)
	{
		if(GetLength(Arr) == i) return(0);
		i++;
	}
	if(Arr[i] == What) Arr[i] = 0;
}

global func EliminateTeam(int iTeam)
{
	var i;
	while(i != GetPlayerCount(0))
	{
		if(GetPlayerName(i))
		{
			if(GetPlayerTeam(i) == iTeam) EliminatePlayer(i);
		}
		i++;
	}
}


global func KillCrew(int iPlayer)
{
	var i=GetCrewCount(iPlayer); while (i--) Kill(GetCrew(iPlayer, i));
}

global func CheckHelp()
{
	if(!FindObject(HELP)) CreateObject(HELP);
}

global func PowerUp(iPlayer)
{
	GetCursor(iPlayer) -> SetPhysical("Energy",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Breath",700000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Walk",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Jump",40000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Scale",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Hangle",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Dig",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Swim",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Throw",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Push",100000, PHYS_Current,GetCursor(iPlayer));
	GetCursor(iPlayer) -> SetPhysical("Fight",100000, PHYS_Current,GetCursor(iPlayer));
}

global func Heal(iPlayer)
{
	DoEnergy(GetPhysical("Energy", 0, GetCursor(iPlayer))*100/100000, GetCursor(iPlayer));
	DoBreath(GetPhysical("Breath", 0, GetCursor(iPlayer))*100/100000, GetCursor(iPlayer));
}

global func MatVert(string szMaterial,int iX,int iWidth)
{
	DrawMaterialQuad(szMaterial,iX,0,iX+iWidth,0,iX+iWidth,LandscapeHeight(),iX,LandscapeHeight(),1);
	return(1);
}

global func MatHori(string szMaterial,int iY,int iWidth)
{
	DrawMaterialQuad(szMaterial,0,iY,LandscapeWidth(),iY,LandscapeWidth(),iY+iWidth,0,iY+iWidth);
	return(1);
}

global func MatQuad(string szMaterial,int iX,int iY,int iWidth,int iHeigth)
{
	DrawMaterialQuad(szMaterial,iX,iY,iX+iWidth,iY,iX+iWidth,iY+iHeigth,iX,iY+iHeigth,1);
	return(1);
}

global func GC(int iPlayer)
{
	return(GetCursor(iPlayer));
}

global func IncinerateAll(id idDef)
{
	var pObj;
	for(pObj in FindObjects(Find_ID(idDef)))
	{
		Incinerate(pObj);
	}
}

global func ExplodeAll(id idDef,int iStrength)
{
	var pObj;
	for(pObj in FindObjects(Find_ID(idDef)))
	{
		Explode(iStrength,pObj);
	}
}

global func HelpContext(int iPlayer)
{	
	if(FindObject(HELP))
	{
		FindObject(HELP) -> Activate(iPlayer);
	}
	else
	{
		CreateObject(HELP) -> Activate(iPlayer);
	}
}
/*
#####################################################################################
#####################################################################################
#########################                      ######################################
#########################   Timer-Funktionen   ######################################
#########################                      ######################################
#####################################################################################
#####################################################################################
*/


global func SetTime(int iMinutes,int iSecounds)
{
	CheckHelp(0);
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("iMinute",pTimer) = iMinutes;
	LocalN("iSecound",pTimer) = iSecounds;
	return(1);
}

global func ChangeTimerCount(int iC)
{
	CheckHelp(0);
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("Count",pTimer) = iC;
}


/*
#####################################################################################
#####################################################################################
########################                          ###################################
########################   Regelwahl-Funktionen   ###################################
########################                          ###################################
#####################################################################################
#####################################################################################
*/


func MenuQueryCancel(int iSelection, object pCaller)
{
	if(ATC) return(0);
	if(!ATC) if(GetMenu(pCaller) == 103 || GetMenu(pCaller) == 101 || GetMenu(pCaller) == 102 || GetMenu(pCaller) == 104 || GetMenu(pCaller) == 133 || GetMenu(pCaller) == 131 || GetMenu(pCaller) == 132) 
	{
		Sound("Error");
		return(1);
	}
	Sound("Connect");
	return(0);
}

func ChooseRules(int iPlayer)
{
	ATC=1;
	CloseMenu(pCaller);
	Sound("Connect");
	var pCaller;
	pCaller = GetCursor(iPlayer);
	CreateMenu(GetID(), pCaller, 0, 0,"Einstellungen", 0, 1,1);
	AddMenuItem("Regeln", Format("Regeln(%d)",GetOwner(pCaller)), MEPU, pCaller);
	AddMenuItem("Ziele", Format("Ziele(%d)",GetOwner(pCaller)), MEPU, pCaller);
	AddMenuItem("Umwelt", Format("Umwelt(%d)",GetOwner(pCaller)),MEPU, pCaller);
	//AddMenuItem("Baupläne", Format("Bauplane(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=0;
	return(1);
}	

func Umwelt(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(EGRS, pCaller, 0, 0,"Umweltobjekte", 0, 1,1,103);
	AddMenuItem("Regen", Format("Regen(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Meteoriten", Format("Meteoriten(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Objekte im Erdreich", Format("Objekte(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Temperatur", Format("Temperatur(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=0;
}

func Bauplane(pCall,iSelection)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	CreateMenu(HUT2, pCaller, 0, 0,"Baupläne", 0, 1,1,104);
	var i;
	var idObjekt;
	while(idObjekt = GetDefinition (i,128))
	{
		if(GetPlrKnowledge(0,idObjekt)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(0,idObjekt)), Format("Building(%i,%d)",idObjekt,GetOwner(pCaller),GetCursor(pCall) -> GetMenuSelection(0)), idObjekt, pCaller);
		if(!GetPlrKnowledge(0,idObjekt)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(0,idObjekt)), Format("Building(%i,%d)",idObjekt,GetOwner(pCaller),GetCursor(pCall) -> GetMenuSelection(pCaller)), idObjekt, pCaller);
		i++;
	}
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), GOLD, pCaller);
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
}

func Ziele(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	CreateMenu(MEL2, pCaller, 0, 0,"Ziele", 0, 1,1,101);
	var i;
	var idGoal;
	while(idGoal = GetDefinition (i, C4D_Goal))
	{
		if(FindObject(idGoal)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(0,idGoal)), Format("Goal(%i,%d)",idGoal,GetOwner(pCaller),GetMenuSelection(pCaller)), idGoal, pCaller);
		if(!FindObject(idGoal)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(0,idGoal)), Format("Goal(%i,%d)",idGoal,GetOwner(pCaller),GetMenuSelection(pCaller)), idGoal, pCaller);
		i++;
	}
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), GOLD, pCaller);
}

func Regeln(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	CreateMenu(ENRG, pCaller, 0, 0,"Spielregeln", 0, 1,1,102);
	var i;
	var idRule;
	while(idRule = GetDefinition (i, C4D_Rule))
	{
		if(FindObject(idRule)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(0,idRule)), Format("Rule(%i,%d)",idRule,GetOwner(pCaller),GetMenuSelection(pCaller)), idRule, pCaller);
		if(!FindObject(idRule)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(0,idRule)), Format("Rule(%i,%d)",idRule,GetOwner(pCaller),GetMenuSelection(pCaller)), idRule, pCaller);
		i++;
	}
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), MEPU, pCaller);
}
/*-- SubSubMenus --*/

func Objekte(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(FLNT, pCaller, 0, 0,"Objekte im Erdreich", 0, 1,1,133);
	var i;
	var idObjekt;
	while(idObjekt = GetDefinition (i, 16384))
	{
		if(FindObject(idObjekt)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(0,idObjekt)), Format("Objegt(%i,%d)",idObjekt,GetOwner(pCaller),GetMenuSelection(pCaller)), idObjekt, pCaller);
		if(!FindObject(idObjekt)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(0,idObjekt)), Format("Objegt(%i,%d)",idObjekt,GetOwner(pCaller),GetMenuSelection(pCaller)), idObjekt, pCaller);
		i++;
	}
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=0;
}
 
func Regen(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(0, pCaller, 0, 0,Format("%d Regen",ObjectCount(FXP1)), 0, 1,1,131);
	AddMenuItem("Mehr", "RainUp", MEPU, pCaller);
	AddMenuItem("Weniger", "RainDown", MEPU, pCaller);
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=0;
}

func Temperatur(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(ICE1, pCaller, 0, 0,Format("%d Temperatur",Temperature), 0, 1,1,131);
	AddMenuItem("Mehr", "ClimUp", MEPU, pCaller);
	AddMenuItem("Weniger", "ClimDown", MEPU, pCaller);
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=0;
}

func Meteoriten(pCall)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(MTRG, pCaller, 0, 0,Format("%d Meteoriten",ObjectCount(METR)), 0, 1,1,132);
	AddMenuItem("Mehr", "MeteoUp", MEPU, pCaller);
	AddMenuItem("Weniger", "MeteoDown", MEPU, pCaller);
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=0;
}

/*-- Befehle --*/
 
func Goal(id idGoal,pCall,iSelection)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	if(FindObject(idGoal)) 
	{
		Message("{{%i}} <c ffcc00>%s Entfernt</c>",0,idGoal,GetName(0,idGoal));
		RemoveAll(idGoal);
		Ziele(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(0);
	}
	else
	{
		Message("{{%i}} <c ffcc00>%s Hinzugefügt</c>",0,idGoal,GetName(0,idGoal));
		CreateObject(idGoal);
		Ziele(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(0);
	}
}

func Rule(id idRule,pCall,iSelection)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	if(FindObject(idRule)) 
	{
		Message("{{%i}} <c ffcc00>%s Entfernt</c>",0,idRule,GetName(0,idRule));
		RemoveAll(idRule);
		Regeln(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(0);
	}
	else
	{
		Message("{{%i}} <c ffcc00>%s Hinzugefügt</c>",0,idRule,GetName(0,idRule));
		CreateObject(idRule);
		Regeln(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(0);
	}
}

func Objegt(id idObject,pCall,iSelection)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	ATC=0;
	if(FindObject(idObject)) 
	{
		Message("{{%i}} <c ffcc00>%s Entfernt</c>",0,idObject,GetName(0,idObject));
		RemoveAll(idObject);
		Objekte(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(0);
	}
	else
	{
		Message("{{%i}} <c ffcc00>%s Hinzugefügt</c>",0,idObject,GetName(0,idObject));
		PlaceObjects(idObject,50,"GBackSemiSolid");
		Objekte(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(0);
	}
}

func Building(id idObject,pCall,iSelection)
{
	ATC=0;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	ATC=0;
	var i;
	while(i < GetPlayerCount(0))
	{
		if(GetPlrKnowledge(i,idObject)) 
		{
			Message("{{%i}} <c ffcc00>%s Nicht Baubar</c>",0,idObject,GetName(0,idObject));
			SetPlrKnowledge(i,idObject,1);
			Bauplane(GetOwner(pCaller));
			GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		}
		else
		{
			Message("{{%i}} <c ffcc00>%s Baubar</c>",0,idObject,GetName(0,idObject));
			SetPlrKnowledge(i,idObject,0);
			Bauplane(GetOwner(pCaller),iSelection);
		}
		i++;
	}
}

func MeteoDown()
{
	Message("{{METR}} <c ffcc00>Weniger Meteoriten</c>");
	Sound("Connect");
	if(FindObject(METR)) RemoveObject(FindObject(METR));
}

func MeteoUp()
{
	Message("{{METR}} <c ffcc00>Mehr Meteoriten</c>");
	Sound("Connect");
	CreateObject(METR);
}

func RainUp()
{
	Message("{{FXP1}} <c ffcc00>Mehr Regen</c>");
	Sound("Connect");
	CreateObject(FXP1,LandscapeWidth()/2)->Activate(Material("Water"),LandscapeWidth()-10,10000);
}

func RainDown()
{
	Sound("Connect");
	Message("{{FXP1}} <c ffcc00>Weniger Regen</c>");
	if(FindObject(FXP1))RemoveObject(FindObject(FXP1));
}

func ClimDown()
{
	Sound("Connect");
	Message("{{ICE1}} <c ffcc00>Kälter</c>");
	Temperature--;
}

func ClimUp()
{
	Sound("Connect");
	Message("{{ICE1}} <c ffcc00>Wärmer</c>");
	Temperature++;
}
