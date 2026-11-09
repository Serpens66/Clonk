/*-- leere Energiezelle --*/

#strict 3

protected func Hit()
{
  Sound("RockHit2");
  return(1);
}

public func Fill()
{
  ChangeDef(CNBF,this());
  return(1);
}
