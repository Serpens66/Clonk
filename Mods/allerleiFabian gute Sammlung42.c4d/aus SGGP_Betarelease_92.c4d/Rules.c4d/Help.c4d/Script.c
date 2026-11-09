#strict 3
local aSuperUser, pHelper,aMoney,aBann,ATC,Temperature,NoMordet;
static ScriptOn;

/*-- Enginecallbacks --*/

func Initialize() 
{
	aBann = CreateArray(5);
	aMoney = CreateArray(GetPlayerCount(nil));
	if(FindObject(HELP)) RemoveObject(nil);
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
	ScriptOn = nil;
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
	if(!S42MigrationEqual(Temperature, nil)) SetClimate(Temperature);
}

func InitializePlayer(int iPlayer)
{
	iPlayer ??= 0;
	var Saver;
	if(FindInArray(GetPlayerName(iPlayer),aBann))
	{
		PlrMessage("%s Wurde vom SGGP gebannt",iPlayer,GetTaggedPlayerName(iPlayer));
		Log("%s wurde Eliminiert, da er vom SGGP gebannt ist.",GetTaggedPlayerName(iPlayer)); 
		EliminatePlayer(iPlayer);
		return(nil);
	}
	if(FindObject(MOSA))
	{
		Saver = CreateObject(GELD,nil,nil,iPlayer);
		MakeCrewMember(Saver,iPlayer);
		Message("%d",nil,GetPhysical("Dig", nil, Saver));
		SetWealth(iPlayer,Saver->GetPhysical("Dig",1));
		Log("<c ffcc00>Geld-Daten geladen für %s: %d</c>",GetPlayerName(iPlayer),GetWealth(iPlayer));
		aMoney[(iPlayer ?? 0)] = GetWealth(iPlayer);
		RemoveObject(Saver);
	}
  	if(FindObject(SELB)) Schedule(Format("KillCrew(%d)",iPlayer),2);
}

func Activate(int iPlayer)
{
	iPlayer ??= 0;
	var pCall;
	pCall = GetCursor(iPlayer);
	CreateMenu(HELP, pCall, nil, nil,"Infos und so", nil, 1,1);
	AddMenuItem("Info", Format("Info(%d)",iPlayer),MEPU, pCall);
	if(S42MigrationEqual(iPlayer, nil) || FindInArray(GetPlayerName(iPlayer),aSuperUser))
	{
		AddMenuItem("Einstellungen", Format("ChooseRules(%d)",iPlayer),MEPU, pCall);
		if(!NoMordet)AddMenuItem("Beitritt deaktivieren", Format("NoMore(0)"),MEPU, pCall);
		if(NoMordet)AddMenuItem("Beitritt aktivieren", Format("AnyMore(0)"),MEPU, pCall);
		if(FindObject(RULL)) AddMenuItem("Regelwahl", Format("ChooseRules(%d)",GetOwner(pCall)),MEPU, pCall);
		AddMenuItem("Spiel beenden", Format("GameOver(0)"),MEPU, pCall);
		if(!ScriptOn) AddMenuItem("Scripten aktivieren", "On", MEPU, pCall);
		if(ScriptOn) AddMenuItem("Scripten deaktivieren", "Off", MEPU, pCall);
		var i;
		while((i ?? 0) < (GetPlayerCount(nil) ?? 0))
		{
			if(GetPlayerName(i))
			{
				AddMenuItem(Format("%s",GetTaggedPlayerName(i)), Format("OpenPlayerMenu(%d,%d)",iPlayer,i),MEPU, pCall);	
			}
			S42MigrationStep(i, 1, true);
		}
	}
}

func OpenPlayerMenu(iPlayer,i)
{
	var pCall;
	pCall = GetCursor(iPlayer);
	CreateMenu(HELP, pCall, nil, nil,Format("%s Quälen",GetTaggedPlayerName(i)), nil, 1,1);
	if(!S42MigrationEqual(i, iPlayer)) if(!S42MigrationEqual(i, nil)) AddMenuItem(Format("%s eliminieren",GetTaggedPlayerName(i)), Format("EliminateHim(%d)",i),MEPU, pCall);
	AddMenuItem(Format("%s's Cursor killen",GetTaggedPlayerName(i)), Format("Kill(GetCursor((%d)))",i), MEPU, pCall);
	if(!S42MigrationEqual(i, iPlayer)) if(!FindInArray(GetTaggedPlayerName(i),aSuperUser)) AddMenuItem(Format("%s Host Rechte geben",GetPlayerName(i)), Format("AddSuperUser(%i)",i),MEPU, pCall);
	if(!S42MigrationEqual(i, iPlayer)) if(!S42MigrationEqual(i, nil)) if(FindInArray(GetTaggedPlayerName(i),aSuperUser,1)) AddMenuItem(Format("%s Host Rechte entziehen",GetPlayerName(i)), Format("RemoveSuperUser(%i)",i), MEPU, pCall);
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
		return(nil);
	}
	var result;
	result = eval(szString);
	Log("<c 838b8b>:: %v</c>",result);
}

