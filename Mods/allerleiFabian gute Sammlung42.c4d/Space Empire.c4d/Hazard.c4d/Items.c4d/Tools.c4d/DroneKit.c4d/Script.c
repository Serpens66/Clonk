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
  CreateContents(DRSU,Contained(),1);
  RemoveObject();
  return(1);
}


/* Produkteigenschaften */

public func IsRefineryProduct() { return(1); }