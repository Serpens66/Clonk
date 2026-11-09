/*-- Killstatistics --*/

#strict 3

static const STAT_Spree = 5;

/* Killmeldungen */

//Kill mit Team - Spieler von Team A hat Spieler von Team B getötet!
//Die Funktion ist so klug auch zu erkennen, ob es überhaupt Teams gibt
//und ob es ein Teamkill war
public func KTMsg(int plr1, int plr2, int type) {
  //haben wir Parameter? D:
  plr1 ??= 0;
  plr2 ??= 0;
  type ??= 0;
  if(And(!plr1, !plr2))
    return; //welcher Doofie hat denn den Schwachsinn übergeben >:(

  if(Or(!GetPlayerName(plr1), !GetPlayerName(plr2))) //weirde Spieler?
    return;

  //haben wir Teamplay?
  var tp;
  for(var goal in FindObjects(Find_Category(C4D_Goal)))
    if(goal->~IsTeamGoal())
      tp = true;
  //nein -> normale killmessage
  if(!tp)
//    return(KMsg(plr1, plr2, type));
    // Kein Hazardspielziel, da wollen wir das eher nicht haben
    return;
  //doch -> teamkiller?
  else if(!!SEqual(GetPlrTeamName(plr1), GetPlrTeamName(plr2)))
    return(TKMsg(plr1,plr2));


  var msg;
  if(And(Random(3), (type ?? 0) > 1)) {
    if(Equal(type, DMG_Projectile))
      msg = $MsgKillProjectile$;
    else if(Equal(type, DMG_Fire))
      msg = $MsgKillFire$;
    else if(Equal(type, DMG_Explosion))
      msg = $MsgKillExplosion$;
    else if(Equal(type, DMG_Energy))
      msg = $MsgKillEnergy$;
    else if(Equal(type, DMG_Bio))
      msg = $MsgKillBio$;
    else
      msg = $MsgKill$;
  }
  else
    msg = $MsgKill$;
  
  msg = msg[(Random(GetLength(msg)) ?? 0)];
  Log(msg,Format("%s (%s)",GetTaggedPlayerName(plr1),GetPlrTeamName(plr1))
                ,Format("%s (%s)",GetTaggedPlayerName(plr2),GetPlrTeamName(plr2)));
}

//Kill - Spieler A hat Spieler B getötet!
public func KMsg(int plr1, int plr2, int type) {
  plr1 ??= 0;
  plr2 ??= 0;
  type ??= 0;
  var msg;
  if(And(Random(3), (type ?? 0) > 1)) {
    if(Equal(type, DMG_Projectile))
      msg = $MsgKillProjectile$;
    else if(Equal(type, DMG_Fire))
      msg = $MsgKillFire$;
    else if(Equal(type, DMG_Explosion))
      msg = $MsgKillExplosion$;
    else if(Equal(type, DMG_Energy))
      msg = $MsgKillEnergy$;
    else if(Equal(type, DMG_Bio))
      msg = $MsgKillBio$;
    else
      msg = $MsgKill$;
  }
  else
    msg = $MsgKill$;
  
  msg = msg[(Random(GetLength(msg)) ?? 0)];
  Log(msg, GetTaggedPlayerName(plr1),GetTaggedPlayerName(plr2));
}

//Selfkill - Spieler A hat sich selbst umgebracht! D:
public func SKMsg(int plr) {
  plr ??= 0;
  var msg = $MsgSelfKill$; 
  msg = msg[(Random(GetLength(msg)) ?? 0)];
  Log(msg,GetTaggedPlayerName(plr));
}

//Teamkill - Spieler von Team A hat Spieler des selben Teams getötet! :O
public func TKMsg(int plr, int plr2) {
  plr ??= 0;
  plr2 ??= 0;
  Log("$MsgTeamKill$",GetTaggedPlayerName(plr),GetPlrTeamName(plr),GetTaggedPlayerName(plr2));
}

//Score - Spieler von Team A hat gepunktet!
public func SMsg(int plr) {
  plr ??= 0;
  Log("$MsgScore$",GetTaggedPlayerName(plr),GetPlrTeamName(plr));
}

