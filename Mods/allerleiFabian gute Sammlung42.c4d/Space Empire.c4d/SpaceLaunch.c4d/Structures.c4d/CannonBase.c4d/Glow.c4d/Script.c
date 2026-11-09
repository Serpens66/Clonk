/*-- Glüh-Effekt --*/

#strict

local base;


/* Initialisierung */

public func Init(par_base)
{
  // wird von der Batterie aufgerufen
  base = par_base;
  SetAction("Glowing", base);
  return(UpdateGlow());
}


/* Timer-Call */

private func UpdateGlow()
{
  // Glühen neu einstellen
  var alpha = 255 - base->GetFill();
  SetColorDw((GetColorDw(base) & 16777215) + (alpha << 24));
  return(1);
}

