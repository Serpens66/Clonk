/*-- Teams --*/

#strict 3
#include GOAL

static const TEAM_TeamColumn = 0;
static const TEAM_PlayerColumn = 1;
static const TEAM_GoalColumn = 2;
static const TEAM_KillColumn = 3;
static const TEAM_DeathColumn = 4;
// 1024 Spieler werden maximal für das Scoreboard unterstützt!
static const TEAM_iRow = 1024;

local init, fNoTeamCheck;
local evaluation;

protected func Activate(iPlr) // Defaultnachrichten
{
  if (IsFulfilled())
  {
    if((aPoints[(GetPlayerTeam(iPlr) ?? 0)] ?? 0) >= (iWinScore ?? 0))
      return(MessageWindow("$MsgGoalFulfilled$",iPlr));
    else
      return(MessageWindow("$MsgGoalLost$",iPlr));
  }
  return(MessageWindow(Format("$MsgGoalUnfulfilled$", (GetWinScore() ?? 0) - (aPoints[(GetPlayerTeam(iPlr) ?? 0)] ?? 0)),iPlr));
}

// Variablen zum Speichern des Punktestandes

local aPoints;

// Siegpunktzahl

local iWinScore;

// Spielerdinge

local aKill, aDeath;

protected func Initialize()
{
  aPoints = CreateArray();
  aKill = CreateArray();
  aDeath = CreateArray();
  ScheduleCall(this(),"InitScoreboard",1);
  return(_inherited());
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  iPlr ??= 0;
  iX ??= 0;
  iY ??= 0;
  iTeam ??= 0;
  if(!init) return;

  if(!aPoints[(iTeam ?? 0)])  aPoints[(iTeam ?? 0)] = nil;

//  if(iPlr > -1)
//    DoScoreboardShow(true, iPlr);

  //Verfeindung setzen
  Hostility(iPlr);
  // Ins Scoreboard eintragen

  var teamp = GetTeamPlayerCount(iTeam);
  // Team ist neu
  if(Equal(teamp, 1))
    InitSingleplayerTeam(iPlr);
  // Team wechselt von Einzelspieler auf Mehrspieler
  if(Equal(teamp, 2))
  {
    InitMultiplayerTeam(iTeam);
    RemoveSingleplayerTeam(iPlr);
    var j = nil;
    while(!Equal(GetTeamPlayer(iTeam, S42MigrationStep(j, 1)), -1))
    {
      InitPlayer(GetTeamPlayer(iTeam, j));
    }
  }
  // Spieler normal einfügen
  if((teamp ?? 0) > 2)
    InitPlayer(iPlr);
  // Sortieren
  SortTeamScoreboard();
}

/* HUD-Funktionen */

private func GetGoalHUD(player) {
  if((player ?? 0) <= -1) return;
  
  var bar = FindObjectOwner(3HUD,player);
  if(!bar)
  {
    // init the base hud object
    bar = CreateObject(3HUD, nil,nil, player);
	bar->SetPosition(55,140);
    bar->SetGraphics(nil, nil, GetID(), 1,4);
    bar->SetObjDrawTransform(1500,nil,nil,nil,1500,-120000,nil,1);
    bar->SetVisibility(VIS_None);
  }
  return bar;
}

private func UpdateHUDs() {
	var player;
	for(var i=nil; (i ?? 0)<(GetPlayerCount() ?? 0); S42MigrationStep(i, 1)) {
		player = GetPlayerByIndex(i);
		if((player ?? 0) >= 0)
			UpdateHUD(player);
	}
}

private func UpdateHUD(int player) {
	player ??= 0;
	var hud = GetGoalHUD(player);
	if(!hud) return;
	
	// display message under the hud
	// if GetHUDInfo return "" or 0, the icon is made invisible
	// to circumvent this even though you want to display no text,
	// just return a whitespace.
	var g = GetHUDInfo(player, hud);
	if(And(g, !FindObject(CHOS))) {
		hud->SetVisibility(VIS_Owner);
		Message("@%s", hud,g);
		
		// now, display the icons...
	}
	else {
	    hud->SetVisibility(VIS_None);
	}
}

