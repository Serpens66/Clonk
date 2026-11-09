/*-- Gewehrmunition --*/

#strict


protected func Hit()
{
  Sound("RockHit3");
  return(1);
}

// Eigenschaften

public func IsRefineryProduct() { return(1); }
