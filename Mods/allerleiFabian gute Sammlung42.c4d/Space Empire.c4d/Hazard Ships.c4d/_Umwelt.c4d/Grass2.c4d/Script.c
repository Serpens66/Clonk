/*-- Grass --*/

#strict 3

protected func Initialize()
{
  // Eigene Position auf 0,0
  SetPosition();
  // Grass platzieren
  var x_dens = 32;
  for (var x = nil; (x ?? 0) < (LandscapeWidth() ?? 0); S42MigrationAssign(x, "+=", (x_dens ?? 0)))
  {
    var y = GetHorizonHeight(x);
    if (Equal(GetMaterial(x, (y ?? 0) + 1), Material("Earth")))
      CreateGrass(x, y);
	if (Equal(GetMaterial(x, (y ?? 0) + 1), Material("Sand")))
      CreateGrass(x, y);
  }
  // Steuerungsobjekt wird nicht mehr benötigt
  RemoveObject();
  return(1);    
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (And((y ?? 0) < (LandscapeHeight() ?? 0), !GBackSemiSolid(x, y)))
    S42MigrationAssign(y, "+=", 10);
  while (And((y ?? 0) > (LandscapeHeight() ?? 0), GBackSemiSolid(x, y)))
    S42MigrationStep(y, -1);
  return(y);
}
    
private func CreateGrass(x, y)
{
  return(CreateObject(GRA3, x, y, -1));
}
