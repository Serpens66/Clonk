#strict
#include _SKA

public func UnpackTo() { return(CHMU); }

protected func Hit()
{
  Sound("ClonkHit*");
  return(1);
}
