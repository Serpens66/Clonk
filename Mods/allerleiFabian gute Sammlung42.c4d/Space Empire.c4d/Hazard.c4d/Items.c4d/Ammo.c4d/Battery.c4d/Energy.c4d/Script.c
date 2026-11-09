#strict

public func IsAmmo()  { return(true); }
public func MaxAmmo(pObj) { 
//Bauarbeiter können vor Stufe 15 keine Muni aufnehmen, danach nur mit Gürtel ein wenig (alle werte ca durch 6)
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()<15) return(0);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmo3()) return(75);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmo4()) return(100);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmoAllrounder1()) return(60);
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15 && pObj->MoreAmmoAllrounder2()) return(70);
// falls noch kein Ergebnis, dann kein Gürtel an:
if(pObj->~IstBauarbeiter() && pObj->~AktuelleStufe()>=15) return(0);

if(pObj->MoreAmmo3()) return(450);   // Gürtel klein für Munition
if(pObj->MoreAmmo4()) return(600);  // Gürtel groß für Munition
if(pObj->MoreAmmoAllrounder1()) return(350);  // Gürtel klein allrounder
if(pObj->MoreAmmoAllrounder2()) return(400);  // Gürtel groß für allrounder
return(300); }