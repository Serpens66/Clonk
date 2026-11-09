/*-- Grass --*/

#strict 3


// Kann mittels des neuen Spielzielauswählers
public func IsChooseable2() { return(1); }
// Kann mittels des neuen Spielzielauswählers
public func IsChooseable() { return(1); }

protected func Initialize()
{
  // Eigene Position auf 0,0
  SetPosition();
  // Grass platzieren
  var x_dens = 12;
  for (var x = nil; (x ?? 0) < (LandscapeWidth() ?? 0); S42MigrationAssign(x, "+=", (x_dens ?? 0)))
  {
    var y = GetHorizonHeight(x);
    if (Equal(GetMaterial(x, (y ?? 0) + 1), Material("Earth")))
      CreateGrass(x, y);
  }
  // Steuerungsobjekt bleibt da, damit rückgängigmachen funktioniert!
  return(1);    
}

func Destruction()
{
var pGras;
while(pGras=FindObject(GRA1,nil,nil,nil,nil,nil,nil,nil,nil,pGras))
RemoveObject(pGras);
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
  return(CreateObject(GRA1, x, y, -1));
}
