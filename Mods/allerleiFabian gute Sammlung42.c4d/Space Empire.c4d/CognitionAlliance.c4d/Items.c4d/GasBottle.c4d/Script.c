/*-- Gasflasche --*/

#strict

protected func Hit()
{
  Sound("MetalHit2");
  return(1);
}

public func Fill() 
{ 
  ChangeDef(CGBF,this()); 
  return(1); 
}

// Eigenschaften
public func IsRefineryProduct() { return(1); }
