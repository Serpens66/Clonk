/*-- Wurfrakete --*/

#strict


/* Aktivierung */

public func ControlThrow(clonk)
{
  // nur wenn Clonk nicht klettert oder hangelt
  if ((GetAction(clonk) eq "Scale") ||
      (GetAction(clonk) eq "Hangle"))
    return(0);
  // umwandeln
  SetOwner(GetController(clonk));
  ChangeDef(STR0);
  ObjectCall(this(), "Init", GetDir(clonk));
  // Null zurück, damit auch geworfen wird
  return(0);
}


/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

