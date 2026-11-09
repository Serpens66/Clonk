/*-- Deradbombe --*/

#strict 3

local timer;

/* Aufschlag */

protected func Hit(...) {
  if (!SEqual(GetAction(), "Idle")) return(1);
  SetAction("Active");
  timer = 200;
  return(1);
}

protected func Exhaust(...) {
  // Gas ausströmen lassen
  if (Random(2)) CreateObject(SDER,nil,3);
  // bis Timer ganz heruntergezählt hat
  S42MigrationStep(timer, -1);
  if (timer) return(1);
  // Zerbrechen lassen
  SetAction("Break");
  Sound("Rip");
  return(1);
}

protected func Destroy(...) { return(RemoveObject());
}

/* Eigenschaften */

public func IsChemicalProduct(...) { return(1);
}
