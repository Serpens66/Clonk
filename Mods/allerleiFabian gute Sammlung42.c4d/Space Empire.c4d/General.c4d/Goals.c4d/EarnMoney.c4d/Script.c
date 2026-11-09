/*-- Geldpunkte --*/

#strict 3
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
  S42MigrationAssign(money, "+=", (amount ?? 0));
}

public func IsFulfilled(player)
{
  if (!money)
    money = 100;
  return((GetWealth(player) ?? 0) >= (money ?? 0));
}

protected func Activate(player)
{
  if (IsFulfilled(player))
    return(MessageWindow(Format("%d Geldpunkte erreicht!",money), player));
  MessageWindow(Format("Spielziel: %d Geldpunkte|(%d erreicht)",
                       money, GetWealth(player)), player);
}

