#strict

public func IsAmmo()  { return(true); }
public func MaxAmmo(pObj) {
//Bauarbeiter können vor Stufe 15 keine Muni aufnehmen, danach nur mit Gürtel ein wenig (alle werte ca durch 6)
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()<15) return(0);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmo5()) return(8);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmo6()) return(10);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmoAllrounder1()) return(6);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmoAllrounder2()) return(7);
// falls noch kein Ergebnis, dann kein Gürtel an:
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15) return(0);

if(pObj->MoreAmmo5()) return(45);   // Gürtel klein für Munition
if(pObj->MoreAmmo6()) return(60);  // Gürtel groß für Munition
if(pObj->MoreAmmoAllrounder1()) return(35);  // Gürtel klein allrounder
if(pObj->MoreAmmoAllrounder2()) return(40);  // Gürtel groß für allrounder
return(30); }