private func SetHUDGraphics(string szGfxName, object pObj, id idSrcDef, int iOverlay, int iOverlayMode, string szAction, int dwBlitMode, object pOvelayObject) {
	iOverlay ??= 0;
	iOverlayMode ??= 0;
	dwBlitMode ??= 0;
	SetGraphics(szGfxName,pObj,idSrcDef,iOverlay,iOverlayMode,szAction,dwBlitMode,pOvelayObject);
	pObj->SetObjDrawTransform(1000,nil,35000+((iOverlay ?? 0)-2)*28000,nil,1000,-66000,nil,iOverlay);
}

public func GetHUDInfo(int player, object hud) {
	player ??= 0;
	var team = GetPlayerTeam(player);
	
	//eigenes Team immer vorne
	if(And(team, (team ?? 0) > 0))
		var sting = Format("<c %x>%d</c>",GetTeamColor(team),TeamGetScore(team));
	
	var anotherteam;
	for(var i=nil; (i ?? 0)<(GetTeamCount() ?? 0); S42MigrationStep(i, 1)) {
		anotherteam = GetTeamByIndex(i);
		if(!Equal(anotherteam, team))
			sting = Format("%s : <c %x>%d</c>",sting,GetTeamColor(anotherteam),TeamGetScore(anotherteam));
	}
	
	return(sting);
}

public func ChooserFinished()
{
  ScheduleCall(this(),"InitScoreboard",1);
}

//Die Initialisierung des Scoreboards. Damit alle Spalten/Zeilen etc. angezeigt werden
private func InitScoreboard()
{
  UpdateHUDs();
  //bool SetScoreboardData (int iRowID, int iColID, string sText, int iData); 

  //Überschriften
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%d $Score$",iWinScore), SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_TeamColumn, "{{KILC}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_PlayerColumn, "{{HZCK}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_GoalColumn, Format("{{%i}}",GetID()), SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_KillColumn, "{{PIWP}}", SBRD_Caption);
  SetScoreboardData(SBRD_Caption, TEAM_DeathColumn, "{{KAMB}}", SBRD_Caption);

  var teams = CreateArray(), team;

  //Daten
  //erstmal rausfinden welche Teams wir anzeigen
  for(var i = nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
  {
    if(!GetPlayerName(GetPlayerByIndex(i)))
	    continue;

	  var done=nil, plrteam = GetPlayerTeam(GetPlayerByIndex(i));
	  if((plrteam ?? 0) < 1) continue;

	  for(var team in teams)
	    if(Equal(team, plrteam))
	      S42MigrationStep(done, 1, true);
	  if(!done)
	  {
	    teams[(GetLength(teams) ?? 0)] = plrteam;
	  }
  }

  //so, jetzt wissen wir die Teams. Jetzt noch nach Teamnummern ordnen. %(
  //Bubblesort!
  var t1, t2;
  for(t1 = nil; (t1 ?? 0) < (GetLength(teams) ?? 0); S42MigrationStep(t1, 1, true)) {
    for(t2 = t1; (t2 ?? 0) < (GetLength(teams) ?? 0); S42MigrationStep(t2, 1, true))
          if((teams[(t2 ?? 0)] ?? 0) < (teams[(t1 ?? 0)] ?? 0))
      {
	      var tmp = teams[(t1 ?? 0)];
                teams[(t1 ?? 0)] = teams[(t2 ?? 0)];
		    teams[(t2 ?? 0)] = tmp;
      }
  }
  //puh, ganz schön viel Aufwand für sowas..
  //aber jetzt müssen wir nur noch darstellen. :)
  for(var x=nil; (x ?? 0) < (GetLength(teams) ?? 0); S42MigrationStep(x, 1, true))
  {
    var t = teams[(x ?? 0)];
    // Team hat keinen Spieler -> Höh?
    if(!GetTeamPlayerCount(t)) continue;
    // Team hat nur einen Spieler -> "Einzelspielerteam"
    if(Equal(GetTeamPlayerCount(t), 1))
    {
      InitSingleplayerTeam(GetTeamPlayer(t, 1));
      continue;
    }
    // Team einfügen, 1024 Spieler werden maximal unterstützt!
    InitMultiplayerTeam(t);
    // Spieler aus dem Team einfügen
    var j = nil;
    while(!Equal(GetTeamPlayer(t, S42MigrationStep(j, 1)), -1))
    {
      InitPlayer(GetTeamPlayer(t, j));
    }
  }
  // Sortieren
  SortTeamScoreboard();
  // Fertig
  init = true;
}

/* Scoreboard */

// Funktionen zum Eintragen

// Team eintragen, dass mehrere Spieler hat
private func InitMultiplayerTeam(int iTeam)
{
  iTeam ??= 0;
  if((iTeam ?? 0) < 1) return;

  var col = GetTeamColor(iTeam);

  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), SBRD_Caption, "");
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_TeamColumn,  Format("<c %x>%s</c>", col, GetTeamName(iTeam)), iTeam);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_PlayerColumn, "");
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aPoints[(iTeam ?? 0)]), aPoints[(iTeam ?? 0)]);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(iTeam)), (TeamGetKills(iTeam) ?? 0)+1);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_DeathColumn, Format("<c %x>%d</c>", col, TeamGetDeath(iTeam)));
  SortTeamScoreboard();
}