private func Off() 
{
	ScriptOn = nil;
  	Log("<c ff0000>Das Scripten wurde deaktiviert</c>");
  	return(1);
}

private func NoMore()
{
	SetMaxPlayer(nil);
	NoMordet=1;
	Log("<c ffcc00>Beitritt wurde deaktiviert</c>");
}
private func AnyMore()
{
	SetMaxPlayer(12);
	NoMordet=nil;
	Log("<c ffcc00>Beitritt wurde deaktiviert</c>");
}

private func AddSuperUser(int iPlayer)
{
	iPlayer ??= 0;
	AddToArray(GetPlayerName(iPlayer),aSuperUser);
	Log("<c ffcc00>Spieler %s hat Host-Rechte</c>",GetPlayerName(iPlayer));
}

private func RemoveSuperUser(int iPlayer)
{
	iPlayer ??= 0;
	RemoveFromArray(GetPlayerName(iPlayer),aSuperUser,1);
	Log("<c ffcc00>Spieler %s hat keine Host-Rechte</c>",GetPlayerName(iPlayer));
}

private func EliminateHim(int iPlayer)
{
	iPlayer ??= 0;
	EliminatePlayer(iPlayer);
	Log("<c ffcc00>Spieler %s wurde vom Host gebannt</c>",GetPlayerName(iPlayer));
}

