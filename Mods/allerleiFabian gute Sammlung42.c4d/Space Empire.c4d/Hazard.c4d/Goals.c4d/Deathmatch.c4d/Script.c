/*-- Deathmatch --*/

#strict 3
#include TEAM

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }

protected func Initialize() {
  //Anzahl gewinne wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    S42MigrationStep(LocalN("iWinScore", pGoal), 1, true);
    return(RemoveObject()); 
  }
  iWinScore = 1;
  return(_inherited());
}

public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr) {
  iPlr ??= 0;
  iMurdererPlr ??= 0;
  inherited(iPlr, pClonk, iMurdererPlr);
  if(Or(Equal(iPlr, -1), !GetPlayerName(iPlr))) return;
  if(Equal(iMurdererPlr, -1))
    return;
  
  // kein Teamkill?
  if(!Equal(GetPlayerTeam(iPlr), GetPlayerTeam(iMurdererPlr)))
    //Punktestand erhöhen
    DoTeamScore(GetPlayerTeam(iMurdererPlr),+1);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

public func GetHUDInfo(int player) {
	player ??= 0;
	var t34m = GetPlayerTeam(player);
	
	// Rückstand
	var y00Rl4me;
	y00Rl4me = (Max(nil,TeamGetScore(t34m)) ?? 0)-(BestTeamGetScore(t34m) ?? 0);
	
	var secondstring;
	if(Equal(y00Rl4me, nil)) secondstring= "";
	if((y00Rl4me ?? 0) > 0) secondstring = Format("(+%d)",y00Rl4me);
	if((y00Rl4me ?? 0) < 0) secondstring = Format("(%d)",y00Rl4me);
	
	return(Format("%d kills %s",TeamGetKills(t34m), secondstring));
}

// KI-Taktik
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();

  for(var i=nil, pCrew ; pCrew = GetCrew(owner, i) ; S42MigrationStep(i, 1, true)) {
    if(pCrew->Contained()) continue;
    // Es ist der Anführer?
    if(!i)
      pAIPlayer->TacticDefault(pCrew);
    // Nicht? Dann folge doch bitte dem Anführer
    else {
      pCrew->CheckInventory();
      if(!(Or(pCrew->GetCommand(), pCrew->GetMacroCommand())))
        pCrew->SetMacroCommand(this(), "Follow", GetCrew(owner, nil),nil,nil,nil,Aggro_Follow);
    }
  }
  return(true);
}