// Mehrspielerteam austragen
private func RemoveMultiplayerTeam(int iTeam)
{
  iTeam ??= 0;
  if((iTeam ?? 0) < 1) return;

  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_TeamColumn);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_PlayerColumn);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_GoalColumn);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_KillColumn);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_DeathColumn);
  SortTeamScoreboard();
}

// Team eintragen, dass nur einen Spieler hat
private func InitSingleplayerTeam(int iPlr)
{
  iPlr ??= 0;
  if(Equal(iPlr, -1)) return;

  var col = GetPlrColorDw(iPlr);

  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, "");
  SetScoreboardData(iPlr, TEAM_GoalColumn,  Format("<c %x>%d</c>", col, aPoints[(GetPlayerTeam(iPlr) ?? 0)]), aPoints[(GetPlayerTeam(iPlr) ?? 0)]);
  SetScoreboardData(iPlr, TEAM_KillColumn,  Format("<c %x>%d</c>", col, TeamGetKills(GetPlayerTeam(iPlr))), (TeamGetKills(GetPlayerTeam(iPlr)) ?? 0)+1);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", col, aDeath[(iPlr ?? 0)]), aDeath[(iPlr ?? 0)]);
  SortTeamScoreboard();
}

// Einzelspielerteam austragen
private func RemoveSingleplayerTeam(int iPlr)
{
  iPlr ??= 0;
  if(Equal(iPlr, -1)) return;

  SetScoreboardData(iPlr, TEAM_TeamColumn);
  SetScoreboardData(iPlr, TEAM_PlayerColumn);
  SetScoreboardData(iPlr, TEAM_GoalColumn);
  SetScoreboardData(iPlr, TEAM_KillColumn);
  SetScoreboardData(iPlr, TEAM_DeathColumn);
  SortTeamScoreboard();
}

// Spieler eintragen
private func InitPlayer(int iPlr)
{
  // Team 0 oder -1 können wir nicht behandeln
  iPlr ??= 0;
  if((GetPlayerTeam(iPlr) ?? 0) < 1) return;
  if(Equal(iPlr, -1)) return;
  
  UpdateHUDs();
  
  // Eintragen
  SetScoreboardData(iPlr, SBRD_Caption, "");
  SetScoreboardData(iPlr, TEAM_TeamColumn, Format("<c %x>\\</c>",GetTeamColor(GetPlayerTeam(iPlr))), GetPlayerTeam(iPlr));
  SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr));
  SetScoreboardData(iPlr, TEAM_GoalColumn, "", aPoints[(GetPlayerTeam(iPlr) ?? 0)]);
  SetScoreboardData(iPlr, TEAM_KillColumn, Format("%d", aKill[(iPlr ?? 0)]), aKill[(iPlr ?? 0)]);
  SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[(iPlr ?? 0)]), aDeath[(iPlr ?? 0)]);
  SortTeamScoreboard();
}

// Spieler austragen
private func RemoveScoreboardPlayer(int iPlr)
{
  iPlr ??= 0;
  if(Equal(iPlr, -1)) return;
  
  if(Equal(GetTeamPlayerCount(GetPlayerTeam(iPlr)), 1)) return(RemoveSingleplayerTeam(iPlr));

  SetScoreboardData(iPlr, TEAM_TeamColumn);
  SetScoreboardData(iPlr, TEAM_PlayerColumn);
  SetScoreboardData(iPlr, TEAM_GoalColumn);
  SetScoreboardData(iPlr, TEAM_KillColumn);
  SetScoreboardData(iPlr, TEAM_DeathColumn);

  SortTeamScoreboard();

  // Team nicht prüfen
  if(fNoTeamCheck) return;
  fNoTeamCheck = true;

  // Team wechselt zum Einzelspielerteam
  if(Equal(GetTeamPlayerCount(GetPlayerTeam(iPlr)), 2)) // 2, weil der Spieler in diesem Moment noch im Spiel ist
  {
    var j = nil, p;
    while(!Equal(GetTeamPlayer(GetPlayerTeam(iPlr), S42MigrationStep(j, 1)), -1))
    {
      p = GetTeamPlayer(iPlr, j);
      if(Equal(p, iPlr)) continue;
      //Log("%d", p);
//      RemovePlayer(p, true);
      RemoveMultiplayerTeam(GetPlayerTeam(iPlr));
      InitSingleplayerTeam(p);
    }
  }
  fNoTeamCheck = nil;
}

