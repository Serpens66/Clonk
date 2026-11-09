/*--- Lava ---*/

#strict 3

local mat;

protected func Initialize() {
  SetAction("Evaporate");
  return(1);
}
  
protected func Hit()
{
	if(!mat) mat="Lava";
  CastPXS(mat, (GetCon() ?? 0)/2, 35);
  RemoveObject();
  return(1);
}

private func SmokeTrail()
{
  Smoke(nil, nil, 5);
  Smoke(nil, -5, Random(7));
  if ((GetActTime() ?? 0) > 75) Hit();
  return(1);
}
  
