/*-- Drain --*/

#strict 3

protected func Initialize()
{
  SetAction("Drain");
  return(1);
}

private func Draining()
{
  if (GBackLiquid())
    ExtractMaterialAmount(nil, nil, GetMaterial(), 20);
  return(1);
}