// Engine-Call
public func RemovePlayer(int iPlr)
{
  iPlr ??= 0;
  if(Equal(iPlr, -1)) return;

	// Auswertungsdialog
	DoEvaluation(iPlr);

  UpdateHUDs();
  RemoveScoreboardPlayer(iPlr);
  aDeath[(iPlr ?? 0)] = nil;
  aKill[(iPlr ?? 0)] = nil;
}

// Sortieren
private func SortTeamScoreboard()
{
  // Nach Toden (Spieler)
  SortScoreboard(TEAM_DeathColumn);
  // Nach Kills (Spieler)
  SortScoreboard(TEAM_KillColumn, true);
  // Nach Teams (Spieler)
  SortScoreboard(TEAM_TeamColumn);
  // Nach Punkten (Teams)
  SortScoreboard(TEAM_GoalColumn, true);
}

/* Helferlein */

// Schaut, wie viele Spieler in einem Team sind
private func GetTeamPlayerCount(int iTeam)
{
  iTeam ??= 0;
  for(var i = (GetPlayerCount() ?? 0)-1,j ; (i ?? 0) > -1 ; S42MigrationStep(i, -1, true))
    if(Equal(GetPlayerTeam(GetPlayerByIndex(i)), iTeam))
      S42MigrationStep(j, 1, true);
  return(j);
}

// Liefert nacheinander die einzelnen Spieler eines Teams (Parameter 1 - x)
private func GetTeamPlayer(int iTeam, int iNo)
{
  iTeam ??= 0;
  iNo ??= 0;
  for(var i = (GetPlayerCount() ?? 0)-1,j ; (i ?? 0) > -1 ; S42MigrationStep(i, -1, true))
    if(Equal(GetPlayerTeam(GetPlayerByIndex(i)), iTeam))
      if(!S42MigrationStep(iNo, -1))
        return(GetPlayerByIndex(i));
  return(-1);

}

// Liefert alle Kills eines Teams
private func TeamGetKills(int iTeam)
{
  iTeam ??= 0;
  for(var i = (GetPlayerCount() ?? 0)-1,j ; (i ?? 0) > -1 ; S42MigrationStep(i, -1, true))
    if(Equal(GetPlayerTeam(GetPlayerByIndex(i)), iTeam))
      S42MigrationAssign(j, "+=", (aKill[(GetPlayerByIndex(i) ?? 0)] ?? 0));
  return(j);
}

private func TeamGetScore(int iTeam)
{
  iTeam ??= 0;
  return(aPoints[(iTeam ?? 0)]);
}

private func BestTeamGetScore(int iExclude)
{
  iExclude ??= 0;
  var greatest = nil;
  for(var i = nil; (i ?? 0)<(GetLength(aPoints) ?? 0) ;S42MigrationStep(i, 1)) {
    if(!Equal(i, iExclude))
      greatest = Max(aPoints[(i ?? 0)],greatest);
  }
  return greatest;
}

// Liefert alle Death eines Teams
// *copy'n'paste + -Kill +Death!
private func TeamGetDeath(int iTeam)
{
  iTeam ??= 0;
  for(var i = (GetPlayerCount() ?? 0)-1,j ; (i ?? 0) > -1 ; S42MigrationStep(i, -1, true))
    if(Equal(GetPlayerTeam(GetPlayerByIndex(i)), iTeam))
      S42MigrationAssign(j, "+=", (aDeath[(GetPlayerByIndex(i) ?? 0)] ?? 0));
  return(j);
}

