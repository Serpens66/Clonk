/*-- MG-Munition --*/

#strict


protected func Hit()
{
  Sound("MetalHit1");
  return(1);
}

// Eigenschaften

public func IsRefineryProduct() { return(1); }
