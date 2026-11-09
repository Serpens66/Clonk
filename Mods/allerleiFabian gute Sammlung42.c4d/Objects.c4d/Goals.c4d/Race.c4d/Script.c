/*-- Wettlauf --*/

#strict 3
#include GOAL

local fWon;
local iLeader;
local iTeamCount;
local fTeamTie;

// Funktionen zum Überladen
// 1: links->rechts
// 2: rechts->links
// 3: unten->oben
// 4: oben->unten
public func GetDirection() { var i; if(i = GameCall("GetRACEDirection")) return(i); return(1); }
public func GetStartOffset() { var i; if(i = GameCall("GetRACEStartOffset")) return(i); return(50); }
public func GetEndOffset() { var i; if(i = GameCall("GetRACEEndOffset")) return(i); return(50); }
public func CheckGoal(iPlr)
{
  var i;
  // Achtung! Die Szenarienfunktion CeckRACEGoal sollte -1 zurückgeben,
  // wenn das Zeil nicht erfüllt ist und 1 wenn es erfüllt ist
  if(i = GameCall("CheckRACEGoal", iPlr)) return(i);
  if(Equal(GetDirection(), 1)) return((GetX(GetCursor(iPlr)) ?? 0) > (LandscapeWidth() ?? 0) - (GetEndOffset() ?? 0));
  if(Equal(GetDirection(), 2)) return((GetX(GetCursor(iPlr)) ?? 0) < (GetEndOffset() ?? 0));
  if(Equal(GetDirection(), 3)) return((GetY(GetCursor(iPlr)) ?? 0) < (GetEndOffset() ?? 0));
  if(Equal(GetDirection(), 4)) return((GetY(GetCursor(iPlr)) ?? 0) > (LandscapeHeight() ?? 0) - (GetEndOffset() ?? 0));
}

public func CheckTeamGoal(iTeam)
{
  var iBest, iPlr, iWay;
  // Alle Spieler durchgehen
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    iPlr = GetPlayerByIndex(i);
    // Nur welche vom gleichen Team suchen
    if(!Equal(GetPlayerTeam(iPlr), iTeam)) continue;
    // Hat es schon einer geschafft?
    if(Equal(CheckGoal(iPlr), 1)) return(true);
  }
  return(nil);
}

// Locals: Tode des Spielers
private func & PlayerDeaths(int iPlr) { iPlr ??= 0;
    return(Local((iPlr ?? 0)*2)); }
private func & TeamDeaths(int iTeam)  { iTeam ??= 0;
    return(Local((iTeam ?? 0)*2+1)); }

static const SCOREBOARD_NAME = "Scoreboard";

protected func Initialize()
{
  // Alle Neustart-Objekte entfernen und eines neu erzeugen,
  // damit dieses als erstes in der Liste der Spielregeln auftaucht
  Schedule("RemoveAll(RSTR)", 1);
  Schedule("CreateObject(RSTR, 10, 10, -1)", 2);
  // Scoreboard-Initialisierungen
  SetScoreboardData(SBRD_Caption, SBRD_Caption, SCOREBOARD_NAME, SBRD_Caption);
  SetScoreboardData(SBRD_Caption, ScoreboardCol(RACE), "{{RACE}}", ScoreboardCol(RACE));
  SetScoreboardData(SBRD_Caption, ScoreboardCol(DEAD), "{{SKUL}}", ScoreboardCol(DEAD));
  Schedule("DoScoreboardShow(1)", 1);
  return(true);
}

