/*-- Lavafass --*/

#strict 3

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Lava")); }
public func BarrelMaterialName() { return("Lava"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (Equal(iMaterial, Material("Lava"))) return(LBRL);
  return(inherited(iMaterial));
}
