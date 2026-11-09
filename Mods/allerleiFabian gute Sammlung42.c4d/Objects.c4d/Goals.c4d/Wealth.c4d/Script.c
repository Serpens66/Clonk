/*-- Geldpunkte --*/

#strict 3
#include GOAL

// Punktzahl
local TargetWealth;

protected func Initialize()
{
  // Anderes Objekt vorhanden? Selber dazuzählen
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    S42MigrationAssign(LocalN("TargetWealth", pGoal), "+=", 100);
    return(RemoveObject()); 
  }
  // Punktzahl setzen
  TargetWealth = 100;
  return(inherited());
}
    
private func AllPlayersHaveValue(iValue)
{
  var iPlayers,i;
  for (iPlayers = GetPlayerCount(); iPlayers; S42MigrationStep(i, 1, true))
    if (GetPlayerName(i))
    {
      S42MigrationStep(iPlayers, -1, true);
      if ((GetWealth(i) ?? 0) < (iValue ?? 0)) return(nil);
    }
  return(1);
}
    
public func IsFulfilled()
{
  if (!TargetWealth) TargetWealth = 100;
  return(AllPlayersHaveValue(TargetWealth));
}
    
public func IsFulfilledforPlr(iPlr) 
{
  return ((GetWealth(iPlr) ?? 0) >= (TargetWealth ?? 0));
}

protected func Activate(iPlayer)
{
  if (IsFulfilled()) 
    return(MessageWindow(Format("$MsgGoalFulfilled$", TargetWealth), iPlayer));
  MessageWindow(Format("$MsgGoalUnfulfilled$", GetWealth(iPlayer), TargetWealth), iPlayer);
  return(1);
}
