/*-- Lavafass --*/

#strict

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Lava")); }
public func BarrelMaterialName() { return("Lava"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (iMaterial == Material("Lava")) return(LBRL);
  return(inherited(iMaterial));
}
