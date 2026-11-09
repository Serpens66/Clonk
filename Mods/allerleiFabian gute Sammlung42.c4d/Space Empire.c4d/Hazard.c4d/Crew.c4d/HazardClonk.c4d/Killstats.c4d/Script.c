/*-- Killstatistics --*/

#strict

static const STAT_Spree = 5;

/* Killmeldungen */

//Kill mit Team - Spieler von Team A hat Spieler von Team B getötet!
//Die Funktion ist so klug auch zu erkennen, ob es überhaupt Teams gibt
//und ob es ein Teamkill war
public func KTMsg(int plr1, int plr2, int type) {
  //haben wir Parameter? D:
  if(!plr1 && !plr2)
    return(); //welcher Doofie hat denn den Schwachsinn übergeben >:(

  if(!GetPlayerName(plr1) || !GetPlayerName(plr2)) //weirde Spieler?
    return();

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
  else if(GetPlrTeamName(plr1) S= GetPlrTeamName(plr2))
    return(TKMsg(plr1,plr2));


  var msg;
  if(Random(3) && type > 1) {
    if(type == DMG_Projectile)
      msg = $MsgKillProjectile$;
    else if(type == DMG_Fire)
      msg = $MsgKillFire$;
    else if(type == DMG_Explosion)
      msg = $MsgKillExplosion$;
    else if(type == DMG_Energy)
      msg = $MsgKillEnergy$;
    else if(type == DMG_Bio)
      msg = $MsgKillBio$;
    else
      msg = $MsgKill$;
  }
  else
    msg = $MsgKill$;
  
  msg = msg[Random(GetLength(msg))];
  Log(msg,Format("%s (%s)",GetTaggedPlayerName(plr1),GetPlrTeamName(plr1))
                ,Format("%s (%s)",GetTaggedPlayerName(plr2),GetPlrTeamName(plr2)));
}

//Kill - Spieler A hat Spieler B getötet!
public func KMsg(int plr1, int plr2, int type) {
  var msg;
  if(Random(3) && type > 1) {
    if(type == DMG_Projectile)
      msg = $MsgKillProjectile$;
    else if(type == DMG_Fire)
      msg = $MsgKillFire$;
    else if(type == DMG_Explosion)
      msg = $MsgKillExplosion$;
    else if(type == DMG_Energy)
      msg = $MsgKillEnergy$;
    else if(type == DMG_Bio)
      msg = $MsgKillBio$;
    else
      msg = $MsgKill$;
  }
  else
    msg = $MsgKill$;
  
  msg = msg[Random(GetLength(msg))];
  Log(msg, GetTaggedPlayerName(plr1),GetTaggedPlayerName(plr2));
}

//Selfkill - Spieler A hat sich selbst umgebracht! D:
public func SKMsg(int plr) {
  var msg = $MsgSelfKill$; 
  msg = msg[Random(GetLength(msg))];
  Log(msg,GetTaggedPlayerName(plr));
}

//Teamkill - Spieler von Team A hat Spieler des selben Teams getötet! :O
public func TKMsg(int plr, int plr2) {
  Log("$MsgTeamKill$",GetTaggedPlayerName(plr),GetPlrTeamName(plr),GetTaggedPlayerName(plr2));
}

//Score - Spieler von Team A hat gepunktet!
public func SMsg(int plr) {
  Log("$MsgScore$",GetTaggedPlayerName(plr),GetPlrTeamName(plr));
}

private func GetPlrTeamName(int plr) {
  return(Format("<c %x><%s></c>", GetTeamColor(GetPlayerTeam(plr)),GetTeamName(GetPlayerTeam(plr))));
}







/* Killstatistiken */

public func KillStat(object pClonk, int killedplr) {
  //nein, die Engine darf keine Kills machen. >:(
  if(!pClonk)
    if(!(pClonk = this()))
      return();
  if(killedplr == NO_OWNER) return();
  //Nein, Teamkills bringen keine Punkte. :(
  if(GetPlayerTeam(killedplr) == GetPlayerTeam(GetController(pClonk)))
    return();
  AddEffect("KillStats",pClonk,23,10,this(),HHKS);
}

//KillStats-Effekt, Tötungsstatistiken!
func FxKillStatsStart(object pTarget, int iEffectNumber, int iTemp) {
  if(iTemp)
    return(FX_OK);
  //Effectvars:
  // 0 - Anzahl der Kills seit dem letzten Tod
  // 1 - Zeitpunkt des letzten Kills (relativ)
  // 2 - Anzahl der Kills innerhalb einer bestimmten Zeitperiode (Doublekill, etc.)
  // 3 - Ob gerade ein Kill war, und die Zeit noch läuft
  EffectVar(0, pTarget, iEffectNumber) = 0;
  EffectVar(1, pTarget, iEffectNumber) = 0;
  EffectVar(2, pTarget, iEffectNumber) = 0;
  FxKillStatsAdd(pTarget, iEffectNumber);
}

//Schaun ob die Killzeit schon abgelaufen ist
func FxKillStatsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  //Haben wir gerade Killingtime?
  if(!EffectVar(3, pTarget, iEffectNumber))
    return();
  //Schon lange genug gewartet?
  if(iEffectTime - EffectVar(1, pTarget, iEffectNumber) > 126) { //Die dreifache Wahrheit! 
    EffectVar(2, pTarget, iEffectNumber) = 0;
    EffectVar(3, pTarget, iEffectNumber) = 0;
  }
}

//Auswertung der Statistik beim Tod
func FxKillStatsStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  //Auswerten?
  if(iReason != 3 && iReason != 4)
    return();
  
  if(EffectVar(0, pTarget, iEffectNumber) >= STAT_Spree)
    if(GetName(pTarget))
      Log("$MsgSpreeEnd$", GetName(pTarget));
}

//Kill++
func FxKillStatsEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName S= "KillStats")
    return(-3);
}

func FxKillStatsAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer) {
  //Neue Daten speichern
  var total = ++EffectVar(0, pTarget, iEffectNumber);
  var running = ++EffectVar(2, pTarget, iEffectNumber);
  EffectVar(1, pTarget, iEffectNumber) = GetEffect("KillStats", pTarget, 0, 6);
  EffectVar(3, pTarget, iEffectNumber) = 1;
  
  //Auf Killfolgen prüfen
  var msg = $MsgKillingSpree$;
  var t = GetLength(msg);
  if(!(total%STAT_Spree) && total/STAT_Spree <= t) {
    Log(Format("%s (%s)", msg[Min(total/STAT_Spree-1,t-1)],GetName(pTarget)));
  }

  //auf Multikills prüfen
  if(running >= 2) {
    msg = $MsgMultikill$;
    Message("<c ff0000>%s</c>",pTarget, msg[Min(running-2,GetLength(msg)-1)]);
  }
}



