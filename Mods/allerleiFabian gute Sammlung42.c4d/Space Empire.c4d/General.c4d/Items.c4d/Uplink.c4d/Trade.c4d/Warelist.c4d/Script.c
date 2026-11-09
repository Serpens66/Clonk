/*-- Warenliste --*/

#strict 3

local s1, s2, s3, s4, s5;   // Nummer in der Liste der jeweils letzten Ware einer Kategorie
local listsize;             // Größe der gesamten Handelswarenliste

// die nummerierten Locals enthalten die Waren-IDs


/* Initialisierung */

protected func Initialize()
{
  // Handelslisten in Locals speichern
  var i = -1;

// Materialien
  Local(S42MigrationStep(i, 1)) = ERTH;
  Local(S42MigrationStep(i, 1)) = ROCK;
  Local(S42MigrationStep(i, 1)) = LOAM;
  Local(S42MigrationStep(i, 1)) = WOOD;
  Local(S42MigrationStep(i, 1)) = GOLD;
  Local(S42MigrationStep(i, 1)) = CTIT;
  Local(S42MigrationStep(i, 1)) = CALU;
  Local(S42MigrationStep(i, 1)) = METL;
  Local(S42MigrationStep(i, 1)) = SURO;
  Local(S42MigrationStep(i, 1)) = CRYS;
  Local(S42MigrationStep(i, 1)) = CTIO;
  Local(S42MigrationStep(i, 1)) = CALO;
  Local(S42MigrationStep(i, 1)) = ORE1;
  Local(S42MigrationStep(i, 1)) = COAL;
  Local(S42MigrationStep(i, 1)) = SPHR;
  Local(S42MigrationStep(i, 1)) = ANML;  //Schmiedeeisen
  Local(S42MigrationStep(i, 1)) = PSTC;   //Plastik
  Local(S42MigrationStep(i, 1)) = ASHS; //Asche
  Local(S42MigrationStep(i, 1)) = ICE1;
  Local(S42MigrationStep(i, 1)) = SAND; //Sand
  s1 = i;

  // Grundausrüstung
  Local(S42MigrationStep(i, 1)) = CNKT; //Bausatz
  Local(S42MigrationStep(i, 1)) = FK9Z;  //Fackel
  Local(S42MigrationStep(i, 1)) = LNKT;
  Local(S42MigrationStep(i, 1)) = CNCR;      //Beton
  Local(S42MigrationStep(i, 1)) = LIQG;      //Flüssiggranit
  Local(S42MigrationStep(i, 1)) = TENP; //Ritterzelt gepackt
  Local(S42MigrationStep(i, 1)) = WDBR; //Holzfass
  Local(S42MigrationStep(i, 1)) = BARL; //Metallfass
  Local(S42MigrationStep(i, 1)) = AM5P; //Fernrohr
  Local(S42MigrationStep(i, 1)) = BETL; //Beutel
  Local(S42MigrationStep(i, 1)) = SPKT; //Sperrenbausatz kleine Fahrstühle
  Local(S42MigrationStep(i, 1)) = GV5Y;  //Gangverschluss
  Local(S42MigrationStep(i, 1)) = FLAG;
  Local(S42MigrationStep(i, 1)) = SAK1; //Sack
  Local(S42MigrationStep(i, 1)) = PNCR; //Profibeton
  Local(S42MigrationStep(i, 1)) = TB5B;  //SFT Zeltpaket
  Local(S42MigrationStep(i, 1)) = NLGT; //Knicklicht
  Local(S42MigrationStep(i, 1)) = FLSH; //Taschenlampe
  Local(S42MigrationStep(i, 1)) = AILK; //Airlock Bausatz
  Local(S42MigrationStep(i, 1)) = UNCR; //Ultimativer beton
  Local(S42MigrationStep(i, 1)) = TOKT; //Werkzeugkasten
  Local(S42MigrationStep(i, 1)) = UPLI;  //Uplink
  Local(S42MigrationStep(i, 1)) = CS4K;  //Sensorenzubehöhr 8 stück
  Local(S42MigrationStep(i, 1)) = PS4K;
  Local(S42MigrationStep(i, 1)) = BS4K;
  Local(S42MigrationStep(i, 1)) = LS4K;
  Local(S42MigrationStep(i, 1)) = SS4K;
  Local(S42MigrationStep(i, 1)) = WS4K;
  Local(S42MigrationStep(i, 1)) = GTE1;
  Local(S42MigrationStep(i, 1)) = KC4K;
  Local(S42MigrationStep(i, 1)) = CGBE; //Hydrogen Bottle
  Local(S42MigrationStep(i, 1)) = KI_T;//Kiste
  Local(S42MigrationStep(i, 1)) = SBST; //Isotope
  Local(S42MigrationStep(i, 1)) = IMTK;//verb. Werkzeugkasten
  Local(S42MigrationStep(i, 1)) = ABRL; //verschieden gefüllte Fässer
  Local(S42MigrationStep(i, 1)) = LBRL;
  Local(S42MigrationStep(i, 1)) = OBRL;
  Local(S42MigrationStep(i, 1)) = WBRL;
  Local(S42MigrationStep(i, 1)) = WDWB; //HolzWasserfass
/* 
  Local(++i) = PLUT; //Plutonium
  Local(++i) = PLU1;
  Local(++i) = PLU2;
  Local(++i) = SHLK;  //Schlacke
  Local(++i) = _SKA;  //Schlackenkasten (testen ob Menge funktioiert -> funktioniert nicht deswegen erstmal rauslassen)
  Local(++i) = CHMU;  //weiterer Müll ++
  Local(++i) = _CHF;
  Local(++i) = HMWS;
  Local(++i) = _MSK;
*/
  Local(S42MigrationStep(i, 1)) = AMWS;  //Atommüll,Metallschrott,Leiche und Yellow Cake haben keine gepackte Version, also ist okay hinzufügen
  Local(S42MigrationStep(i, 1)) = YECK;
  Local(S42MigrationStep(i, 1)) = CORP;
  Local(S42MigrationStep(i, 1)) = MTWS;

  s2 = i;



  // spezielle Ausrüstung Munition
  Local(S42MigrationStep(i, 1)) = _Y3B; //früher Feuerschlöscher
  Local(S42MigrationStep(i, 1)) = JPTH; //Springstein
  Local(S42MigrationStep(i, 1)) = ARWP; //Pfeile
  Local(S42MigrationStep(i, 1)) = FARP; //Feuerpfeile
  Local(S42MigrationStep(i, 1)) = TAAZ;  // Tauscheranzug
  Local(S42MigrationStep(i, 1)) = THLM; //Taucherhelm
  Local(S42MigrationStep(i, 1)) = XARP;  //Sprengpfeile
  Local(S42MigrationStep(i, 1)) = DK8Z;  //Frogspring
  Local(S42MigrationStep(i, 1)) = E18Z;  //Air-Elevator
  Local(S42MigrationStep(i, 1)) = E78Z; //frogspring2
  Local(S42MigrationStep(i, 1)) = GUNP; //schießpulver
  Local(S42MigrationStep(i, 1)) = ADM1; // Munitionspack Geschützturm
  Local(S42MigrationStep(i, 1)) = ADM3; // Batterie
  Local(S42MigrationStep(i, 1)) = WC_0; // 4 Waffenbauteile
  Local(S42MigrationStep(i, 1)) = WC_1;
  Local(S42MigrationStep(i, 1)) = WC_2;
  Local(S42MigrationStep(i, 1)) = WC_3;
  Local(S42MigrationStep(i, 1)) = GRAP; //Granatenmunitionskiste
  Local(S42MigrationStep(i, 1)) = STAP;//Munitionskiste
  Local(S42MigrationStep(i, 1)) = AIRT;//Atemflasche
  Local(S42MigrationStep(i, 1)) = MEDI; //Medipack
  Local(S42MigrationStep(i, 1)) = ENAP; //Engergiezelle munition
  Local(S42MigrationStep(i, 1)) = SDBM; //Deradbombe
  Local(S42MigrationStep(i, 1)) = PTAP; //Partikelbehälter (munition)
  Local(S42MigrationStep(i, 1)) = S_RB; //5 Einfache Rüstungen
  Local(S42MigrationStep(i, 1)) = S_LJ;
  Local(S42MigrationStep(i, 1)) = S_MA;
  Local(S42MigrationStep(i, 1)) = S_L2;
  Local(S42MigrationStep(i, 1)) = S_CA;
  Local(S42MigrationStep(i, 1)) = STLM;  //better Bullets munition
  Local(S42MigrationStep(i, 1)) = SHMU; //SchrotMunition
  Local(S42MigrationStep(i, 1)) = SNI_; //Snipermunition
  Local(S42MigrationStep(i, 1)) = GSAP; // Benzinkanister (Munition)
  Local(S42MigrationStep(i, 1)) = JTPK; //Jetpack
  Local(S42MigrationStep(i, 1)) = MIAP; //Raketen Munition
  Local(S42MigrationStep(i, 1)) = HARM; //Rüstung
  Local(S42MigrationStep(i, 1)) = HSHD; //Schildgenerator
  Local(S42MigrationStep(i, 1)) = SPC1; //Explosiv Siege Ammo
  Local(S42MigrationStep(i, 1)) = _SKT; //Schildupgrade
  Local(S42MigrationStep(i, 1)) = S_TA; //Tesla Rüstung
  Local(S42MigrationStep(i, 1)) = PNGU; //Pinguinanzug
  Local(S42MigrationStep(i, 1)) = SPC2; //Cluster siege Ammo
  Local(S42MigrationStep(i, 1)) = _Y4C;//Schutzanzug
  Local(S42MigrationStep(i, 1)) = S_C2; //verstärkte Rüstungen 4 stück
  Local(S42MigrationStep(i, 1)) = S_LA;
  Local(S42MigrationStep(i, 1)) = S_M2;
  Local(S42MigrationStep(i, 1)) = S_CJ;
  Local(S42MigrationStep(i, 1)) = GU01; //die kleinen Munitionsgürtel (10 stück)
  Local(S42MigrationStep(i, 1)) = GU03;
  Local(S42MigrationStep(i, 1)) = GU05;
  Local(S42MigrationStep(i, 1)) = GU07;
  Local(S42MigrationStep(i, 1)) = GU09;
  Local(S42MigrationStep(i, 1)) = GU11;
  Local(S42MigrationStep(i, 1)) = GU13;
  Local(S42MigrationStep(i, 1)) = GU15;
  Local(S42MigrationStep(i, 1)) = GU17;
  Local(S42MigrationStep(i, 1)) = GU19;
  Local(S42MigrationStep(i, 1)) = SPC4; //Toxic Siege Ammo
  Local(S42MigrationStep(i, 1)) = SPC3;//Nuclear Siege Ammo
  Local(S42MigrationStep(i, 1)) = JB4K; //Jetbelt
  Local(S42MigrationStep(i, 1)) = S_BA; //SpezialRüstung
  Local(S42MigrationStep(i, 1)) = SREP; //Repair Naniten
  Local(S42MigrationStep(i, 1)) = KLAS; //LaserUpgrade
  Local(S42MigrationStep(i, 1)) = KSLM; //Bio Upgrade
  Local(S42MigrationStep(i, 1)) = KRFL; //WAffenteil Upgrade
  Local(S42MigrationStep(i, 1)) = STRH; //Partikelkonverter (Upgrade)
  Local(S42MigrationStep(i, 1)) = S_PA; //Power Rüstung
  Local(S42MigrationStep(i, 1)) = GU02; // die großen Munitionsgürtel (10stück)
  Local(S42MigrationStep(i, 1)) = GU04;
  Local(S42MigrationStep(i, 1)) = GU06;
  Local(S42MigrationStep(i, 1)) = GU08;
  Local(S42MigrationStep(i, 1)) = GU10;
  Local(S42MigrationStep(i, 1)) = GU12;
  Local(S42MigrationStep(i, 1)) = GU14;
  Local(S42MigrationStep(i, 1)) = GU16;
  Local(S42MigrationStep(i, 1)) = GU18;
  Local(S42MigrationStep(i, 1)) = GU20;
  Local(S42MigrationStep(i, 1)) = 2SKT; //großes Schildupgrade
  Local(S42MigrationStep(i, 1)) = S_P2; //verstärkte Power Rüstung
  Local(S42MigrationStep(i, 1)) = SFEX;  //neuer Feuerlöscher
  Local(S42MigrationStep(i, 1)) = CNBF; //Plasma Accumulator
  s3 = i;


  // Waffen und Sprengzubehör
  Local(S42MigrationStep(i, 1)) = _BMR;  //Bumerang
  Local(S42MigrationStep(i, 1)) = FLNT;
  Local(S42MigrationStep(i, 1)) = TFLN;
  Local(S42MigrationStep(i, 1)) = D18Z;  //Smoke Bombe
  Local(S42MigrationStep(i, 1)) = CU8Z; //WireCutter
  Local(S42MigrationStep(i, 1)) = AXE1; //Wurfaxt
  Local(S42MigrationStep(i, 1)) = SFLN; //Superflint
  Local(S42MigrationStep(i, 1)) = FBMP; //Brandbombe
  Local(S42MigrationStep(i, 1)) = EFLN; //TeraFlint
  Local(S42MigrationStep(i, 1)) = SFLF; //SuperBrandFlint
  Local(S42MigrationStep(i, 1)) = EFL0; //Nitroflint
  Local(S42MigrationStep(i, 1)) = TRP1; //Torpedo
  Local(S42MigrationStep(i, 1)) = EFL2; //MAcroFlint
  Local(S42MigrationStep(i, 1)) = IBMP;  //Infernobombe
  Local(S42MigrationStep(i, 1)) = EFL6;  //Random Flint
  Local(S42MigrationStep(i, 1)) = SFLI;  //SuperInfernoFlint
  Local(S42MigrationStep(i, 1)) = EFL3; //Tetra Flint
  Local(S42MigrationStep(i, 1)) = SW01; //Messer
  Local(S42MigrationStep(i, 1)) = BLFL; //Blitzflint
  Local(S42MigrationStep(i, 1)) = PIWP;//Sturmpistole
  Local(S42MigrationStep(i, 1)) = ACWP;//AntiContent Waffe
  Local(S42MigrationStep(i, 1)) = SNWP;//Schallpistole
  Local(S42MigrationStep(i, 1)) = GRN2;  // einzelne Handgranate
  Local(S42MigrationStep(i, 1)) = SW02;  //Kampfmesser
  Local(S42MigrationStep(i, 1)) = GLWP; //Granatwerfer
  Local(S42MigrationStep(i, 1)) = BZWP; //Bazooka
  Local(S42MigrationStep(i, 1)) = MPWP;// Maschinengewehr
  Local(S42MigrationStep(i, 1)) = PPWP;//Partik. Pistole
  Local(S42MigrationStep(i, 1)) = SHGN; //Shotgun
  Local(S42MigrationStep(i, 1)) = ASHG; // abgesägte Schrotflinte
  Local(S42MigrationStep(i, 1)) = SOC2;//Dual Soccom
  Local(S42MigrationStep(i, 1)) = SW03; //Klinge
  Local(S42MigrationStep(i, 1)) = TRP3; //Torpedo Gold
  Local(S42MigrationStep(i, 1)) = MIN_; //Fakemine
  Local(S42MigrationStep(i, 1)) = SMI_; //FakeSchwarmmine
  Local(S42MigrationStep(i, 1)) = LMI_; //FakeLaserMine
  Local(S42MigrationStep(i, 1)) = MINE; //Mine
  Local(S42MigrationStep(i, 1)) = STRK;//HandRakete
  Local(S42MigrationStep(i, 1)) = ENWP;//Energiegewehr
  Local(S42MigrationStep(i, 1)) = FTWP;//Flammenwerfer
  Local(S42MigrationStep(i, 1)) = PTWP;//Partikellader
  Local(S42MigrationStep(i, 1)) = SCOT;//Scount (sniper)
  Local(S42MigrationStep(i, 1)) = SW04;  //Hammer (nahkampf)
  Local(S42MigrationStep(i, 1)) = MIWP; //Minigun
  Local(S42MigrationStep(i, 1)) = CWBO; //Bomb (alt)
  Local(S42MigrationStep(i, 1)) = SSPR; //Explosiv Kit
  Local(S42MigrationStep(i, 1)) = PGWP;//PumpGun
  Local(S42MigrationStep(i, 1)) = GGWP; //PartikelKanone  (Energiekanone)
  Local(S42MigrationStep(i, 1)) = TRP2; //Tetra-Torpedo
  Local(S42MigrationStep(i, 1)) = BLWP; //Partikelblaster
  Local(S42MigrationStep(i, 1)) = ELWP; //Partikelgewehr
  Local(S42MigrationStep(i, 1)) = M249; //M249 Waffe
  Local(S42MigrationStep(i, 1)) = P90_; //P90 Waffe
  Local(S42MigrationStep(i, 1)) = C40_; //PlastikSprengstoff + 2 Zünder
  Local(S42MigrationStep(i, 1)) = MFUS;
  Local(S42MigrationStep(i, 1)) = SFUS;
  Local(S42MigrationStep(i, 1)) = SMIN; //Schwarmmine
  Local(S42MigrationStep(i, 1)) = LMIN; //LaserMine
  Local(S42MigrationStep(i, 1)) = AMIN; //Alienmine
  Local(S42MigrationStep(i, 1)) = TRP4; //Todestorpedo
  Local(S42MigrationStep(i, 1)) = SNSS; //Nuclear bomb
  Local(S42MigrationStep(i, 1)) = SC_2; // Gaus Sniper
  Local(S42MigrationStep(i, 1)) = ATO2; //Atomtorpedo
  s4 = i;



  // Fahrzeuge
  Local(S42MigrationStep(i, 1)) = IDOL; //Statuen
  Local(S42MigrationStep(i, 1)) = GIDL;
  Local(S42MigrationStep(i, 1)) = WLRZ; //Holzlore
  Local(S42MigrationStep(i, 1)) = SLBT; //Boot
  Local(S42MigrationStep(i, 1)) = BALN; //Ballon
  Local(S42MigrationStep(i, 1)) = CATA; //Katapult
  Local(S42MigrationStep(i, 1)) = BRDG; //Holzbrückenstück
  Local(S42MigrationStep(i, 1)) = WAGN; //Waggon
  Local(S42MigrationStep(i, 1)) = STMG; //Dampfmaschine
  Local(S42MigrationStep(i, 1)) = XBOW; //Armbrust
  Local(S42MigrationStep(i, 1)) = BLMP; //Luftschiff
  Local(S42MigrationStep(i, 1)) = TNNE; //Tonne
  Local(S42MigrationStep(i, 1)) = LORY; //Lore
  Local(S42MigrationStep(i, 1)) = CANN;  //Haubitze
  Local(S42MigrationStep(i, 1)) = CTW3; //6 Geschütze für den Geschützturm
  Local(S42MigrationStep(i, 1)) = CTW5;
  Local(S42MigrationStep(i, 1)) = CTW7;
  Local(S42MigrationStep(i, 1)) = ACT1;
  Local(S42MigrationStep(i, 1)) = ACT3;
  Local(S42MigrationStep(i, 1)) = LICN;
  Local(S42MigrationStep(i, 1)) = SUB1; //Uboot
  Local(S42MigrationStep(i, 1)) = LORG; //GoldLore
  Local(S42MigrationStep(i, 1)) = LAMP;  //portable Lampe
  Local(S42MigrationStep(i, 1)) = CENM;//Emergency power module
  Local(S42MigrationStep(i, 1)) = CVLA;//Leichter Jäger
  Local(S42MigrationStep(i, 1)) = TANK;// Panzer
  Local(S42MigrationStep(i, 1)) = SHTL;// Shuttle
  Local(S42MigrationStep(i, 1)) = SCR_; //Scorpion
  Local(S42MigrationStep(i, 1)) = PRE_; //Predetor
  Local(S42MigrationStep(i, 1)) = TSP_; //Technosspähre
  Local(S42MigrationStep(i, 1)) = CGC_; //Bomber Cougar fregatte
  Local(S42MigrationStep(i, 1)) = MAM_; //Mammoth
  Local(S42MigrationStep(i, 1)) = TCN_; // Tucan
  Local(S42MigrationStep(i, 1)) = CCD_; //Concordia
  Local(S42MigrationStep(i, 1)) = CAN_; //Cancer
  Local(S42MigrationStep(i, 1)) = ST_3;  //Die Magnetsphäre
  s5 = i;



  // Gesamtgröße der Liste festhalten
  listsize = (s5 ?? 0)+1;
  return(1);
}


