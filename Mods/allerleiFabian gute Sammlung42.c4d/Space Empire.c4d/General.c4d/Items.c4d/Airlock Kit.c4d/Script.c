/*--- Bausatz ---*/

#strict

protected func Initialize()
{
  return(1);
}

protected func Hit()
{
  Sound("RockHit1");
  return(1);
}

protected func Activate()
{
  // Besitzer setzen für CreateConstruction
  SetOwner(GetOwner(Contained()));
  // Baustelle erschaffen
  CreateConstruction(AIAL, 0, 16, GetOwner(Contained()), 1, 0,0);
  // Bausatz verbrauchen
  RemoveObject();
  return(1);
}


/* Produkteigenschaften */

public func IsRefineryProduct() { return(1); }
public func IsAnvilProduct() { return(1); }
