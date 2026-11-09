/*-- Wurfrakete --*/

#strict 3


/* Aktivierung */

public func ControlThrow(clonk)
{
  // nur wenn Clonk nicht klettert oder hangelt
  if (Or((!!SEqual(GetAction(clonk), "Scale")),
      (!!SEqual(GetAction(clonk), "Hangle"))))
    return(nil);
  // umwandeln
  SetOwner(GetController(clonk));
  ChangeDef(STR0);
  ObjectCall(this(), "Init", GetDir(clonk));
  // Null zurück, damit auch geworfen wird
  return(nil);
}


/* Eigenschaften */

public func IsRefineryProduct() { return(1); }

