#strict 3

public func IsAmmo()  { return(true); }
public func MaxAmmo(pObj) {
//Bauarbeiter können vor Stufe 15 keine Muni aufnehmen, danach nur mit Gürtel ein wenig (alle werte ca durch 6)
if(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)<15)) return(nil);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmo5())) return(8);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmo6())) return(10);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmoAllrounder1())) return(6);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmoAllrounder2())) return(7);
// falls noch kein Ergebnis, dann kein Gürtel an:
if(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15)) return(nil);

if(pObj->MoreAmmo5()) return(45);   // Gürtel klein für Munition
if(pObj->MoreAmmo6()) return(60);  // Gürtel groß für Munition
if(pObj->MoreAmmoAllrounder1()) return(35);  // Gürtel klein allrounder
if(pObj->MoreAmmoAllrounder2()) return(40);  // Gürtel groß für allrounder
return(30); }


