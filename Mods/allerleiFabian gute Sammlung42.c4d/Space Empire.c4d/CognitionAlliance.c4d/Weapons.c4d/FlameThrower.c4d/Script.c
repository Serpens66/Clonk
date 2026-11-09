/*-- Flammenwerfer --*/

#strict
#include SFTH    // basiert auf SLC-Flammenwerfer


/* Eigenschaften */

private GetFlamesPerShot: return(5);      // SLC: 4;   CoAl: 5
private GetShotsPerAmmo:  return(20);     // SLC: 30;  CoAl: 20
private GetAmmoPerGasCan: return(10);     // SLC: 8;   CoAl: 10

public IsWeaponProduct: return(1);