public func InitializePlayer(int iPlr, int x, int y, object pBase, int iTeam)
{
  // Ist das Spiel schon aus? Dann gleich eliminieren
  iPlr ??= 0;
  x ??= 0;
  y ??= 0;
  iTeam ??= 0;
  if(fWon) return(EliminatePlayer(iPlr));
  // Scoreboard aktualisieren:
  if (iTeam)
  {
    // Höherer Teamindex? Merken
    if((iTeamCount ?? 0) < (iTeam ?? 0)) iTeamCount = iTeam;
    // den Teamnamen eintragen
    SetScoreboardData(iTeam,       SBRD_Caption, GetSmartTeamName(iTeam), iTeam);
    SetScoreboardData(iTeam,       ScoreboardCol(RACE), Format("%d%",GetTeamWayPercent(iTeam)), GetTeamWayPercent(iTeam));
    SetScoreboardData(iTeam,       ScoreboardCol(DEAD), Format("%d",TeamDeaths(iTeam)), TeamDeaths(iTeam));
  }
  else
  {
    // den Spielernamen eintragen
    var iPlrID = GetPlayerID(iPlr);
    SetScoreboardData(iPlrID,       SBRD_Caption, GetTaggedPlayerName(iPlr), iPlrID);
    SetScoreboardData(iPlrID,       ScoreboardCol(RACE),       Format("%d%",nil), nil);
    SetScoreboardData(iPlrID,       ScoreboardCol(DEAD),       Format("%d",nil), nil);
    PlayerDeaths(iPlr) = nil;
  }
  
  // Board anzeigen
  DoScoreboardShow(1, (iPlr ?? 0)+1);

  // Mit keinem Spielern verfeindet sein
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    SetHostility(iPlr, GetPlayerByIndex(i), nil, 1);
    SetHostility(GetPlayerByIndex(i), iPlr, nil, 1);
  }
  return(true);
}

public func RemovePlayer(int iPlr)
{
  // Beim Spielende Anzeigen stehenlassen
  iPlr ??= 0;
  if(fWon) return;
  // Teamanzeige nicht ausleeren
  if(GetPlayerTeam(iPlr)) return;
  // Anzeige leeren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       ScoreboardCol(RACE),       nil ,nil);
  SetScoreboardData(iPlrID,       ScoreboardCol(DEAD),       nil ,nil);
  PlayerDeaths(iPlr) = nil;
  return(true);
}

public func OnClonkDeath(object pClonk)
{
  // Nur wenn der Spieler noch existiert
  if(!GetPlayerName(GetOwner(pClonk))) return;
  // Nur Clonks der Crew zählen
  if(!!SEqual(GetName(pClonk), GetName(nil, GetID(pClonk)))) return;
  var iDeathCount, iTeam = GetPlayerTeam(GetOwner(pClonk));
  if(GetPlayerTeam(GetOwner(pClonk)))
  {
    iDeathCount = S42MigrationStep(TeamDeaths(iTeam), 1);
    SetScoreboardData(iTeam,       ScoreboardCol(DEAD), Format("%d",iDeathCount), iDeathCount);
    return(true);
  }
  var iPlrID = GetPlayerID(GetOwner(pClonk)), iColID = ScoreboardCol(GetID());
  // Todzähler aktualisieren  
  iDeathCount = S42MigrationStep(PlayerDeaths(GetOwner(pClonk)), 1);
  // Scoreboard aktualisieren
  SetScoreboardData(iPlrID,       ScoreboardCol(DEAD),       Format("%d",iDeathCount), iDeathCount);
  return(true);
}

public func GetTeamWayPercent(iTeam)
{
  var iBest, iPlr, iWay;
  // Alle Spieler durchgehen
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    iPlr = GetPlayerByIndex(i);
    // Nur welche vom gleichen Team suchen
    if(!Equal(GetPlayerTeam(iPlr), iTeam)) continue;
    // Den Besten rausfinden
    iWay = GetWayPercent(iPlr);
    if((iWay ?? 0) > (iBest ?? 0)) iBest = iWay;
  }
  return(iBest);
}

