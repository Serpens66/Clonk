#strict 3

public func IsAmmo()  { return(true); }
public func MaxAmmo(pObj) { 
//Bauarbeiter können vor Stufe 15 keine Muni aufnehmen, danach nur mit Gürtel ein wenig (alle werte ca durch 6)
if(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)<15)) return(nil);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmo3())) return(75);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmo4())) return(100);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmoAllrounder1())) return(60);
if(And(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15), pObj->MoreAmmoAllrounder2())) return(70);
// falls noch kein Ergebnis, dann kein Gürtel an:
if(And(pObj->~IstBauarbeiter(), (pObj->~AktuelleStufe() ?? 0)>=15)) return(nil);

if(pObj->MoreAmmo3()) return(450);   // Gürtel klein für Munition
if(pObj->MoreAmmo4()) return(600);  // Gürtel groß für Munition
if(pObj->MoreAmmoAllrounder1()) return(350);  // Gürtel klein allrounder
if(pObj->MoreAmmoAllrounder2()) return(400);  // Gürtel groß für allrounder
return(300); }