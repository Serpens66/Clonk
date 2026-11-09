/*-- Militärgebäude --*/

// Militärgebäude werden ab und an von der KI angegriffen
// alle anderen Gebäude werden verschont

#strict 3

// Gebäde
#appendto CDFT // Abwehrturm
#appendto CGDT  // Wachturm Cognition
#appendto SCNB // Geschützplattform
#appendto SLTW // Laserturm
#appendto SMBS // hautbasis, kann mit waffe ausgrüstet werden
#appendto STOW // Wachturm Spacelaunch
#appendto CBBS // Kampfbasis
#appendto FLAK
#appendto PRC1 // Partikelgeschütz

// Fahrzeuge die betreten werden
// können mit IsHazardShip() gefunden werden, dürfen dann aber nicht Controller -1 haben, dann werden sie auch angegriffen

func IsMilitaryBuilding() { if((GetOCF() ?? 0) & (OCF_Fullcon ?? 0)) return(1); return(nil);}








