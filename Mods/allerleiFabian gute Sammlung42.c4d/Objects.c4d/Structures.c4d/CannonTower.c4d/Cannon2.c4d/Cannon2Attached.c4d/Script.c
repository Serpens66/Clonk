/*--- Artilleriegeschütz (angeschraubt) ---*/

#strict 3

#include CTW2

/* Einige andere Eigenschaften */

protected func RotationSpeed() { return(1); }
protected func CannonMobileID() { return(CTW5); }
protected func CannonAmmo(object obj) { return(Or(((GetOCF(obj) ?? 0) & (OCF_Collectible ?? 0)), ((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0))) ); }
protected func CannonPowderNeeded(object obj) { return(2 + 2 * ((!Equal(((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0)), nil)) ?? 0) ); }
protected func CannonPower(object obj) { return(20 - 2 * ((!Equal(((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0)), nil)) ?? 0) ); }
protected func CannotShootMenuName(object obj) { return Format("$TxtShootsn$", obj->GetName(), CannonPowderNeeded(obj)); }

