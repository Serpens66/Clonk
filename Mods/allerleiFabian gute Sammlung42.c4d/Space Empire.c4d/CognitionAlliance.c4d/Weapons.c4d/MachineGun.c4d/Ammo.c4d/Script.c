/*-- MG-Munition --*/

#strict 3


protected func Hit()
{
  Sound("MetalHit1");
  return(1);
}

// Eigenschaften

public func IsRefineryProduct() { return(1); }
