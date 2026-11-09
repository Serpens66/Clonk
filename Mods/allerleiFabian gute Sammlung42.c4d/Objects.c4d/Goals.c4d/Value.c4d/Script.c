/*-- Siedlungspunkte --*/

#strict 3
#include GOAL

// Punktzahl
local TargetScore;

protected func Initialize()
{
  // Anderes Objekt vorhanden? Selber dazuzählen
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    S42MigrationAssign(LocalN("TargetScore", pGoal), "+=", 100);
    return(RemoveObject()); 
  }
  // Punktzahl setzen
  TargetScore = 100;
  return(inherited());
}
    
private func AllPlayersHaveValue(iValue)
{
  var iPlayers,i;
  for (iPlayers = GetPlayerCount(); iPlayers; S42MigrationStep(i, 1, true))
    if (GetPlayerName(i))
    {
      S42MigrationStep(iPlayers, -1, true);
      if ((GetPlrValueGain(i) ?? 0) < (iValue ?? 0)) return(nil);
    }
  return(1);
}
    
public func IsFulfilled()
{
  if (!TargetScore) TargetScore = 100;
  return(AllPlayersHaveValue(TargetScore));
}

public func IsFulfilledforPlr(iPlr) 
{
  return ((GetPlrValueGain(iPlr) ?? 0) >= (TargetScore ?? 0));
}

protected func Activate(iPlayer)
{
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$", TargetScore), iPlayer));
  MessageWindow(Format("$MsgGoalUnfulfilled$", GetPlrValueGain(iPlayer), TargetScore), iPlayer);
  return(1);
}

