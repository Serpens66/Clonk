/*-- Glüh-Effekt --*/

#strict 3

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
  var alpha = 255 - (base->GetFill() ?? 0);
  SetColorDw(((GetColorDw(base) ?? 0) & 16777215) + ((alpha ?? 0) << 24));
  return(1);
}

