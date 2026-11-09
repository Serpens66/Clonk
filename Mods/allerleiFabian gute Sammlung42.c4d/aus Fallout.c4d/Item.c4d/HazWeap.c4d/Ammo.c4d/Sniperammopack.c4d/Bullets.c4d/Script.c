#strict 3

public func IsAmmo()  { return(true); }
public func MaxAmmo(pObj) {
//Bauarbeiter können vor Stufe 15 keine Muni aufnehmen, danach nur mit Gürtel ein wenig (alle werte ca durch 6)
if(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)<15)) return(nil);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmo15())) return(3);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmo16())) return(4);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmoAllrounder1())) return(1);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmoAllrounder2())) return(2);
// falls noch kein Ergebnis, dann kein Gürtel an:
if(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15)) return(nil);

if(pObj->MoreAmmo15()) return(18);   // Gürtel klein für Munition  1,5mal soviel
if(pObj->MoreAmmo16()) return(24);  // Gürtel groß für Munition  doppelt soviel
if(pObj->MoreAmmoAllrounder1()) return(14);  // Gürtel klein allrounder  x + ((1/6)*x
if(pObj->MoreAmmoAllrounder2()) return(16);  // Gürtel groß für allrounder  x + ((1/3)*x
 return(12); }

