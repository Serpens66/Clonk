/*-- Neues Objekt --*/

#strict
#appendto LOAM
#appendto LOA2

private func CanBridgeHere(object clonk)
{
  // Nur wenn der Clonk steht und sich im Freien befindet
  if (GetAction(clonk) ne "Walk" && GetAction(clonk) ne "HoldWalk") return false;
  if (Contained(clonk)) return false;
  // Brücke OK
  return true;
}