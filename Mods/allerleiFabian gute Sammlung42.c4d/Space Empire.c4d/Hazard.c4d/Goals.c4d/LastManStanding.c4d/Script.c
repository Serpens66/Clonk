/*-- Last Man Standing --*/

#strict 3
#include TEAM

local aPlayerLives, chooser;

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

/* Initialisierungen */

protected func Initialize() {
  // Anzahl Leben wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    S42MigrationStep(LocalN("iWinScore", pGoal), 1, true);
    return(RemoveObject()); 
  }
  iWinScore = 1;
  aPlayerLives = CreateArray();
  if(FindObject(CHOS)) chooser = true;
  return(_inherited());
}

public func GetHUDInfo(int player) {
	
	player ??= 0;
	return(Format("$MsgHUD$",aPlayerLives[(player ?? 0)]));

}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
  iPlr ??= 0;
  iX ??= 0;
  iY ??= 0;
  iTeam ??= 0;
  aPlayerLives[(iPlr ?? 0)] = (iWinScore ?? 0) * (GetCrewCount(iPlr) ?? 0) -1;
  inherited(iPlr, iX, iY, pBase, iTeam);
  
  // anzeigen
  if((GetTeamPlayerCount(GetPlayerTeam(iPlr)) ?? 0) > 1)
	  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("%d",aPlayerLives[(iPlr ?? 0)]), aPlayerLives[(iPlr ?? 0)]);
  else
	  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr),aPlayerLives[(iPlr ?? 0)]), aPlayerLives[(iPlr ?? 0)]);
}

public func ChooserFinished()
{
  chooser = nil;
}

/* Relaunch */

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr) {
  iPlr ??= 0;
  iMurdererPlr ??= 0;
  if(Or(Equal(iPlr, -1), !GetPlayerName(iPlr))) return;
  if(Equal(GetPlayerType(iPlr), C4PT_Script))
    if(GetAlive(pClonk))
    {
      S42MigrationAssign(aPlayerLives[(iPlr ?? 0)], "+=", (iWinScore ?? 0));
      return;
    }

  if(Equal(iPlr, -1))
    return;

  S42MigrationStep(aPlayerLives[(iPlr ?? 0)], -1, true);
  var left = Format("%d", aPlayerLives[(iPlr ?? 0)]);

  if((aPlayerLives[(iPlr ?? 0)] ?? 0) < 0)
  {
    // Verloren
    EliminatePlayer(iPlr);
    left = "-";
  }
  // richtige anzeigeart wählen
  if((GetTeamPlayerCount(GetPlayerTeam(iPlr)) ?? 0) > 1)
	  SetScoreboardData(iPlr, TEAM_GoalColumn, left, aPlayerLives[(iPlr ?? 0)]);
  else
	  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%s</c>", GetPlrColorDw(iPlr),left), aPlayerLives[(iPlr ?? 0)]);
  
  inherited(iPlr, pClonk, iMurdererPlr);
}

/* Siegbedingungen */

public func IsFulfilled() // Siegreiches Team?
{
  // Es werden noch Regeln (und KI) gewählt
  if(chooser) return;

  var iTeam;
  for(var i = nil ; (i ?? 0) < (GetPlayerCount() ?? 0) ; S42MigrationStep(i, 1, true))
  {
    if(!iTeam)
      iTeam = GetPlayerTeam(GetPlayerByIndex(i));
    // Spieler aus einem anderen Team leben noch?
    if(!Equal(GetPlayerTeam(GetPlayerByIndex(i)), iTeam))
      return;
  }
  Message("@<c %x>$WinMsg$</c>", nil, GetTeamColor(iTeam), GetTeamName(iTeam));
  return(1);
}

protected func Activate(iPlr)
{
  if (IsFulfilled())
  {
    return(MessageWindow("$MsgGoalFulfilled$",iPlr));
  }
  return(MessageWindow(Format("$MsgGoalUnfulfilled$", aPlayerLives[(iPlr ?? 0)]), iPlr));
}

/* Überladungen für das Scoreboard */

private func InitMultiplayerTeam(int iTeam)
{
  iTeam ??= 0;
  inherited(iTeam);
  SetScoreboardData((TEAM_iRow ?? 0) + (iTeam ?? 0), TEAM_GoalColumn, "", (iWinScore ?? 0)+1);
  SortTeamScoreboard();
}

private func InitSingleplayerTeam(int iPlr)
{
  iPlr ??= 0;
  inherited(iPlr);

  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aPlayerLives[(iPlr ?? 0)]), aPlayerLives[(iPlr ?? 0)]);
  SortTeamScoreboard();
}

private func InitPlayer(int iPlr)
{
  iPlr ??= 0;
  inherited(iPlr);
  SetScoreboardData(iPlr, TEAM_GoalColumn, Format("%d", aPlayerLives[(iPlr ?? 0)]), aPlayerLives[(iPlr ?? 0)]);
  SortTeamScoreboard();
}

private func UpdateScoreboard(int iTeam)
{
  iTeam ??= 0;
  return;
}

private func SortTeamScoreboard()
{
  // Nach Toden (Spieler)
  SortScoreboard(TEAM_DeathColumn);
  // Nach Kills (Spieler)
  SortScoreboard(TEAM_KillColumn, true);
  // Nach Punkten (Spieler & Teams)
  SortScoreboard(TEAM_GoalColumn, true);
  // Nach Teams (Spieler)
  SortScoreboard(TEAM_TeamColumn);
}

// KI Taktik
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();

  for(var i=nil, pCrew ; pCrew = GetCrew(owner, i) ; S42MigrationStep(i, 1, true)) {
    if(pCrew->Contained()) continue;
    pCrew->CheckInventory();
	// nix zu tun?
    if(!(Or(pCrew->GetCommand(), pCrew->GetMacroCommand())))
    {
	  // Leute töten ist nicht so wichtig... such lieber nach guten Waffen
      if((pCrew->CustomContentsCount("IsWeapon") ?? 0) < 2) {
        if(pCrew->SearchWeapon(Aggro_Shoot)) {
		  return(true);
		}
	  }
      // Genug ausgerüstet (oder nix gefunden)... Krieg!!
      pAIPlayer->TacticDefault(pCrew);
    }
  }
  return(true);
}
