/*-- Granatenpack --*/

#strict 3
#include STAP

protected func Hit()
{
//if(!Random(10)) Incinerate();  //erstmal wieder deaktiviert, weil neue Muntionsfabrik die Objekte auf den Boden schmeiﬂt...
 _inherited();
}

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(GRAM); }
public func AmmoCount()    { return(6); }

// public func IsChemicalProduct() { return(1); }  // isweaponproduct in system
// public func IsRefineryProduct() { return(1); }