public func GetWayPercent(iPlr)
{
  // Wenn das Ziel erfüllt ist 100%
  if(Equal(CheckGoal(iPlr), 1)) return(100);
  // Wenn das Szenarioscript eine eigene Berechnung hat
  var i;
  if(i = GameCall("GetRACEWayPercent", iPlr)) return(BoundBy(i, nil, 99));
  // Ansonsten je nach Wegstrecke 0%-99%
  if(Equal(GetDirection(), 1))
    return(BoundBy(((GetX(GetCursor(iPlr)) ?? 0)-(GetStartOffset() ?? 0))*100/((LandscapeWidth() ?? 0)-(GetEndOffset() ?? 0)-(GetStartOffset() ?? 0)), nil, 99));
  if(Equal(GetDirection(), 2))
    return(BoundBy(((LandscapeWidth() ?? 0)-(GetX(GetCursor(iPlr)) ?? 0)-(GetStartOffset() ?? 0))*100/((LandscapeWidth() ?? 0)-(GetEndOffset() ?? 0)-(GetStartOffset() ?? 0)), nil, 99));
  if(Equal(GetDirection(), 3))
    return(BoundBy(((LandscapeHeight() ?? 0)-(GetY(GetCursor(iPlr)) ?? 0)-(GetStartOffset() ?? 0))*100/((LandscapeHeight() ?? 0)-(GetEndOffset() ?? 0)-(GetStartOffset() ?? 0)), nil, 99));
  if(Equal(GetDirection(), 4))
    return(BoundBy(((GetY(GetCursor(iPlr)) ?? 0)-(GetStartOffset() ?? 0))*100/((LandscapeHeight() ?? 0)-(GetEndOffset() ?? 0)-(GetStartOffset() ?? 0)), nil, 99));
}

public func UpdateScoreboard()
{
  if(fWon) return;
  var iPlr, iWay, iOldWay;
  // Bei Teamplay nur die Teams eintragen
  if(iTeamCount)
  {
    for(var iTeam = 1; (iTeam ?? 0) <= (iTeamCount ?? 0); S42MigrationStep(iTeam, 1, true))
    {
      iWay = GetTeamWayPercent(iTeam);
      // Weitestes Team finden
      if(Or(!iOldWay, (iOldWay ?? 0)<(iWay ?? 0)))
      {
        iOldWay = iWay;
        iLeader = iTeam;
      }
      // Daten eintragen
      SetScoreboardData(iTeam,       ScoreboardCol(RACE),       Format("%3d%", iWay), iWay);
      SetScoreboardData(iTeam,       ScoreboardCol(DEAD),       Format("%d", TeamDeaths(iTeam)), TeamDeaths(iTeam));
      // Hat schon jemand gewonnen?
      if(Equal(CheckTeamGoal(iTeam), 1))
      {
        // Mit Wettstreit gewinnt nur der im Ziel
        if(ObjectCount(RVLR)) EliminatePlayers();
        else fWon = GetPlayerCount();
      }
    }
  }
  else
  {
    // alle Spieler durchgehen
    for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
    {
      iPlr = GetPlayerByIndex(i);
      // Keinen Cursor? Dann ist er bestimmt noch in der Teamauswahl
      if(!GetCursor(iPlr)) continue;
      iWay = GetWayPercent(iPlr);
      // Weitesten Spieler finden
      if(Or(!iOldWay, (iOldWay ?? 0)<(iWay ?? 0)))
      {
        iOldWay = iWay;
        iLeader = iPlr;
      }
      // Daten eintragen
      SetScoreboardData(GetPlayerID(iPlr),       ScoreboardCol(RACE),       Format("%3d%", iWay), iWay);
      SetScoreboardData(GetPlayerID(iPlr),       ScoreboardCol(DEAD),       Format("%d", PlayerDeaths(iPlr)), PlayerDeaths(iPlr));
      // Hat schon jemand gewonnen?
      if(Equal(CheckGoal(iPlr), 1))
      {
        // Mit Wettstreit gewinnt nur der im Ziel
        if(ObjectCount(RVLR)) EliminatePlayers();
        else fWon = GetPlayerCount();
      }
    }
  }
  // Sortieren primaer nach Fortschritt; sekundaer nach Toden
  SortScoreboard(ScoreboardCol(DEAD), nil);
  SortScoreboard(ScoreboardCol(RACE), true);
  return(true);
}

public func EliminatePlayers()
{
  var iPlr, szWinners, fHasWon;
  // alle Spieler durchgehen
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    iPlr = GetPlayerByIndex(i);
    if(iTeamCount) fHasWon = CheckTeamGoal(GetPlayerTeam(iPlr));
    else fHasWon = CheckGoal(iPlr);
    if(!Equal(fHasWon, 1))
      EliminatePlayer(iPlr);
    else 
    {
      S42MigrationStep(fWon, 1, true);
      if(iTeamCount) if(!Equal(GetPlayerTeam(iPlr), iLeader)) fTeamTie = 1;
      if(!szWinners) szWinners = Format("%s", GetPlayerName(iPlr));
      else szWinners = Format("%s, %s", szWinners, GetPlayerName(iPlr));
    }
  }
  if(iTeamCount)
  {
    if(fTeamTie) Message ("$MsgTie$", nil);
    else Message ("$MsgTeamWinner$", nil, GetSmartTeamName(iLeader), szWinners);
  }
  else Message ("$MsgWinner$", nil, szWinners);
}

