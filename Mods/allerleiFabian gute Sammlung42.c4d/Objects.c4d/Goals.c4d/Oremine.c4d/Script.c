/*-- Erzabbau --*/

#strict 3
#include GOAL

local Sold;

public func IsFulfilled()
{
  // Erz noch nicht gezählt?
  var OreCnt = GetMaterialCount(Material("Ore"));
  if (Equal(OreCnt, -1)) return;
  // noch zu viel?
  if ((OreCnt ?? 0) >= 150) return;
  // noch Erzstücke abzubauen?
  return(!ObjectCount(ORE1));
}

public func IsFulfilledforPlr (plr)
{
  if (!IsFulfilled()) return ;
  var i = GetPlayerCount();
  while (S42MigrationStep(i, -1, true))
    if ((Local (GetPlayerByIndex (i)) ?? 0) > (Local (plr) ?? 0)) return (nil);
  return (1);
}

func SellID () { return (ORE1); }

public func PlayerHasSold (plr, obj)
{
  if (Equal(GetID (obj), SellID ()))
  {
    S42MigrationStep(Local (plr), 1);
    S42MigrationStep(Sold, 1);
  }
}

public func PlayerHasBought (plr, obj)
{
  if (Equal(GetID (obj), SellID ()))
  {
    S42MigrationStep(Local (plr), -1);
    S42MigrationStep(Sold, -1);
  }
}

protected func Activate(iPlayer)
{
  // Alles abgebaut
  if (IsFulfilled()) 
    return(MessageWindow("$MsgGoalFulfilled$", iPlayer));
  // Noch nicht alles abgebaut (fest Menge noch unbekannt)
  if (Equal(GetMaterialCount(Material("Ore")), -1))
    return(MessageWindow(Format("$MsgGoalUnfulfilled2$", ObjectCount(ORE1)), iPlayer));
  // Noch nicht alles abgebaut ( -50 statt -150 weil beim / 100 abgerundet wird )
  MessageWindow(Format("$MsgGoalUnfulfilled$", (Max((GetMaterialCount(Material("Ore")) ?? 0) - 50, nil) ?? 0) / 100, ObjectCount(ORE1)), iPlayer);
}
