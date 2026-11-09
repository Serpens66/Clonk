/*--- Holz ---*/

#strict 3

local type;

protected func Hit()
{
  Sound("WoodHit*");
  CastPXS("Earth", 20, 20);  
  RemoveObject();
  return(1);
}

protected func Plant() {
  if (Random(6)) return(nil);
  if(!!SEqual(MaterialName(GetMaterial(nil,10)), "Sulphur"))
  {
    ExtractMaterialAmount(nil,10,Material("Sulphur"),40);
    ExtractMaterialAmount(nil,15,Material("Sulphur"),40);
    ExtractMaterialAmount(nil,20,Material("Sulphur"),40);
    ChangeDef(EO8Z);
    return;
  }
  var rand = RandomX(1,3);
  if (Equal(rand, 1)) CreateConstruction (TRE1, nil, 10, -1, 20);
  if (Equal(rand, 2)) CreateConstruction (TRE2, nil, 10, -1, 20);
  if (Equal(rand, 3)) CreateConstruction (TRE3, nil, 10, -1, 20);
  RemoveObject();
}
