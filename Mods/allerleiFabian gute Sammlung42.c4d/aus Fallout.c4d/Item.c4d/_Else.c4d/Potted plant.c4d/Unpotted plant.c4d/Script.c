/*--- Holz ---*/

#strict

local type;

protected func Hit()
{
  Sound("WoodHit*");
  CastPXS("Earth", 20, 20);  
  RemoveObject();
  return(1);
}

protected func Plant() {
  if (Random(6)) return(0);
  if(MaterialName(GetMaterial(0,10)) eq "Sulphur") 
  {
    ExtractMaterialAmount(0,10,Material("Sulphur"),40);
    ExtractMaterialAmount(0,15,Material("Sulphur"),40);
    ExtractMaterialAmount(0,20,Material("Sulphur"),40);
    ChangeDef(EO8Z);
    return();
  }
  var rand = RandomX(1,3);
  if (rand == 1) CreateConstruction (TRE1, 0, 10, -1, 20);
  if (rand == 2) CreateConstruction (TRE2, 0, 10, -1, 20);
  if (rand == 3) CreateConstruction (TRE3, 0, 10, -1, 20);
  RemoveObject();
}
