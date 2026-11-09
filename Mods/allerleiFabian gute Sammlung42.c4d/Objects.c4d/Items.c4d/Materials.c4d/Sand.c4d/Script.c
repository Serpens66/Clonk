/*-- Sandklumpen --*/

#strict 3

protected func Hit()
{
  CastPXS("Sand", 100, 20);
  RemoveObject();
  return(1);
}