//aktualisiert den Punktestand
private func UpdateScoreboard(int iTeam)
{
  iTeam ??= 0;
  UpdateHUDs();

  if((GetTeamPlayerCount(iTeam) ?? 0) > 1)
  {
    SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_GoalColumn, Format("<c %x>%d</c>", GetTeamColor(iTeam), aPoints[(iTeam ?? 0)]), aPoints[(iTeam ?? 0)]);
    var p, j=nil;
    while(!Equal(GetTeamPlayer(iTeam, S42MigrationStep(j, 1)), -1))
    {
      p = GetTeamPlayer(iTeam, j);
      SetScoreboardData(p, TEAM_GoalColumn, "", aPoints[(iTeam ?? 0)]);
    }
  }
  else
    SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), aPoints[(iTeam ?? 0)]), aPoints[(iTeam ?? 0)]);

  SortTeamScoreboard();
}

private func Hostility(int iPlr)
{
  iPlr ??= 0;
  for(var x; (x ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(x, 1, true))
    if(And(GetPlayerByIndex(x), !Equal(iPlr, x))) {
      if(!Equal(GetPlayerTeam(iPlr), GetPlayerTeam(GetPlayerByIndex(x))))
        SetHostility(iPlr, GetPlayerByIndex(x), 1, 1);
      else
        SetHostility(iPlr, GetPlayerByIndex(x), nil, 1);
    }
}

public func Destruction()
{
  //Scoreboard löschen
  for(var x=1; GetTeamName(x); S42MigrationStep(x, 1, true)) {
    var iColID = 1;
    SetScoreboardData(x,       SBRD_Caption, nil);
    SetScoreboardData(x,       iColID,       nil);
  }
}

/* Aufrufe */

public func WinScoreChange(int iNewScore)
{
  iNewScore ??= 0;
  iWinScore = iNewScore;
  UpdateHUDs();
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("%d $Score$",iWinScore), SBRD_Caption);
}

public func ScoreChange(int iTeam, int iChange)
{
  iTeam ??= 0;
  iChange ??= 0;
  S42MigrationAssign(aPoints[(iTeam ?? 0)], "+=", (iChange ?? 0));
  UpdateScoreboard(iTeam);
  UpdateHUDs();
  CheckTime();
}

public func IsFulfilled() // Siegreiches Team?
{
	// leben noch menschliche spieler?
	if(!GetPlayerCount(C4PT_User)) {
		if(IsNetwork()) // nein - wenn wir im Netzwerk sind beenden.
		{
            EvaluationForAll();
			return(1);
		}
	}

  var team;
  for(var iScore in aPoints) {
    if((iScore ?? 0) >= (iWinScore ?? 0))
    {
      EliminateLosers();
	  
      Message("@<c %x>$WinMsg$</c>",nil,GetTeamColor(team),GetTeamName(team));
	  
      //Leben noch Verlierer? -> Ligainkompatibilität!
      if(LosersAlive())
        return(nil);
      
      EvaluationForAll();
      return(1);
    }
    S42MigrationStep(team, 1, true);
 }
  // Nur noch ein Team und das hat schon Punkte? -> Sieg (Verlierer sind rausgegangen)
  team = OneTeam();
  if(!Equal(team, -1))
    if((aPoints[(team ?? 0)] ?? 0) > 0)
    {
      Message("@<c %x>$WinMsg$</c>",nil,GetTeamColor(team),GetTeamName(team));

      EvaluationForAll();
      return(1);
    }
}

private func EvaluationForAll() {
	// Auswertungsdialog
	if(!evaluation) {
		for(var i=nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
		{
	    	var plr = GetPlayerByIndex(i);
			DoEvaluation(plr);
		}
		evaluation = true;
	}
}

func DoEvaluation(int plr)
{
	plr ??= 0;
	AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aKill[(plr ?? 0)]), GetPlayerID(plr));
	AddEvaluationData(Format("{{KAMB}}$Death$: %d", aDeath[(plr ?? 0)]), GetPlayerID(plr));
}

