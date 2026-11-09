/*-- Holzwasserfass --*/

#strict
#include WDBR

// Fassdaten
public func BarrelMaterial() { return(Material("Water")); }
public func BarrelMaterialName() { return("Water"); }

// Fassermittlungskette
global func GetWoodBarrelType(iMaterial)
{
  if (iMaterial == Material("Water")) return(WDWB);
  return(_inherited(iMaterial));
}

/* Aufschlag */ 

public func BarrelEject()
{
  // Inhaltsmenge ermitteln
  var wamnt = iFillLevel, obj, fExt;
  // Objekte im Umkreis löschen
  while (obj = FindObject(0, -20, -15, 40, 30, OCF_OnFire(), 0,0, NoContainer(), obj))
    if (Random(wamnt*3) > GetMass(obj))
    {
      fExt=1; Extinguish(obj);
    }
  // Geräusche
  if (fExt) Sound("Extinguish");
  Sound("Splash1");
  return(_inherited());
}

/* Entzündung */

protected func Incineration()
{
  // Verdunkeln, damit man besser sieht dass es unbenutzbar ist
  SetClrModulation(0x5f5f4f);
  return _inherited(...);
}


/* Schaden */
protected func Damage(int iChange)
{
  if(iChange > 10)      // Kaputt
  {
    BarrelEject();      // Leer
    Incinerate(this()); // Feuer
  }
}
