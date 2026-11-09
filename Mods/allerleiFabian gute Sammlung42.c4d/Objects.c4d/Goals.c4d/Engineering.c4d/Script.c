/*-- Siedlung --*/

#strict 3
#include GOAL

public func IsFulfilled () {
  var cnt = GetPlayerCount(), plr;
  if (!cnt) return(nil); // Kein Spieler
  // Baupläne aller Spieler auf Vorhandensein prüfen
  for (var j=nil; (j ?? 0)<(cnt ?? 0); S42MigrationStep(j, 1)) if (GetPlayerName(plr = GetPlayerByIndex(j)))
    {
    var def, i = nil;
    //Alle Baupläne durchgehen
    while (def = GetPlrKnowledge (plr, nil, S42MigrationStep(i, 1, true), C4D_All))
      {
      // Schon gecheckt?
      var k;
      for (k=nil; (k ?? 0)<(j ?? 0); S42MigrationStep(k, 1)) if (GetPlrKnowledge(GetPlayerByIndex(k), def)) break;
      if ((k ?? 0) < (j ?? 0)) continue;
      // Objekt ist gebaut - egal von welchem Spieler
      if(FindObject(def, nil,nil,nil,nil, OCF_Fullcon)) continue;
      // oder Stellvertreterobjekt ist gebaut (z.B. bei Geschütztürmen)
      if(def->~GetAlternativeType())
        if(FindObject(def->~GetAlternativeType(), nil,nil,nil,nil, OCF_Fullcon)) continue;
      //Wenn es kein passendes Objekt gibt, ist das Spielziel nicht erfüllt
      return nil;
      }
    }
  // Kein Bauplan nicht umgesetzt -> erfüllt
  return true;
}

public func IsFulfilledforPlr (plr) {
  var def, i;
  //Alle Baupläne durchgehen
  while (def = GetPlrKnowledge (plr, nil, S42MigrationStep(i, 1, true), C4D_All))
  {
    // Objekt ist gebaut
    if(FindObjectOwner(def, plr, nil, nil, nil, nil, OCF_Fullcon)) continue;
    // oder Stellvertreterobjekt ist gebaut (z.B. bei Geschütztürmen)
    if(def->~GetAlternativeType())
      if(FindObjectOwner(def->~GetAlternativeType(), plr, nil, nil, nil, nil, OCF_Fullcon)) continue;
    //Wenn es kein passendes Objekt gibt, ist das Spielziel nicht erfüllt
    return(nil);
  }
  return (true);
}

public func Activate(act_plr) 
{
  // Alle Baupläne durchgehen...
  var show_all_player_plans = !ObjectCount(RVLR), done, def, i, plr;
  // Geschafft
  if (show_all_player_plans) done = IsFulfilled(); else done = IsFulfilledforPlr(act_plr);
  if (done) return MessageWindow("$done$", act_plr);
  var obj = GetCursor(act_plr), name = GetName();
  CreateMenu(GetID(), obj, this(), nil, name);
  // Im Wettkampfmodus nur nicht umgesetzte Baupläne des eigenen Spielers anzeigen; ansonsten alle Spielerbaupläne prüfen
  if (show_all_player_plans)
    {
    var cnt = GetPlayerCount();
    // Alle nicht umgesetzten Baupläne anzeigen
    for (var j=nil; (j ?? 0)<(cnt ?? 0); S42MigrationStep(j, 1)) if (GetPlayerName(plr = GetPlayerByIndex(j)))
      {    
      i = nil;
      //Alle Baupläne durchgehen
      while (def = GetPlrKnowledge (plr, nil, S42MigrationStep(i, 1, true), C4D_All))
        {
        // Schon gecheckt?
        var k;
        for (k=nil; (k ?? 0)<(j ?? 0); S42MigrationStep(k, 1)) if (GetPlrKnowledge(GetPlayerByIndex(k), def)) break;
        if ((k ?? 0) < (j ?? 0)) continue;
        // Objekt ist gebaut - egal von welchem Spieler
        if(FindObject(def, nil,nil,nil,nil, OCF_Fullcon)) continue;
        // oder Stellvertreterobjekt ist gebaut (z.B. bei Geschütztürmen)
        if(def->~GetAlternativeType())
          if(FindObject(def->~GetAlternativeType(), nil,nil,nil,nil, OCF_Fullcon)) continue;
        // Ansonsten dieses ins Menü
        AddMenuItem(name, "And()", def, obj, 1, nil, Format("$MsgGoalTodo$", GetName(nil, def)));
        }
      }
    }
  else
    {
    // Wettkampfmodus
    i = nil;
    while (def = GetPlrKnowledge(act_plr, nil, S42MigrationStep(i, 1, true), C4D_All))
      {
      //...und ins Menü tun, wenn sie noch fehlen
      if(FindObjectOwner(def, act_plr, nil, nil, nil, nil, OCF_Fullcon)) continue;
      if(def->~GetAlternativeType())
        if(FindObjectOwner(def->~GetAlternativeType(), act_plr, nil, nil, nil, nil, OCF_Fullcon)) continue;
    
      AddMenuItem(GetName(), "And()", def, obj, 1, nil, Format("$MsgGoalTodo$", GetName(nil, def)));
      }
    }
  return true;
}
