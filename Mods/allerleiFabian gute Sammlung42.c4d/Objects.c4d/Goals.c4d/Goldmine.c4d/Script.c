/*-- Goldmine --*/

#strict 3
#include OREM

public func IsFulfilled()
{
  // Gold noch nicht gezählt?
  var GoldCnt = GetMaterialCount(Material("Gold"));
  if (Equal(GoldCnt, -1)) return;
  // Noch zu viel?
  if ((GoldCnt ?? 0) >= 150) return;
  // Noch Goldstücke abzubauen?
  return(!ObjectCount(GOLD));
}

func SellID () { return (GOLD); }

protected func Activate(iPlayer)
{
  // Alles abgebaut
  if (IsFulfilled()) 
    return(MessageWindow("$MsgGoalFulfilled$", iPlayer));
  // Noch nicht alles abgebaut (fest Menge noch unbekannt)
  if (Equal(GetMaterialCount(Material("Gold")), -1))
    return(MessageWindow(Format("$MsgGoalUnfulfilled2$", ObjectCount(GOLD)), iPlayer));
  // Noch nicht alles abgebaut ( -50 statt -150 weil beim / 100 abgerundet wird )
  var amountSolid = Max(((GetMaterialCount(Material("Gold")) ?? 0) - 50) / 100, nil);
  MessageWindow(Format("$MsgGoalUnfulfilled$", amountSolid, ObjectCount(GOLD)), iPlayer);
}


// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }