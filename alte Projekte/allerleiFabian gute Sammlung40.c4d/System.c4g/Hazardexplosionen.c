/*-- Hazardexplosionen --*/

// in den Skripten der meisten Dinge, die explodieren, habe ich eingebaut, dass wenn die
// Fkt. IsKeinErdschaden wahr ist, die Dinge nicht normal explodieren, sondern nur eine fakeexplosion
// zusammen mit einem BlastObjects der Größe der ursprünglichen Explosion stattfinden.
// Dadurch bleibt der effekt der Explosion fast derselbe, es wird nur keine Erde dadurch zerstört.
// wenn es wieder normale Explosionen sein sollen, einfach die ID aus der init entfernen.

// in der Initfunktion werden nicht bei allen Dingen der Wert auf 1 gesetzt. Z.b bei Flints und Bomben usw. die tatsächlich dazu gedacht sind, die Erde zu zerstören.

// zusätzlich kann durch die Funktion MachKeinErschaden() die Variable während des Spiels 
// angepasst werden, z.b bei flints und Co die durch eine KI verwendet werden sollen..
// denn ein KI Angriff soll nicht dafür sorgen, dass nichts mehr bebaubar ist

#strict

#appendto SP02 // Splitterflint, wird eher als Waffe, als für Bergbau verwendet -> keinerdschaden
#appendto SB52 // Verfolgerbombe
#appendto JB4K // Jetbelt, wenn man es schmeißt
#appendto ST_3 // Magnetssphäre bei Zerstörung
#appendto AMIN // Alienmine
#appendto P__2 // Alienminensplitter
#appendto PTBL // shot2 vom Partikelblaster
#appendto XARW // Sprengpfeil, standard normale explosion
#appendto FLNT // Flint, standard normale explosion (eine änderung hiervon wirkt sich auch auf alle anderen darauf basierenden flints aus)
#appendto GUNP // Schießpulver bei Feuer, standard normale explosion
#appendto SFLN // SuperFlint, standard normale explosion
#appendto STFN // Super-T-Flint, "
#appendto TFLN // T-Flint, ""
#appendto TETF // Tera-T-Flint, ""
#appendto EFLN // Tera-Flint, ""
#appendto TRP1 // Torpedo, ""
#appendto CEH2 // Wasserstoffblase
#appendto CENM // Energiemodul bei Zerstörung
#appendto CVLA // leicher Jäger bei Zerstörung
#appendto CWBO // Bombe, standard normale Explosion
#appendto SSPR // Sprengsatz
#appendto SBST // Brennstäbe bei Zerstörung
#appendto CAN_ // Cancer bei Zerstörung
#appendto CCD_ // Concordia bei Zerstörung
#appendto CGC_ // Cougar bei Zerstörung
#appendto MAM_ // MAmmoth bei Z
#appendto PRE_ // Predator
#appendto SCR_ // Scorpion
#appendto TSP_ // TEchnosspähre
#appendto TCN_ // Tucan
#appendto ALN4 // Kamikaze Jumper
#appendto LMIN // Lasermine bei zerstörung
#appendto MINE // Mine
#appendto SMI1 // Schwarmmine
#appendto SWRM // Swarmer
#appendto ABMB // Bombe des Airstrikes (skript der panzerkugel)
#appendto DRSU // Supportdrohne bei Zerstörung
#appendto MISS // BazookaRakete
#appendto MIS_ // GebäudeRakete
#appendto HMI_ // Gebäudesuchrakete
#appendto HMIS // Wärmesuchrakete
#appendto IIII // Bunkerraketen

#appendto CGRN // Splittergranate
#appendto CGFR // Splitterteile
#appendto GREN // Granate aus Granatwerfer
#appendto NGRN // Napalmgranate aus granatwerfer
#appendto TBUL // Panzerkugel
#appendto SAM1 // ExplosiveAmmo, erstmal keinerdschaden..
#appendto SAM2 // ClusterAmmo
#appendto SAML // Clusterammoteil
#appendto SAM3 // NuklearAmmo
#appendto STR0 // Wurfrakete
#appendto MROK // Mireok
#appendto SHRF // Schmonk
#appendto EFL0 // NitroFlint, standard normale explosion
#appendto EFL2 // MacroFlint, ""
#appendto EFL3 // TetraFlint, ""
#appendto TRP2 // TetraTorpedo, ""
#appendto BO5B // Bombenobjekt aus Bomb Pack
#appendto GRN2 // Handgranate
#appendto TAUC // UAV-Kontrollstation bei Zerstörung
#appendto TAUV // UAV 
#appendto C40_ // Platiksprengstoff, standardmäßig normal
#appendto TRP4 // Todestorpedo, normal
#appendto SPL1 // Splitter Todestorpedo, normal
#appendto TRP3 // Goldener Torpedo, normal
#appendto EFL6 // RandomFlint
#appendto SFLF // SuperBrandFlint
#appendto SFLI // SuperInfernoFlint
#appendto CBB_ // Blaster-/Ionen-Strahl

#appendto C_S7 // Minirakete des leichten Jägers
#appendto CMM_ // Scavenger (Rakete Cougar)

local keinerdschaden;

// die globalen Standard Variablen in Hash Form, werden im Regelchooser SetExplosivStandard() gesetzt.
// der erste Spieler kann im Kontextmenü die Einstellungen jederzeit ändern
// HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))


func IsErdschaden() {return(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()) ) );}

func IsExplosiv() {return(1);}

func ChangeErdschaden(izahl)
{
HashPut(hashExplosivErdschaden, Format("Erdschaden %v",GetID()), izahl);
}
