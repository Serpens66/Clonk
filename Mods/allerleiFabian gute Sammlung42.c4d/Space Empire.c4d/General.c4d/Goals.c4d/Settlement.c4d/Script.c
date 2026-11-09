/*-- Siedlung kooperativ --*/

#strict
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
  score += amount;
}

private func GetCoopValue()
{
  // Siedlungspunkte zusammenzählen
  var pts, player, i;
  for (i = 0; i < GetPlayerCount(); i++)
  {
    player = GetPlayerByIndex(i);
    // Spieler nicht vorhanden?
    if (!GetPlayerName(player))
      continue;
    // dazu addieren
    pts += GetPlrValueGain(player);
  }
  return(pts);
}

public func IsFulfilled()
{
  if (!score)
    score = 250;
  return(GetCoopValue() >= score);
}

protected func Activate(player)
{
  if (IsFulfilled())
    return(MessageWindow(Format("%d Siedlungspunkte erreicht!",score), player));
  MessageWindow(Format("Spielziel: alle zusammen %d Siedlungspunkte|(%d erreicht)",
                       score,GetCoopValue()), player);
}