/* Aufrufe von anderen Objekten */

public func CategoryStart(num)
{
  if (Equal(num, 1)) return(nil);
  if (Equal(num, 2)) return((s1 ?? 0)+1);
  if (Equal(num, 3)) return((s2 ?? 0)+1);
  if (Equal(num, 4)) return((s3 ?? 0)+1);
  if (Equal(num, 5)) return((s4 ?? 0)+1);
  return(nil);
}

public func CategorySize(num)
{
  if (Equal(num, 1)) return((s1 ?? 0)+1);
  if (Equal(num, 2)) return((s2 ?? 0)-(s1 ?? 0));
  if (Equal(num, 3)) return((s3 ?? 0)-(s2 ?? 0));
  if (Equal(num, 4)) return((s4 ?? 0)-(s3 ?? 0));
  if (Equal(num, 5)) return((s5 ?? 0)-(s4 ?? 0));
  return(nil);
}

public func GetListSize(...) { return(listsize);
}

public func FindCategory(num)
{
  if ((num ?? 0) <= (s1 ?? 0)) return(1);
  if ((num ?? 0) <= (s2 ?? 0)) return(2);
  if ((num ?? 0) <= (s3 ?? 0)) return(3);
  if ((num ?? 0) <= (s4 ?? 0)) return(4);
  if ((num ?? 0) <= (s5 ?? 0)) return(5);
  return(nil);
}

