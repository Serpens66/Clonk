/*-- Energiezelle --*/

#strict 3

protected func Hit()
{
  Sound("RockHit2");
  return(1);
}

public func Empty()
{
  ChangeDef(CNBE,this());
  return(1);
}

// Eigenschaften
public func IsRefineryProduct() { return(1); }
