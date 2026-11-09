/*-- Ölförderung --*/

#strict 3
#include OREM

public func IsFulfilled () {
  var OilCnt = GetMaterialCount (Material ("Oil"));
  if (Equal(OilCnt, -1)) return ;
  if ((OilCnt ?? 0) >= 100) return ;
  return (!ObjectCount (OBRL));
}

public func PlayerHasSold (plr, obj) {     
  if (Equal(GetID (obj), OBRL)) S42MigrationAssign(Local (plr), "+=", (obj -> OBRL::CalcValue() ?? 0));
} 

protected func Activate (plr) {
  if (IsFulfilled ()) return (MessageWindow ("$done$", plr));
  // Fördermenge noch unbekannt
  if (Equal(GetMaterialCount(Material("Oil")), -1))
    return(MessageWindow(Format("$todo2$", ObjectCount(OBRL)), plr));
  // +100, damit ab 100 eine Zahl ungleich 0 angezeigt wird
  MessageWindow(Format("$todo$", ((GetMaterialCount(Material("Oil")) ?? 0) + 100) / 200, ObjectCount(OBRL)), plr);
}
