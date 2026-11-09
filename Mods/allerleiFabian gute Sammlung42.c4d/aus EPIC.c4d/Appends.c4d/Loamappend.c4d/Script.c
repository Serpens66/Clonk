/*-- Neues Objekt --*/

#strict 3
#appendto LOAM
#appendto LOA2

private func CanBridgeHere(object clonk)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (And(!SEqual(GetAction(clonk), "Walk"), !SEqual(GetAction(clonk), "HoldWalk"))) return nil;
  if (Contained(clonk)) return nil;
  // Brücke OK
  return true;
}