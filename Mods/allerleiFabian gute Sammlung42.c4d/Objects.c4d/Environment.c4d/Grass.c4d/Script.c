/*-- Grass --*/

#strict


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
  for (var x = 0; x < LandscapeWidth(); x += x_dens)
  {
    var y = GetHorizonHeight(x);
    if (GetMaterial(x, y + 1) == Material("Earth"))
      CreateGrass(x, y);
  }
  // Steuerungsobjekt bleibt da, damit rückgängigmachen funktioniert!
  return(1);    
}

func Destruction()
{
var pGras;
while(pGras=FindObject(GRA1,0,0,0,0,0,0,0,0,pGras))
RemoveObject(pGras);
}
       
private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return(y);
}
    
private func CreateGrass(x, y)
{
  return(CreateObject(GRA1, x, y, -1));
}
