/*-- Drain --*/

#strict 3

protected func Initialize()
{
  SetAction("Drain");
  return(1);
}

private func Draining()
{
  if(GetLength(hashSpielRegeln))
  {
      if((RandomX(nil,1000) ?? 0)<=(HashGet(hashSpielRegeln, "Grundwasser") ?? 0)) // mit chance wasser saugen
      if(GBackLiquid())
        ExtractMaterialAmount(nil, nil, GetMaterial(), 20);
  }
  else
  {
      if(!Random(100)) // mit chance wasser saugen 
      if(GBackLiquid())
        ExtractMaterialAmount(nil, nil, GetMaterial(), 20);
  }   
  // "Nachrutschen" 
    if((GetY() ?? 0)>(LandscapeHeight() ?? 0)/4)
      if(!GBackSemiSolid(nil,+3))
        SetPosition(GetX(),(GetY() ?? 0)+1);
  return(1);
}