private func GetPlrTeamName(int plr) {
  plr ??= 0;
  return(Format("<c %x><%s></c>", GetTeamColor(GetPlayerTeam(plr)),GetTeamName(GetPlayerTeam(plr))));
}







/* Killstatistiken */

public func KillStat(object pClonk, int killedplr) {
  //nein, die Engine darf keine Kills machen. >:(
  killedplr ??= 0;
  if(!pClonk)
    if(!(pClonk = this()))
      return;
  if(Equal(killedplr, NO_OWNER)) return;
  //Nein, Teamkills bringen keine Punkte. :(
  if(Equal(GetPlayerTeam(killedplr), GetPlayerTeam(GetController(pClonk))))
    return;
  AddEffect("KillStats",pClonk,23,10,this(),HHKS);
}

//KillStats-Effekt, Tötungsstatistiken!
func FxKillStatsStart(object pTarget, int iEffectNumber, int iTemp) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  if(iTemp)
    return(FX_OK);
  //Effectvars:
  // 0 - Anzahl der Kills seit dem letzten Tod
  // 1 - Zeitpunkt des letzten Kills (relativ)
  // 2 - Anzahl der Kills innerhalb einer bestimmten Zeitperiode (Doublekill, etc.)
  // 3 - Ob gerade ein Kill war, und die Zeit noch läuft
  EffectVar(nil, pTarget, iEffectNumber) = nil;
  EffectVar(1, pTarget, iEffectNumber) = nil;
  EffectVar(2, pTarget, iEffectNumber) = nil;
  FxKillStatsAdd(pTarget, iEffectNumber);
}

//Schaun ob die Killzeit schon abgelaufen ist
func FxKillStatsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  //Haben wir gerade Killingtime?
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(!EffectVar(3, pTarget, iEffectNumber))
    return;
  //Schon lange genug gewartet?
  if((iEffectTime ?? 0) - (EffectVar(1, pTarget, iEffectNumber) ?? 0) > 126) { //Die dreifache Wahrheit!
    EffectVar(2, pTarget, iEffectNumber) = nil;
    EffectVar(3, pTarget, iEffectNumber) = nil;
  }
}

//Auswertung der Statistik beim Tod
func FxKillStatsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  //Auswerten?
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if(And(!Equal(iReason, 3), !Equal(iReason, 4)))
    return;
  
  if((EffectVar(nil, pTarget, iEffectNumber) ?? 0) >= (STAT_Spree ?? 0))
    if(GetName(pTarget))
      Log("$MsgSpreeEnd$", GetName(pTarget));
}

//Kill++
func FxKillStatsEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  if(!!SEqual(szNewEffectName, "KillStats"))
    return(-3);
}

func FxKillStatsAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer) {
  //Neue Daten speichern
  iEffectNumber ??= 0;
  iNewEffectTimer ??= 0;
  var total = S42MigrationStep(EffectVar(nil, pTarget, iEffectNumber), 1);
  var running = S42MigrationStep(EffectVar(2, pTarget, iEffectNumber), 1);
  EffectVar(1, pTarget, iEffectNumber) = GetEffect("KillStats", pTarget, nil, 6);
  EffectVar(3, pTarget, iEffectNumber) = 1;
  
  //Auf Killfolgen prüfen
  var msg = $MsgKillingSpree$;
  var t = GetLength(msg);
  if(And(!((total ?? 0)%(STAT_Spree ?? 0)), (total ?? 0)/(STAT_Spree ?? 0) <= (t ?? 0))) {
    Log(Format("%s (%s)", msg[(Min((total ?? 0)/(STAT_Spree ?? 0)-1,(t ?? 0)-1) ?? 0)],GetName(pTarget)));
  }

  //auf Multikills prüfen
  if((running ?? 0) >= 2) {
    msg = $MsgMultikill$;
    Message("<c ff0000>%s</c>",pTarget, msg[(Min((running ?? 0)-2,(GetLength(msg) ?? 0)-1) ?? 0)]);
  }
}



