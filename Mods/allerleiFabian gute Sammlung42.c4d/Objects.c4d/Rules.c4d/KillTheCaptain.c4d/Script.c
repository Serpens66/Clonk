/*-- KillTheCaptain --*/

#strict 3

private func Execute()
{
  // Alle Spieler überprüfen
  var iPlrCnt, iPlr = nil;
  for (iPlrCnt = GetPlayerCount(); iPlrCnt; S42MigrationStep(iPlr, 1, true))
    if (GetPlayerName(iPlr))
      {
      S42MigrationStep(iPlrCnt, -1, true);
      if (!Equal(GetPlayerTeam(iPlr), -1)) if (!GetCaptain(iPlr))
        EliminatePlayer(iPlr);  
      }
}

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}