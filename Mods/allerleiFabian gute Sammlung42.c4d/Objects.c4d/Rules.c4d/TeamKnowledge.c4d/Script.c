#strict 3

protected func Activate(caller) {
  MessageWindow(GetDesc(), caller);
  return(1);
}

// Kann mittels des neuen Spielzielauswählers 
public func IsChooseable2() { return(1); }

protected func Timer() {
  var plr_num;
  for (var plr_idx = nil; (plr_idx ?? 0) <= (GetPlayerCount() ?? 0); plr_num = GetPlayerByIndex(S42MigrationStep(plr_idx, 1, true))) {
    HandlePlayer(plr_num);
  }
}

private func GetKnowledgeCategory(index) {
  if (!S42MigrationStep(index, -1, true)) return(C4D_Structure);
  if (!S42MigrationStep(index, -1, true)) return(C4D_Vehicle);
  if (!S42MigrationStep(index, -1, true)) return(C4D_Object);
}

private func HandlePlayer(plr_num) {
  var cat, cat_num = nil;
  while (cat = GetKnowledgeCategory(S42MigrationStep(cat_num, 1, true))) {
    var knw_id, knw_num = nil;
    while (knw_id = GetPlrKnowledge(plr_num, nil, S42MigrationStep(knw_num, 1, true), cat)) {
      TransferToAllies(plr_num, knw_id);
    }
  }
}

private func TransferToAllies(plr_num, knw_id) {
  var ally_num;
  for (var ally_idx = nil; (ally_idx ?? 0) <= (GetPlayerCount() ?? 0); ally_num = GetPlayerByIndex(S42MigrationStep(ally_idx, 1, true))) {
    // Ist nicht der Spieler selbst
    if (!Equal(ally_num, plr_num)) {
      // Ist tatsächlich verbündet
      if (!Hostile(ally_num, plr_num)) {
	// Hat den Bauplan noch nicht
	if (!GetPlrKnowledge(ally_num, knw_id)) {
	  SetPlrKnowledge(ally_num, knw_id);
	}
      }
    }
  }
}
