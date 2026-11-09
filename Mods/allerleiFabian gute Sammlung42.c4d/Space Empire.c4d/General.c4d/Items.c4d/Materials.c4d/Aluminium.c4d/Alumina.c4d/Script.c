/*-- Bauxit --*/

#strict 3

protected func Initialize()
{
  if (Stuck()) SetPosition(GetX(),(GetY() ?? 0)-2);
}

protected func Hit()
{
  Sound("RockHit3");
}