private func Info(int iPlayer)
{
	iPlayer ??= 0;
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
	iCount ??= 0;
	var Bob;
	Bob = -5;
	while(!S42MigrationEqual(Bob, 8))
	{
		S42MigrationStep(Bob, 1, true);
		CreateObject(NUCR,iCount,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,-(iCount ?? 0),(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,nil,(Bob ?? 0)*-(iCount ?? 0)/10) -> DoCon((iCount ?? 0)*10);
	}
	while(!S42MigrationEqual(Bob, 11))
	{
		S42MigrationStep(Bob, 1, true);
		CreateObject(NUCR,-(iCount ?? 0)*6,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,-(iCount ?? 0)*4,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,-(iCount ?? 0)*2,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,-(iCount ?? 0),(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,nil,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,iCount,(Bob ?? 0)*-(iCount ?? 0)/10) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,(iCount ?? 0)*2,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,(iCount ?? 0)*4,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
		CreateObject(NUCR,(iCount ?? 0)*6,(Bob ?? 0)*-(iCount ?? 0)/2) -> DoCon((iCount ?? 0)*10);
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
  ShakeViewPort(iLevel, nil, GetX(pTarget), GetY(pTarget));
  SetPosition(GetX(pTarget),GetY(pTarget),pNuce);
  pNuce -> BlastFree(nil,nil, (iLevel ?? 0)*5);
  
  pNuce -> CastNuclearSmoke(iLevel);
  CreateParticle("NuclearBlast",nil,nil,nil,nil,(iLevel ?? 0) * 50);
  var pDes;
  BlastObjects(GetX(pNuce),GetY(pNuce),(iLevel ?? 0) * 500);
  for(pDes in FindObjects(Find_Distance(iLevel)))
  {
   if((GetOCF(pDes) ?? 0) & (OCF_Alive ?? 0))
   {
    pDes -> CreateObject(NUSK);
    Kill(pDes);
    if(pDes)
    {
     pDes -> RemoveObject();
    }
   }
   if((GetOCF(pDes) ?? 0) & (OCF_Inflammable ?? 0))
   {
    Incinerate(pDes);
   }
  }
  pTarget->RemoveObject();
  pNuce -> Rauch(iLevel);
  AddEffect("FlashPlayers", nil, 300, 10, nil,NUCE);
  Sound("Blast3");
  return(1);
}

global func Invisib()
{
	SetMatAdjust(HSLa(nil,nil,nil,255));
}

global func BlackWhite()
{
	var pO;
	for(pO in FindObjects(Find_NoContainer(nil)))
	{
		SetClrModulation(HSL(nil,nil,nil),pO);
	}
	SetMatAdjust(HSLa(nil,nil,nil,nil));
	SetSkyAdjust(HSLa(nil,nil,255,nil),HSL(nil,nil,255));
}

global func ReColor()
{
	var pO;
	for(pO in FindObjects(Find_NoContainer(nil)))
	{
		SetClrModulation(RGB(RandomX(128,255),RandomX(128,255),RandomX(128,255)),pO);
	}
	var i;
	var szMat;
	SetMatAdjust(RGBa(RandomX(nil,255),RandomX(nil,255),RandomX(nil,255),RandomX(nil,255)));
	SetSkyAdjust(RGBa(RandomX(nil,255),RandomX(nil,255),RandomX(nil,255),RandomX(nil,255)),RGBa(RandomX(nil,255),RandomX(nil,255),RandomX(nil,255),RandomX(nil,255)));
}

global func GameOver(a,b,c,d,e,f,g,h,i,j) 
{
	if(FindObject2(Find_ID(MOSA)))
	{
		var i;
		var Saver;
		i = nil;
		while(!S42MigrationEqual(i, GetPlayerCount(nil)))
		{
			if(GetPlayerName(i))
			{
				Saver = CreateObject(GELD,nil,nil,i);
				MakeCrewMember(Saver,i);
				Saver -> SetPhysical("Dig",GetWealth(i),1); 
				RemoveObject(Saver);
			}
			S42MigrationStep(i, 1, true);
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
	Message("<c ffcc00>%s</c>",nil,szMessage);
}

global func StartDialog(int iPlr,string szText,id idPort) 
{
	iPlr ??= 0;
	CreateMenu(nil, GetCursor(iPlr), this(), nil, "Dialog", nil, C4MN_Style_Dialog, nil,333);
	AddMenuItem(Format("Portrait:%i:%x:%s",idPort,nil,"1"), "", NONE, GetCursor(iPlr), nil, nil, "", 5, nil, nil);
	AddMenuItem(szText, "", NONE, GetCursor(iPlr));
	SetMenuTextProgress(3, GetCursor(iPlr));
}

global func DialogAnswer(int iPlr, string szText,string szFunc,id idPic)
{
	iPlr ??= 0;
	if(!szText) return(nil);
	if(!szFunc) return(nil);
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
	for(pObject in FindObjects(Find_NoContainer(nil)))
	{
		Log("	pObject = CreateObject(%i,%d,%d,%d);",GetID(pObject),(GetX(pObject) ?? 0)-(GetDefCoreVal("Offset", nil, GetID(pObject), 1) ?? 0),(GetY(pObject) ?? 0)-(GetDefCoreVal("Offset", nil, GetID(pObject), 2) ?? 0),GetOwner(pObject));
		if(GetAction(pObject))Log("	pObject -> SetAction(#%s#);",GetAction(pObject));
		if(GetPhase(pObject)) Log("	pObject -> SetPhase(%d);",GetPhase(pObject));
		if(S42MigrationEqual(GetID(pObject), STGT)) Log("	pObject -> Configure(%d,#%s#);",LocalN("Renameing",pObject),LocalN("Name",pObject));
		if(S42MigrationEqual(GetID(pObject), WAHL)) Log("	pObject -> SetPassword(#%s#);",LocalN("Password",pObject));
		if(FindContents(nil,pObject))
		{
			var iCount;
			iCount = nil;
			while(pContent = Contents(iCount,pObject))
			{
				Log("	CreateObject(%i,100,100,%d) -> Enter(pObject);",GetID(pContent), GetOwner(pContent));
				S42MigrationStep(iCount, 1, true);
			}
		}
	}
	Log("	return(true);");
	Log("}");
	Log("----------------------------------------------");
	Log("Replace All '#' by quotation marks");
	Log("Remove all [00:00:00]");
	return(nil);
}

global func FindInArray(What,Arr)
{
	var i;
	i = nil;
	while(!S42MigrationEqual(Arr[(i ?? 0)], What))
	{
		if(S42MigrationEqual(GetLength(Arr), i)) return(nil);
		S42MigrationStep(i, 1, true);
	}
	return((i ?? 0)+1);
}

global func AddToArray(What,Arr)
{
	SetLength(Arr,(GetLength(Arr) ?? 0)+1);
	Arr[(GetLength(Arr) ?? 0)] = What;
	return(GetLength(Arr));
}

global func RemoveFromArray(What,Arr)
{
	var i;
	i = nil;
	while(!S42MigrationEqual(Arr[(i ?? 0)], What))
	{
		if(S42MigrationEqual(GetLength(Arr), i)) return(nil);
		S42MigrationStep(i, 1, true);
	}
	if(S42MigrationEqual(Arr[(i ?? 0)], What)) Arr[(i ?? 0)] = nil;
}

global func EliminateTeam(int iTeam)
{
	iTeam ??= 0;
	var i;
	while(!S42MigrationEqual(i, GetPlayerCount(nil)))
	{
		if(GetPlayerName(i))
		{
			if(S42MigrationEqual(GetPlayerTeam(i), iTeam)) EliminatePlayer(i);
		}
		S42MigrationStep(i, 1, true);
	}
}


global func KillCrew(int iPlayer)
{
	iPlayer ??= 0;
	var i=GetCrewCount(iPlayer); while (S42MigrationStep(i, -1, true)) Kill(GetCrew(iPlayer, i));
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
	DoEnergy((GetPhysical("Energy", nil, GetCursor(iPlayer)) ?? 0)*100/100000, GetCursor(iPlayer));
	DoBreath((GetPhysical("Breath", nil, GetCursor(iPlayer)) ?? 0)*100/100000, GetCursor(iPlayer));
}

global func MatVert(string szMaterial,int iX,int iWidth)
{
	iX ??= 0;
	iWidth ??= 0;
	DrawMaterialQuad(szMaterial,iX,nil,(iX ?? 0)+(iWidth ?? 0),nil,(iX ?? 0)+(iWidth ?? 0),LandscapeHeight(),iX,LandscapeHeight(),1);
	return(1);
}

global func MatHori(string szMaterial,int iY,int iWidth)
{
	iY ??= 0;
	iWidth ??= 0;
	DrawMaterialQuad(szMaterial,nil,iY,LandscapeWidth(),iY,LandscapeWidth(),(iY ?? 0)+(iWidth ?? 0),nil,(iY ?? 0)+(iWidth ?? 0));
	return(1);
}

global func MatQuad(string szMaterial,int iX,int iY,int iWidth,int iHeigth)
{
	iX ??= 0;
	iY ??= 0;
	iWidth ??= 0;
	iHeigth ??= 0;
	DrawMaterialQuad(szMaterial,iX,iY,(iX ?? 0)+(iWidth ?? 0),iY,(iX ?? 0)+(iWidth ?? 0),(iY ?? 0)+(iHeigth ?? 0),iX,(iY ?? 0)+(iHeigth ?? 0),1);
	return(1);
}

global func GC(int iPlayer)
{
	iPlayer ??= 0;
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
	iStrength ??= 0;
	var pObj;
	for(pObj in FindObjects(Find_ID(idDef)))
	{
		Explode(iStrength,pObj);
	}
}

global func HelpContext(int iPlayer)
{	
	iPlayer ??= 0;
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
	iMinutes ??= 0;
	iSecounds ??= 0;
	CheckHelp(nil);
	var pTimer;
	pTimer = FindObject(TIMR);
	if(!pTimer) pTimer = CreateObject(TIMR);
	LocalN("iMinute",pTimer) = iMinutes;
	LocalN("iSecound",pTimer) = iSecounds;
	return(1);
}

global func ChangeTimerCount(int iC)
{
	iC ??= 0;
	CheckHelp(nil);
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
	iSelection ??= 0;
	if(ATC) return(nil);
	if(!ATC) if(S42MigrationEqual(GetMenu(pCaller), 103) || S42MigrationEqual(GetMenu(pCaller), 101) || S42MigrationEqual(GetMenu(pCaller), 102) || S42MigrationEqual(GetMenu(pCaller), 104) || S42MigrationEqual(GetMenu(pCaller), 133) || S42MigrationEqual(GetMenu(pCaller), 131) || S42MigrationEqual(GetMenu(pCaller), 132))
	{
		Sound("Error");
		return(1);
	}
	Sound("Connect");
	return(nil);
}

func ChooseRules(int iPlayer)
{
	iPlayer ??= 0;
	ATC=1;
	CloseMenu(pCaller);
	Sound("Connect");
	var pCaller;
	pCaller = GetCursor(iPlayer);
	CreateMenu(GetID(), pCaller, nil, nil,"Einstellungen", nil, 1,1);
	AddMenuItem("Regeln", Format("Regeln(%d)",GetOwner(pCaller)), MEPU, pCaller);
	AddMenuItem("Ziele", Format("Ziele(%d)",GetOwner(pCaller)), MEPU, pCaller);
	AddMenuItem("Umwelt", Format("Umwelt(%d)",GetOwner(pCaller)),MEPU, pCaller);
	//AddMenuItem("Baupläne", Format("Bauplane(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=nil;
	return(1);
}	

func Umwelt(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(EGRS, pCaller, nil, nil,"Umweltobjekte", nil, 1,1,103);
	AddMenuItem("Regen", Format("Regen(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Meteoriten", Format("Meteoriten(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Objekte im Erdreich", Format("Objekte(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Temperatur", Format("Temperatur(%d)",GetOwner(pCaller),GetMenuSelection(pCaller)), MEPU, pCaller);
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=nil;
}

func Bauplane(pCall,iSelection)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	CreateMenu(HUT2, pCaller, nil, nil,"Baupläne", nil, 1,1,104);
	var i;
	var idObjekt;
	while(idObjekt = GetDefinition (i,128))
	{
		if(GetPlrKnowledge(nil,idObjekt)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(nil,idObjekt)), Format("Building(%i,%d)",idObjekt,GetOwner(pCaller),GetCursor(pCall) -> GetMenuSelection(nil)), idObjekt, pCaller);
		if(!GetPlrKnowledge(nil,idObjekt)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(nil,idObjekt)), Format("Building(%i,%d)",idObjekt,GetOwner(pCaller),GetCursor(pCall) -> GetMenuSelection(pCaller)), idObjekt, pCaller);
		S42MigrationStep(i, 1, true);
	}
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), GOLD, pCaller);
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
}

func Ziele(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	CreateMenu(MEL2, pCaller, nil, nil,"Ziele", nil, 1,1,101);
	var i;
	var idGoal;
	while(idGoal = GetDefinition (i, C4D_Goal))
	{
		if(FindObject(idGoal)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(nil,idGoal)), Format("Goal(%i,%d)",idGoal,GetOwner(pCaller),GetMenuSelection(pCaller)), idGoal, pCaller);
		if(!FindObject(idGoal)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(nil,idGoal)), Format("Goal(%i,%d)",idGoal,GetOwner(pCaller),GetMenuSelection(pCaller)), idGoal, pCaller);
		S42MigrationStep(i, 1, true);
	}
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), GOLD, pCaller);
}

