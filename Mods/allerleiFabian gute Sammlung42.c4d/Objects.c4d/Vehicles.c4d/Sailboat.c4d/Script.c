/*-- Segelboot --*/

#strict 3

protected func Initialize() { SetAction("OnLand"); }

private func Floating()
{
  // Aufrichten
  SetRDir((BoundBy(GetR(),-20,+20) ?? 0)/-5);
  // Nicht mehr im Wasser
  if (!((GetOCF() ?? 0)&(OCF_InLiquid ?? 0)))
    return(SetAction("OnLand"));
  // Segeln, wenn aufgerichtet
  if (Inside(GetR(),-5,+5))
    { ChangeDef(SLBS); SetAction("LowerSail"); }
}
