#strict 3
#include STAP

protected func Hit()
{
//if(!Random(10)) Incinerate();  //erstmal wieder deaktiviert, weil neue Muntionsfabrik die Objekte auf den Boden schmeiﬂt...
 _inherited();
}

public func IsAmmoPacket() { return(true); }
public func AmmoID()       { return(MIAM); }
public func AmmoCount()    { return(4); }

// public func IsChemicalProduct() { return(1); }
// public func IsRefineryProduct() { return(1); }