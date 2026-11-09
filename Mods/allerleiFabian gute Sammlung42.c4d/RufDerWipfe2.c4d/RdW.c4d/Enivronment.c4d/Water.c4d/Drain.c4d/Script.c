/*-- Drain --*/

#strict

protected func Initialize()
{
  SetAction("Drain");
  return(1);
}

private func Draining()
{
  if(GetLength(hashSpielRegeln))
  {
      if(RandomX(0,1000)<=HashGet(hashSpielRegeln, "Grundwasser")) // mit chance wasser saugen 
      if(GBackLiquid())
        ExtractMaterialAmount(0, 0, GetMaterial(), 20);
  }
  else
  {
      if(!Random(100)) // mit chance wasser saugen 
      if(GBackLiquid())
        ExtractMaterialAmount(0, 0, GetMaterial(), 20);
  }   
  // "Nachrutschen" 
    if(GetY()>LandscapeHeight()/4)
      if(!GBackSemiSolid(0,+3))
        SetPosition(GetX(),GetY()+1);
  return(1);
}
