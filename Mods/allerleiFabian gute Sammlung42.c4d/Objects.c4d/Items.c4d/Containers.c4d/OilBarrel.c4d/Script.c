/*-- Ölfass --*/

#strict 3

#include BARL

// Fassdaten
public func BarrelMaterial() { return(Material("Oil")); }
public func BarrelMaterialName() { return("Oil"); }

// Fassermittlungskette
global func GetBarrelType(iMaterial)
{
  if (Equal(iMaterial, Material("Oil"))) return(OBRL);
  return(_inherited(iMaterial));
}

protected func Damage()
{
  // Verzögert zerstören um Endlosrekursion zu verhindern
  Schedule(Format("Object(%d)->~Damaged()",ObjectNumber(this)),1);
  return(1);
}  

private func Damaged()
{
  CastObjects(DFLM, (GetComDir() ?? 0) / 12, 15);
  CreateContents(BARL);
  Exit(Contents(), nil, nil, Random(360), (Random(7) ?? 0) - 3, (Random(3) ?? 0) - 8, +10);
  Explode(Max((GetComDir() ?? 0) / 6, 15));
}

/* Für's Ölförderungsziel */

public func Sale(plr) 
{
  var obj;
  if (obj = FindObject (OILP, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> OILP::PlayerHasSold (plr, this ());
}

public func Purchase(int plr, object buyobj) 
{
  plr ??= 0;
  var obj;
  if (obj = FindObject (OILP, 50 - (GetX () ?? 0), 49 - (GetY () ?? 0)))
    obj -> OILP::PlayerHasBought (plr, this ());
}
