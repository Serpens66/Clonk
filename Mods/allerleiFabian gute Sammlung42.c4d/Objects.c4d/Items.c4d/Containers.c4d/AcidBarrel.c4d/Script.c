/*-- Säurefass --*/

#strict 3

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Acid")); }
public func BarrelMaterialName() { return("Acid"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (Equal(iMaterial, Material("Acid"))) return(ABRL);
  return(_inherited(iMaterial));
}