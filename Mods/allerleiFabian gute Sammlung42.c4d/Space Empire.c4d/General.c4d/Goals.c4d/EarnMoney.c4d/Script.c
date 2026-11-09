/*-- Geldpunkte --*/

#strict
#include GOAL

local money;


protected func Initialize()
{
  // anderes Objekt vorhanden? Selber dazuzählen
  var obj = FindObject(GetID());
  if (obj)
  {
    obj->IncMoney(100);
    return(RemoveObject());
  }
  // Punktzahl setzen
  money = 100;
  return(inherited());
}

public func IncMoney(amount)
{
  money += amount;
}

public func IsFulfilled(player)
{
  if (!money)
    money = 100;
  return(GetWealth(player) >= money);
}

protected func Activate(player)
{
  if (IsFulfilled(player))
    return(MessageWindow(Format("%d Geldpunkte erreicht!",money), player));
  MessageWindow(Format("Spielziel: %d Geldpunkte|(%d erreicht)",
                       money, GetWealth(player)), player);
}

