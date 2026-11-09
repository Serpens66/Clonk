/*-- Flammenwerfer --*/

#strict 3
#include SFTH    // basiert auf SLC-Flammenwerfer


/* Eigenschaften */

private func GetFlamesPerShot() { return(5);      // SLC: 4;   CoAl: 5
}
private func GetShotsPerAmmo() {  return(20);     // SLC: 30;  CoAl: 20
}
private func GetAmmoPerGasCan() { return(10);     // SLC: 8;   CoAl: 10
}

public func IsWeaponProduct() { return(1);
}

