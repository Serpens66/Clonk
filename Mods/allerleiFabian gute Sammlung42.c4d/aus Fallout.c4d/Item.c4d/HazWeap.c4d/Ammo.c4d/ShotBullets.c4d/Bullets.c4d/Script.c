#strict

public func IsAmmo()  { return(true); }
public func MaxAmmo(pObj) {
//Bauarbeiter können vor Stufe 15 keine Muni aufnehmen, danach nur mit Gürtel ein wenig (alle werte ca durch 6)
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()<15) return(0);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmo13()) return(9);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmo14()) return(12);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmoAllrounder1()) return(7);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmoAllrounder2()) return(8);
// falls noch kein Ergebnis, dann kein Gürtel an:
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15) return(0);

if(pObj->MoreAmmo13()) return(54);   // Gürtel klein für Munition  1,5mal soviel
if(pObj->MoreAmmo14()) return(72);  // Gürtel groß für Munition  doppelt soviel
if(pObj->MoreAmmoAllrounder1()) return(42);  // Gürtel klein allrounder  x + ((1/6)*x
if(pObj->MoreAmmoAllrounder2()) return(48);  // Gürtel groß für allrounder  x + ((1/3)*x
 return(36); }