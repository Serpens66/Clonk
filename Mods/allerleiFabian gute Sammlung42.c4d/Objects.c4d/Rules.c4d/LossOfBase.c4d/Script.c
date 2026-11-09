/*-- LossOfBase --*/

#strict 3

private func Execute() {
  // Alle Spieler überprüfen
  var plr_num;
  for (var plr_cnt = GetPlayerCount(); plr_cnt; S42MigrationStep(plr_num, 1, true)) {
    if (GetPlayerName(plr_num)) {
      S42MigrationStep(plr_cnt, -1, true);
      if (!Equal(GetPlayerTeam(plr_num), -1)) if (!FindBase(plr_num))
        EliminatePlayer(plr_num);
    }
  }
}
      
protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}
