/*-- Siedlungspunkte --*/

#strict 3
#include GOAL

local score;


protected func Initialize()
{
  // anderes Objekt vorhanden? Selber dazuzählen
  var obj = FindObject(GetID());
  if (obj)
  {
    obj->IncScore(100);
    return(RemoveObject());
  }
  // Punktzahl setzen
  score = 100;
  return(inherited());
}

public func IncScore(amount)
{
  S42MigrationAssign(score, "+=", (amount ?? 0));
}

public func IsFulfilled(player)
{
  if (!score)
    score = 100;
  return((GetPlrValueGain(player) ?? 0) >= (score ?? 0));
}

protected func Activate(player)
{
  if (IsFulfilled(player))
    return(MessageWindow(Format("%d Siedlungspunkte erreicht!",score), player));
  MessageWindow(Format("Spielziel: %d Siedlungspunkte|(%d erreicht)",
                       score,GetPlrValueGain(player)), player);
}

