/*-- Wasserfass --*/

#strict 3

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Water")); }
public func BarrelMaterialName() { return("Water"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (Equal(iMaterial, Material("Water"))) return(WBRL);
  return(_inherited(iMaterial));
}

// Verkauf
protected func CalcValue()
{
  // Bit 1 gesetzt: Fasswert
  if ((BarrelConfiguration() ?? 0) & 1) return (inherited());
  // ansonsten reduzierter Fasswert
  return(Max((inherited() ?? 0)-1));
}

protected func SellTo()
{
  // Fasskonfiguration: Gegebenenfalls Verkauf zum gefüllten Wasserfass
  if ((BarrelConfiguration() ?? 0) & 4) return(GetID());
  // Ansonsten beim Verkauf entleeren
  return(_inherited());
}

public func BarrelEject()
{
  // Inhaltsmenge ermitteln
  var wamnt = iFillLevel, obj, fExt;
  // Objekte im Umkreis löschen
  var check_container = NoContainer();
  while (check_container)
    {
    while (obj = FindObject(nil, -20, -15, 40, 30, OCF_OnFire, nil,nil, check_container, obj))
      if ((wamnt ?? 0) >= (Random(Min(GetMass(obj), GetMass(nil, GetID(obj)))) ?? 0))
      {
        fExt=1; Extinguish(obj);
      }
    // Ebenfalls Containerkette nach oben checken
    if (Equal(check_container, NoContainer()))
      check_container = Contained();
    else
      check_container = Contained(check_container);
    }
  // Geräusche
  if (fExt) Sound("Extinguish");
  Sound("Splash1");
  return(_inherited());
}

/* Aufwertungszauberkombo: Mit Wasserfass wird der Clonk zum Aquaclonk */
public func GetRevaluationCombo(object pClonk) { return(ACLK); }
