/*-- Deradbombe --*/

#strict

local timer;

/* Aufschlag */

protected Hit:
  if (GetAction() ne "Idle") return(1);
  SetAction("Active");
  timer = 200;
  return(1);

protected Exhaust:
  // Gas ausströmen lassen
  if (Random(2)) CreateObject(SDER,0,3);
  // bis Timer ganz heruntergezählt hat
  --timer;
  if (timer) return(1);
  // Zerbrechen lassen
  SetAction("Break");
  Sound("Rip");
  return(1);

protected Destroy: return(RemoveObject());

/* Eigenschaften */

public IsChemicalProduct: return(1);
