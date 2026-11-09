/*-- Lavafass --*/

#strict 3

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("DuroLava")); }
public func BarrelMaterialName() { return("DuroLava"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (Equal(iMaterial, Material("DuroLava"))) return(DBRL);
  return(inherited(iMaterial));
}
