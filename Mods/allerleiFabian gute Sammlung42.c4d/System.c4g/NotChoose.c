/*-- Regeln, die nicht mit dem Startwähler Settings auswählbar sein sollen --*/

#strict 3
#appendto CTFL
#appendto FGRV
#appendto LSBS
// #appendto WTRS // Zauber im Zauberturm erforschen
#appendto RSTR
#appendto DARK // Düsternis des hazardpacks, wird nicht verwendet

// Hilfsregeln
#appendto NBL2 // Nebelentferner
#appendto VMG2 // Verschmutzungsentferner
#appendto VMG1 // Verschm Erzeuger
#appendto NBL1 // Nebel erzeuger
#appendto NBL4 // Nebel entferner alle X
#appendto NB9Z // Nebel
#appendto VM9Z // Verschmutzung
#appendto WSPE // Windströmungsspeicherobjekt

// angriffsentferner
#appendto AAN_
#appendto AAN8
#appendto AAN9
#appendto AA10
#appendto AA11
#appendto AA12
#appendto AA13
#appendto AA14
#appendto AA15
#appendto AA16
#appendto AA17
#appendto AA18
#appendto AA19
#appendto AA20
#appendto AA21
#appendto AA22
#appendto AA23
#appendto AAN0
#appendto AAN1
#appendto AAN2
#appendto AAN3
#appendto AAN4
#appendto AAN5
#appendto AAN6
#appendto AAN7
// Angriffsregeln
#appendto BERA
#appendto AN01
#appendto AN02
#appendto AN03
#appendto AN04
#appendto AN05
#appendto AN06
#appendto FEAN
#appendto FL_N
#appendto JUMA
#appendto KAMA
#appendto K_AN
#appendto MOAN
#appendto MURA
#appendto PH_A
#appendto SNAN
#appendto VOAN
#appendto AS_N
#appendto CLAN
#appendto HZ_N
#appendto MA_N
#appendto RI_N
#appendto MOA_
#appendto PRA_
#appendto SCA_

// Regeln, die nur als Erklärungstext bei den Variablen Einstellungen gebraucht werden
#appendto ANL_ // AntiLag
#appendto PP3P // ausbildungxp
#appendto PP1P // ausbildungksoten
#appendto PP2P // ausb zeit
#appendto PPT7 // baustoffe bei abriss
#appendto FFF3 // Forschungs anpassen
#appendto PPT8 // Zeitalter
#appendto BBGG // Baustellengoldkosten
#appendto PPGG // Produktion kostet Geld
#appendto SBUI // Baustellenboost
#appendto PRTI // schnellere Produktion
#appendto SN_A // Umweltbelastbarkeit

// Regeln von denen mehrere aktiviert werden koennen und noch nicht variabel sind (diese dann lieber über die regel des erweiterungspacks im spiel oder vor dem spiel zufügen)
#appendto _RAS // Aschregen
#appendto _RIC // Eisregen
#appendto _RLA // LAvaregen
#appendto _ROS // Regen
#appendto _RSS // Säureregen
#appendto _LLS // Blitze
#appendto _CAE // ERdbeben
#appendto _CMS // Meteoriten
#appendto _CAM // Monster
#appendto _CAV // Vulkan
#appendto _ICR // Eishagel
#appendto HLAV // Heisse LAva, funktioniert noch nicht.

func IsNotChooseable() {return(1);}