func Regeln(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	CreateMenu(ENRG, pCaller, nil, nil,"Spielregeln", nil, 1,1,102);
	var i;
	var idRule;
	while(idRule = GetDefinition (i, C4D_Rule))
	{
		if(FindObject(idRule)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(nil,idRule)), Format("Rule(%i,%d)",idRule,GetOwner(pCaller),GetMenuSelection(pCaller)), idRule, pCaller);
		if(!FindObject(idRule)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(nil,idRule)), Format("Rule(%i,%d)",idRule,GetOwner(pCaller),GetMenuSelection(pCaller)), idRule, pCaller);
		S42MigrationStep(i, 1, true);
	}
	AddMenuItem("Fertig", Format("ChooseRules(%d)",GetOwner(pCaller)), MEPU, pCaller);
}
/*-- SubSubMenus --*/

func Objekte(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(FLNT, pCaller, nil, nil,"Objekte im Erdreich", nil, 1,1,133);
	var i;
	var idObjekt;
	while(idObjekt = GetDefinition (i, 16384))
	{
		if(FindObject(idObjekt)) AddMenuItem(Format("<c ff0000>%s löschen</c>",GetName(nil,idObjekt)), Format("Objegt(%i,%d)",idObjekt,GetOwner(pCaller),GetMenuSelection(pCaller)), idObjekt, pCaller);
		if(!FindObject(idObjekt)) AddMenuItem(Format("<c 00ff00>%s hinzufügen</c>",GetName(nil,idObjekt)), Format("Objegt(%i,%d)",idObjekt,GetOwner(pCaller),GetMenuSelection(pCaller)), idObjekt, pCaller);
		S42MigrationStep(i, 1, true);
	}
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=nil;
}
 
