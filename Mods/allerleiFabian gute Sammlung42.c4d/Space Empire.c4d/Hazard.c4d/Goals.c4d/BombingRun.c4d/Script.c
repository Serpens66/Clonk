/*-- BR --*/

#strict 3
#include TEAM

protected func Initialize()
{
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

public func GetHUDInfo(int player, object hud) {
	
	player ??= 0;
	var team = GetPlayerTeam(player);
	if((team ?? 0) < 1) return(inherited(player));
	
	var bomb = GetBomb();
	var bc = GetBombCarrier(bomb);
	if(bc) {
		var bcteam = GetPlayerTeam(GetOwner(bc));
		
		// which side
		if(Equal(bcteam, team))
			SetHUDGraphics(nil,hud,GetID(),2,2,"BombFriendly");
		else
			SetHUDGraphics(nil,hud,GetID(),2,2,"BombEnemy");
	}
	// Bombe an Heimatposition
	else if(And(bomb, bomb->IsAtHome())) {
		SetHUDGraphics(nil,hud,GetID(),2,2);
	}
	// ansonsten isse lose
	else {
		SetHUDGraphics(nil,hud,GetID(),2,2,"BombGone");
	}
	
	return(inherited(player));
}

public func BombTaken() {
	UpdateHUDs();
}

public func BombReturned() {
	UpdateHUDs();
}

public func BombScored() {
	UpdateHUDs();
}

public func BombLoose() {
	UpdateHUDs();
}

/* Globale Funktionen */

global func CreateBomb(int iX, int iY, int iColor) // Platziert eine Bombe an der angegebenen Position mit der angegebenen Farbe.
{
  iX ??= 0;
  iY ??= 0;
  iColor ??= 0;
  var pBomb = CreateObject(GBRB, AbsX(iX), AbsY(iY), -1);
  if (pBomb) pBomb->Init(iX,iY,iColor);
}

public func Activate(iCaller)
{
 var desc=GetDesc();
 desc=Format("%s||$WinString$",desc,iWinScore);
 MessageWindow(desc,iCaller);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

// Taktik für KI

public func GetBomb()
{
  return(FindObject2(Find_ID(GBRB)));
}

public func GetBombCarrier(object bomb)
{
  if (!bomb) return(nil);
  return(bomb->Contained());
}

public func GetOwnGoal(int owner)
{
  owner ??= 0;
  for(var goal in FindObjects(Find_ID(GBRG)))
    if(Equal(goal->GetGoalTeam(), GetPlayerTeam(owner)))
      return(goal);
}

private func GetEnemyGoal(int owner)
{
  owner ??= 0;
  for(var goal in FindObjects(Find_ID(GBRG)))
    if(!Equal(goal->GetGoalTeam(), GetPlayerTeam(owner)))
      return(goal);
}

// Tactic for bombing run
public func AITactic(object pAIPlayer)
{
  var owner = pAIPlayer->GetOwner();
  var bomb = GetBomb();
  if (!bomb) return;
  var carrier = GetBombCarrier(bomb);
  var egoal = GetEnemyGoal(owner);

  for(var i=nil, pCrew ; pCrew = GetCrew(owner, i) ; S42MigrationStep(i, 1, true)) {
    if(pCrew->Contained()) continue;

    // bomb carrier
    if(Equal(pCrew, carrier)) {
      // rush to enemy goal
      if (And(egoal, !Equal(pCrew->GetMacroCommand(nil, 1), egoal)))
        pAIPlayer->Run(pCrew, egoal);
    }
    else {
      // someone has the bomb
      if(carrier) {
        if(!Equal(pCrew->GetMacroCommand(nil, 1), carrier)) {
          // hostile bomb carrier?
          if(Hostile(owner, carrier->GetOwner())) {
              // Intercept!
              pCrew->SetMacroCommand(this, "Follow", carrier, nil,nil,nil, Aggro_Follow);
          }
          else {
            // friendly bomb carrier, guard
              pCrew->SetMacroCommand(this, "Follow", carrier, nil,nil,nil, Aggro_Shoot);
          }
        }
      }
      // No one has the bomb!
      else {
        // rush to bomb
        if (!Equal(pCrew->GetMacroCommand(nil, 1), bomb))
          pAIPlayer->Run(pCrew, bomb);
      }
    }
  }
}