func IsFulfilled () {
  if(!fWon) return;
  // Das Ziel ist erfüllt, wenn alle Spieler die verloren haben eliminiert sind
  // denn sonst könnte es passieren, das das GameOver vor der Eliminierung kommt
  return((GetPlayerCount() ?? 0)<=(fWon ?? 0));
}

func Activate (iPlr) {
  // Nachricht nach Erfüllung ausgeben
  if(And(iTeamCount, fWon))
  {
    if(fTeamTie) return(MessageWindow ("$MsgTie$", iPlr));
    else if (Equal(iLeader, GetPlayerTeam(iPlr))) return(MessageWindow ("$MsgYourTeamWon$", iPlr));
    else return(MessageWindow (Format("$MsgOtherTeamWon$", GetSmartTeamName(iLeader)), iPlr));
  }
  else if(Equal(fWon, 1))
  {
    if (Equal(iLeader, iPlr)) return(MessageWindow ("$MsgYouWon$", iPlr));
    else return(MessageWindow (Format("$MsgOtherWon$", iLeader), iPlr));
  }
  else if((fWon ?? 0)>1) return(MessageWindow ("$MsgTie$", iPlr));
  return (MessageWindow (GetFulfillText (iPlr), iPlr));
}

func GetFulfillText (int iPlayer) {
  iPlayer ??= 0;
  if(iTeamCount)
  {
    // Kein Team?!
    if(!iLeader) return("");
    // Fortschritt zeigen
    if (Equal(iLeader, GetPlayerTeam(iPlayer))) return(Format("$MsgYourTeamAhead$", GetTeamWayPercent(iLeader)));
    return(Format("$MsgOtherTeamAhead$",GetSmartTeamName(iLeader),GetTeamWayPercent(iLeader),GetTeamWayPercent(GetPlayerTeam(iPlayer))));
  }
  // kein Clonk im Spiel??!
  if (!GetPlayerName(iLeader)) return("");
  // Fortschritt zeigen
  if (Equal(iLeader, iPlayer)) return(Format("$MsgYouAhead$", GetWayPercent(iLeader)));
  return(Format("$MsgOtherAhead$",GetName(GetCursor(iLeader)),GetPlayerName(iLeader),GetWayPercent(iLeader),GetWayPercent(iPlayer)));
}

// Gibt für unbenannte Teams den Spieleranamen aus
private func GetSmartTeamName(int iTeam)
  {
  iTeam ??= 0;
  var sTeamName;
  if (!GetTeamConfig(TEAM_AutoGenerateTeams))
    {
    // Vom Szenario vorgegebene Teams: Original-Teamnamen benutzen
    sTeamName = GetTeamName(iTeam);
    }
  else
    {
    // Automatische Teams: Diese haben keinen ordentlichen Namen. Aus erstem Mitgliedsnamen zusammenbauen
    for (var i, cnt=GetPlayerCount(); (i ?? 0)<(cnt ?? 0); S42MigrationStep(i, 1))
      {
      var iPlr = GetPlayerByIndex(i);
      if (Equal(GetPlayerTeam(iPlr), iTeam))
        {
        // Teamname als Spielername; gefärbt wenn nicht ohnehin nach Team gefärbt wird
        var sTeamPlrName;
        if (GetTeamConfig(TEAM_TeamColors))
          sTeamPlrName = GetPlayerName(iPlr);
        else
          sTeamPlrName = GetTaggedPlayerName(iPlr);
        if (!sTeamName) sTeamName = sTeamPlrName; else sTeamName = Format("%s, %s", sTeamName, sTeamPlrName);
        }
      }
    }
  // Bei Teamfarben auch diese einbeziehen
  if (GetTeamConfig(TEAM_TeamColors))
    return Format("<c %x>%s</c>", GetTeamColor(iTeam), sTeamName);
  else
    return sTeamName;
  }
