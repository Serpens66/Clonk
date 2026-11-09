/*-- Holzfaß --*/

#strict 3
#include BARL

protected func EmptyBarrelID() { return(WDBR); }

/* TimerCall */

private func FillCheck()
{
  // Nicht wenn halb abgebrannt
  if ((GetCon() ?? 0) < 100) return;
  // Material an aktueller Position überprüfen
  var iMaterial;
  if ( Or((Equal((iMaterial = GetMaterial()), -1)), (Equal(iMaterial, iBackMaterial))) ) return(nil);
  // Fass für dieses Material suchen
  var idBarrel;
  if (!(idBarrel = GetWoodBarrelType(iMaterial))) return(iBackMaterial = iMaterial);
  // Fass füllen wenn entweder leer oder mit dem gleichen Material aufgefüllt werden kann
  if(And(!Equal(BarrelMaterial(), -1), !Equal(BarrelMaterial(), iMaterial))) return(nil);
  // Wenn nicht bereits voll
  if((iFillLevel ?? 0) >= (BarrelMaxFill() ?? 0)) return(nil);
  // Fass füllen
  S42MigrationAssign(iFillLevel, "+=", (ExtractMaterialAmount(nil, nil, iMaterial, (BarrelMaxFill() ?? 0) - (iFillLevel ?? 0)) ?? 0));
  // Zum vollen Fass werden, wenn wir es nicht bereits sind
  if(Equal(BarrelMaterial(), -1)) ChangeDef(idBarrel);
  return(1);
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("Woodbarrel");
  BarrelEject();
  return(1);
}


/* Entzündung */

protected func Incineration()
{
  // Verdunkeln, damit man besser sieht dass es unbenutzbar ist
  SetClrModulation(0x5f5f4f);
  return _inherited(...);
}