func Regen(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(nil, pCaller, nil, nil,Format("%d Regen",ObjectCount(FXP1)), nil, 1,1,131);
	AddMenuItem("Mehr", "RainUp", MEPU, pCaller);
	AddMenuItem("Weniger", "RainDown", MEPU, pCaller);
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=nil;
}

func Temperatur(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(ICE1, pCaller, nil, nil,Format("%d Temperatur",Temperature), nil, 1,1,131);
	AddMenuItem("Mehr", "ClimUp", MEPU, pCaller);
	AddMenuItem("Weniger", "ClimDown", MEPU, pCaller);
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=nil;
}

func Meteoriten(pCall)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	ATC=1;
	CloseMenu(pCaller);
	CreateMenu(MTRG, pCaller, nil, nil,Format("%d Meteoriten",ObjectCount(METR)), nil, 1,1,132);
	AddMenuItem("Mehr", "MeteoUp", MEPU, pCaller);
	AddMenuItem("Weniger", "MeteoDown", MEPU, pCaller);
	AddMenuItem("Fertig", Format("Umwelt(%d)",GetOwner(pCaller)), MEPU, pCaller);
	ATC=nil;
}

/*-- Befehle --*/
 
func Goal(id idGoal,pCall,iSelection)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	if(FindObject(idGoal)) 
	{
		Message("{{%i}} <c ffcc00>%s Entfernt</c>",nil,idGoal,GetName(nil,idGoal));
		RemoveAll(idGoal);
		Ziele(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(nil);
	}
	else
	{
		Message("{{%i}} <c ffcc00>%s Hinzugefügt</c>",nil,idGoal,GetName(nil,idGoal));
		CreateObject(idGoal);
		Ziele(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(nil);
	}
}

func Rule(id idRule,pCall,iSelection)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	if(FindObject(idRule)) 
	{
		Message("{{%i}} <c ffcc00>%s Entfernt</c>",nil,idRule,GetName(nil,idRule));
		RemoveAll(idRule);
		Regeln(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(nil);
	}
	else
	{
		Message("{{%i}} <c ffcc00>%s Hinzugefügt</c>",nil,idRule,GetName(nil,idRule));
		CreateObject(idRule);
		Regeln(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(nil);
	}
}

func Objegt(id idObject,pCall,iSelection)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	ATC=nil;
	if(FindObject(idObject)) 
	{
		Message("{{%i}} <c ffcc00>%s Entfernt</c>",nil,idObject,GetName(nil,idObject));
		RemoveAll(idObject);
		Objekte(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(nil);
	}
	else
	{
		Message("{{%i}} <c ffcc00>%s Hinzugefügt</c>",nil,idObject,GetName(nil,idObject));
		PlaceObjects(idObject,50,"GBackSemiSolid");
		Objekte(GetOwner(pCaller));
		GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		return(nil);
	}
}

func Building(id idObject,pCall,iSelection)
{
	ATC=nil;
	var pCaller;
	pCaller = GetCursor(pCall);
	Sound("Connect");
	iSelection = GetMenuSelection(pCaller);
	ATC=1;
	CloseMenu(pCaller);
	ATC=nil;
	var i;
	while((i ?? 0) < (GetPlayerCount(nil) ?? 0))
	{
		if(GetPlrKnowledge(i,idObject)) 
		{
			Message("{{%i}} <c ffcc00>%s Nicht Baubar</c>",nil,idObject,GetName(nil,idObject));
			SetPlrKnowledge(i,idObject,1);
			Bauplane(GetOwner(pCaller));
			GetCursor(GetOwner(pCaller)) -> SelectMenuItem(iSelection);
		}
		else
		{
			Message("{{%i}} <c ffcc00>%s Baubar</c>",nil,idObject,GetName(nil,idObject));
			SetPlrKnowledge(i,idObject,nil);
			Bauplane(GetOwner(pCaller),iSelection);
		}
		S42MigrationStep(i, 1, true);
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
	CreateObject(FXP1,(LandscapeWidth() ?? 0)/2)->Activate(Material("Water"),(LandscapeWidth() ?? 0)-10,10000);
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
	S42MigrationStep(Temperature, -1, true);
}

func ClimUp()
{
	Sound("Connect");
	Message("{{ICE1}} <c ffcc00>Wärmer</c>");
	S42MigrationStep(Temperature, 1, true);
}