private func OneTeam() // Nur noch ein einziges Team im Spiel?
{
  var plr, team;
  for(var i = nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
  {
    plr = GetPlayerByIndex(i);
    if(!team) team = GetPlayerTeam(plr);
    else if(!Equal(GetPlayerTeam(plr), team))
      return(-1);
  }
  return(team);
}

private func EliminateLosers()  // Hier trennt sich die Spreu vom Weizen
{
  for(var i=nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
    if((aPoints[(GetPlayerTeam(GetPlayerByIndex(i)) ?? 0)] ?? 0) < (iWinScore ?? 0))
      EliminatePlayer(GetPlayerByIndex(i)); // HARHAR
}

private func LosersAlive()
{
  for(var x = nil ; (x ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(x, 1, true))
    if((aPoints[(GetPlayerTeam(GetPlayerByIndex(x)) ?? 0)] ?? 0) < (iWinScore ?? 0))
      if((GetPlayerByIndex(x) ?? 0) >= 0)
        return(1);

  return(nil);
}

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
  iPlr ??= 0;
  iMurdererPlr ??= 0;
  if(Or(Equal(iPlr, -1), !GetPlayerName(iPlr))) return;

  UpdateHUDs();
  
  //Tode++
  S42MigrationStep(aDeath[(iPlr ?? 0)], 1, true);
  if((GetTeamPlayerCount(GetPlayerTeam(iPlr)) ?? 0) > 1)
    SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[(iPlr ?? 0)]), aDeath[(iPlr ?? 0)]);
  else
    SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aDeath[(iPlr ?? 0)]), aDeath[(iPlr ?? 0)]);
  if((GetTeamPlayerCount(GetPlayerTeam(iPlr)) ?? 0) > 1)
    SetScoreboardData((TEAM_iRow ?? 0) + (GetPlayerTeam(iPlr) ?? 0), TEAM_DeathColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iPlr)), TeamGetDeath(GetPlayerTeam(iPlr))), (TeamGetDeath(GetPlayerTeam(iPlr)) ?? 0)+1);
  //kein Selfkill? kein Teamkill?
  if(And(!Equal(iMurdererPlr, -1), !Equal(GetPlayerTeam(iPlr), GetPlayerTeam(iMurdererPlr))))
  {	
    //kill++
    S42MigrationStep(aKill[(iMurdererPlr ?? 0)], 1, true);
    SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("<c %x>%d</c>", GetPlrColorDw(iMurdererPlr), aKill[(iMurdererPlr ?? 0)]), aKill[(iMurdererPlr ?? 0)]);
    //team: kill++
    if((GetTeamPlayerCount(GetPlayerTeam(iMurdererPlr)) ?? 0) > 1)
    {
      SetScoreboardData((TEAM_iRow ?? 0) + (GetPlayerTeam(iMurdererPlr) ?? 0), TEAM_KillColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iMurdererPlr)), TeamGetKills(GetPlayerTeam(iMurdererPlr))), (TeamGetKills(GetPlayerTeam(iMurdererPlr)) ?? 0)+1);
      SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("%d", aKill[(iMurdererPlr ?? 0)]), aKill[(iMurdererPlr ?? 0)]);
    }
  }
  SortTeamScoreboard();
  // Geld
  Money(iPlr, pClonk, iMurdererPlr);
}

// Default-Verhalten für Geld
public func Money(int iPlr, object pClonk, int iMurdererPlr)
{
  iPlr ??= 0;
  iMurdererPlr ??= 0;
  if(!Equal(iMurdererPlr, NO_OWNER))
  {
    // Teamkill! Wert des Clonks wird abgezogen
    if(Equal(GetPlayerTeam(iPlr), GetPlayerTeam(iMurdererPlr)))
      DoWealth(iMurdererPlr, - (pClonk->GetValue(nil,nil,nil, iMurdererPlr) ?? 0));
    // Regulärer Kill, Wert des Clonks wird vergeben
    else
      DoWealth(iMurdererPlr, + (pClonk->GetValue(nil,nil,nil, iMurdererPlr) ?? 0));
  }
  // Geld für den getöteten, halber Wert des Clonks wird gegeben
  DoWealth(iPlr, (+ (pClonk->GetValue(nil,nil,nil, iPlr) ?? 0) ?? 0) / 2);
}
// Hinweis: die Werte können in CalcValue-Aufrufe verändert werden

/* Globale Funktionen */

global func DoTeamScore(int iTeam, int iChange)
{
  iTeam ??= 0;
  iChange ??= 0;
  for(var pGoal in FindObjects(Find_Func("IsTeamGoal")))
    pGoal->~ScoreChange(iTeam, iChange);
}

global func SetWinScore(int iNewScore, object pGoal)
{
  iNewScore ??= 0;
  if(pGoal)
  {
    pGoal->~WinScoreChange(iNewScore);
    return(nil);
  }
  for(var goal in FindObjects(Find_Func("IsTeamGoal")))
    goal->~WinScoreChange(iNewScore);
}

global func GetWinScore(object pGoal)
{
  return(LocalN("iWinScore", pGoal));
}

/* Status */

public func IsTeamGoal() { return(1); }
