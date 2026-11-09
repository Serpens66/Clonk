/*-- Bauxit --*/

#strict

protected func Initialize()
{
  if (Stuck()) SetPosition(GetX(),GetY()-2);
}

protected func Hit()
{
  Sound("RockHit3");
}
