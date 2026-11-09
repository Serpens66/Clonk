/*-- Siedlung kooperativ --*/

#strict 3
#include GOAL

local score;


protected func Initialize()
{
  // anderes Objekt vorhanden? Selber dazuzählen
  var obj = FindObject(GetID());
  if (obj)
  {
    ObjectCall(obj, "IncScore", 500);
    return(RemoveObject());
  }
  // Punktzahl setzen
  score = 500;
  return(inherited());
}

public func IncScore(amount)
{
  S42MigrationAssign(score, "+=", (amount ?? 0));
}

private func GetCoopValue()
{
  // Siedlungspunkte zusammenzählen
  var pts, player, i;
  for (i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1, true))
  {
    player = GetPlayerByIndex(i);
    // Spieler nicht vorhanden?
    if (!GetPlayerName(player))
      continue;
    // dazu addieren
    S42MigrationAssign(pts, "+=", (GetPlrValueGain(player) ?? 0));
  }
  return(pts);
}

public func IsFulfilled()
{
  if (!score)
    score = 250;
  return((GetCoopValue() ?? 0) >= (score ?? 0));
}

protected func Activate(player)
{
  if (IsFulfilled())
    return(MessageWindow(Format("%d Siedlungspunkte erreicht!",score), player));
  MessageWindow(Format("Spielziel: alle zusammen %d Siedlungspunkte|(%d erreicht)",
                       score,GetCoopValue()), player);
}

