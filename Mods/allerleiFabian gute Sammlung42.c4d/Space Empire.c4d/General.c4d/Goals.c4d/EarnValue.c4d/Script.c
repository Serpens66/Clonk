/*-- Siedlungspunkte --*/

#strict
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
  score += amount;
}

public func IsFulfilled(player)
{
  if (!score)
    score = 100;
  return(GetPlrValueGain(player) >= score);
}

protected func Activate(player)
{
  if (IsFulfilled(player))
    return(MessageWindow(Format("%d Siedlungspunkte erreicht!",score), player));
  MessageWindow(Format("Spielziel: %d Siedlungspunkte|(%d erreicht)",
                       score,GetPlrValueGain(player)), player);
}

