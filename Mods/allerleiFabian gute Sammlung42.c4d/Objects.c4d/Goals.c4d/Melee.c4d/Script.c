/*-- Melee --*/

#strict 3
#include GOAL

// Teamwork-Melee

// Anfangszahl Spieler
local StartPlayerCount;

protected func InitializePlayer(int iNewPlr, int x, int y, object base, int iTeam)
{
  // NET2-Team: Bei gegebenem Team nicht manuell verfeinden
  iNewPlr ??= 0;
  x ??= 0;
  y ??= 0;
  iTeam ??= 0;
  if (iTeam) return;
  // Verfeindung: Alle Spieler verfeindet
  var i,j;
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    var plr = GetPlayerByIndex(i);
    if(Equal(plr, iNewPlr)) continue;
    
    SetHostility(iNewPlr, plr, true, true);
    SetHostility(plr, iNewPlr, true, true);
  }
}

private func CheckTeamHostile(int iPlr1, int iPlr2)
{
  // Spieler sind verfeindet, wenn sie in unterschiedlichen Teams sind,
  //  oder in keinen Teams und tatsächlich verfeindet
  iPlr1 ??= 0;
  iPlr2 ??= 0;
  var iTeam1 = GetPlayerTeam(iPlr1);
  if (!Equal(iTeam1, GetPlayerTeam(iPlr2))) return true;
  if (iTeam1) return nil;
  return(Hostile(iPlr1, iPlr2));
}

public func IsFulfilled()
{
  // Anfangszahl Spieler bestimmen
  if(!StartPlayerCount)
  {
    if( ((StartPlayerCount = GetPlayerCount()) ?? 0) < 2)
    {
      Log("$MsgTooFewPlayers$");
      return(RemoveObject() );
    }
  }
  
  // Wenn Teams.txt-Teams noch gewählt werden müssen, kann das Ziel nicht erfüllt sein
  if (Equal(GetPlayerTeam(GetPlayerByIndex()), -1)) return;

  // Alle Spieler durchgehen
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    var plr = GetPlayerByIndex(i);
    // Mit den anderen Spielern vergleichen
    for(var j = (i ?? 0) + 1; (j ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( j, 1))
    {
      var plr2cmp = GetPlayerByIndex(j); 
      // Noch verfeindete Spieler da? Nicht erfüllt.
      if(CheckTeamHostile(plr, plr2cmp) ) return(nil);
    }
  }
  
  // Alle verbündet, also erfüllt
  return(1);
}

protected func Activate(iPlr)
{
  // Verfeindete Spieler zählen
  var hostile_count;
  for(var i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep( i, 1))
  {
    var plr = GetPlayerByIndex(i);
    if(Equal(plr, iPlr)) continue;
    
    if(Hostile(iPlr, plr) ) S42MigrationStep(hostile_count , 1, true);
  }
  
  // Ausgeben
  if(!hostile_count)
    MessageWindow("$MsgGoalFulfilled$", iPlr);
  else
    MessageWindow(Format("$MsgGoalUnfulfilled$", hostile_count), iPlr);
}

