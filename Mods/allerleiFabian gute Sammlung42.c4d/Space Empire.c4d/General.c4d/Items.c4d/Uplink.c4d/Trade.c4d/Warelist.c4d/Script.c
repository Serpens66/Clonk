/*-- Warenliste --*/

#strict

local s1, s2, s3, s4, s5;   // Nummer in der Liste der jeweils letzten Ware einer Kategorie
local listsize;             // Größe der gesamten Handelswarenliste

// die nummerierten Locals enthalten die Waren-IDs


/* Initialisierung */

protected func Initialize()
{
  // Handelslisten in Locals speichern
  var i = -1;

// Materialien
  Local(++i) = ERTH;
  Local(++i) = ROCK;
  Local(++i) = LOAM;
  Local(++i) = WOOD;
  Local(++i) = GOLD;
  Local(++i) = CTIT;
  Local(++i) = CALU;
  Local(++i) = METL;
  Local(++i) = SURO;
  Local(++i) = CRYS;
  Local(++i) = CTIO;
  Local(++i) = CALO;
  Local(++i) = ORE1;
  Local(++i) = COAL;
  Local(++i) = SPHR;
  Local(++i) = ANML;  //Schmiedeeisen
  Local(++i) = PSTC;   //Plastik
  Local(++i) = ASHS; //Asche
  Local(++i) = ICE1;
  Local(++i) = SAND; //Sand
  s1 = i;

  // Grundausrüstung
  Local(++i) = CNKT; //Bausatz
  Local(++i) = FK9Z;  //Fackel
  Local(++i) = LNKT;
  Local(++i) = CNCR;      //Beton
  Local(++i) = LIQG;      //Flüssiggranit
  Local(++i) = TENP; //Ritterzelt gepackt
  Local(++i) = WDBR; //Holzfass
  Local(++i) = BARL; //Metallfass
  Local(++i) = AM5P; //Fernrohr
  Local(++i) = BETL; //Beutel
  Local(++i) = SPKT; //Sperrenbausatz kleine Fahrstühle
  Local(++i) = GV5Y;  //Gangverschluss
  Local(++i) = FLAG; 
  Local(++i) = SAK1; //Sack
  Local(++i) = PNCR; //Profibeton
  Local(++i) = TB5B;  //SFT Zeltpaket
  Local(++i) = NLGT; //Knicklicht
  Local(++i) = FLSH; //Taschenlampe
  Local(++i) = AILK; //Airlock Bausatz
  Local(++i) = UNCR; //Ultimativer beton
  Local(++i) = TOKT; //Werkzeugkasten
  Local(++i) = UPLI;  //Uplink
  Local(++i) = CS4K;  //Sensorenzubehöhr 8 stück
  Local(++i) = PS4K;
  Local(++i) = BS4K;
  Local(++i) = LS4K;
  Local(++i) = SS4K;
  Local(++i) = WS4K;
  Local(++i) = GTE1;
  Local(++i) = KC4K;
  Local(++i) = CGBE; //Hydrogen Bottle
  Local(++i) = KI_T;//Kiste
  Local(++i) = SBST; //Isotope 
  Local(++i) = IMTK;//verb. Werkzeugkasten
  Local(++i) = ABRL; //verschieden gefüllte Fässer
  Local(++i) = LBRL;
  Local(++i) = OBRL;
  Local(++i) = WBRL;
  Local(++i) = WDWB; //HolzWasserfass
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
  Local(++i) = AMWS;  //Atommüll,Metallschrott,Leiche und Yellow Cake haben keine gepackte Version, also ist okay hinzufügen 
  Local(++i) = YECK; 
  Local(++i) = CORP; 
  Local(++i) = MTWS;  

  s2 = i;



  // spezielle Ausrüstung Munition
  Local(++i) = _Y3B; //früher Feuerschlöscher
  Local(++i) = JPTH; //Springstein
  Local(++i) = ARWP; //Pfeile
  Local(++i) = FARP; //Feuerpfeile
  Local(++i) = TAAZ;  // Tauscheranzug
  Local(++i) = THLM; //Taucherhelm
  Local(++i) = XARP;  //Sprengpfeile
  Local(++i) = DK8Z;  //Frogspring
  Local(++i) = E18Z;  //Air-Elevator
  Local(++i) = E78Z; //frogspring2
  Local(++i) = GUNP; //schießpulver
  Local(++i) = ADM1; // Munitionspack Geschützturm
  Local(++i) = ADM3; // Batterie
  Local(++i) = WC_0; // 4 Waffenbauteile
  Local(++i) = WC_1;
  Local(++i) = WC_2;
  Local(++i) = WC_3;
  Local(++i) = GRAP; //Granatenmunitionskiste
  Local(++i) = STAP;//Munitionskiste
  Local(++i) = AIRT;//Atemflasche
  Local(++i) = MEDI; //Medipack
  Local(++i) = ENAP; //Engergiezelle munition
  Local(++i) = SDBM; //Deradbombe
  Local(++i) = PTAP; //Partikelbehälter (munition)
  Local(++i) = S_RB; //5 Einfache Rüstungen
  Local(++i) = S_LJ;
  Local(++i) = S_MA;
  Local(++i) = S_L2;
  Local(++i) = S_CA;
  Local(++i) = STLM;  //better Bullets munition
  Local(++i) = SHMU; //SchrotMunition
  Local(++i) = SNI_; //Snipermunition
  Local(++i) = GSAP; // Benzinkanister (Munition)
  Local(++i) = JTPK; //Jetpack
  Local(++i) = MIAP; //Raketen Munition
  Local(++i) = HARM; //Rüstung
  Local(++i) = HSHD; //Schildgenerator
  Local(++i) = SPC1; //Explosiv Siege Ammo
  Local(++i) = _SKT; //Schildupgrade
  Local(++i) = S_TA; //Tesla Rüstung
  Local(++i) = PNGU; //Pinguinanzug
  Local(++i) = SPC2; //Cluster siege Ammo
  Local(++i) = _Y4C;//Schutzanzug
  Local(++i) = S_C2; //verstärkte Rüstungen 4 stück
  Local(++i) = S_LA;
  Local(++i) = S_M2;
  Local(++i) = S_CJ;
  Local(++i) = GU01; //die kleinen Munitionsgürtel (10 stück)
  Local(++i) = GU03;
  Local(++i) = GU05;
  Local(++i) = GU07;
  Local(++i) = GU09;
  Local(++i) = GU11;
  Local(++i) = GU13;
  Local(++i) = GU15;
  Local(++i) = GU17;
  Local(++i) = GU19;
  Local(++i) = SPC4; //Toxic Siege Ammo
  Local(++i) = SPC3;//Nuclear Siege Ammo
  Local(++i) = JB4K; //Jetbelt
  Local(++i) = S_BA; //SpezialRüstung
  Local(++i) = SREP; //Repair Naniten
  Local(++i) = KLAS; //LaserUpgrade
  Local(++i) = KSLM; //Bio Upgrade
  Local(++i) = KRFL; //WAffenteil Upgrade
  Local(++i) = STRH; //Partikelkonverter (Upgrade)
  Local(++i) = S_PA; //Power Rüstung
  Local(++i) = GU02; // die großen Munitionsgürtel (10stück)
  Local(++i) = GU04;
  Local(++i) = GU06;
  Local(++i) = GU08;
  Local(++i) = GU10;
  Local(++i) = GU12;
  Local(++i) = GU14;
  Local(++i) = GU16;
  Local(++i) = GU18;
  Local(++i) = GU20;
  Local(++i) = 2SKT; //großes Schildupgrade
  Local(++i) = S_P2; //verstärkte Power Rüstung
  Local(++i) = SFEX;  //neuer Feuerlöscher
  Local(++i) = CNBF; //Plasma Accumulator
  s3 = i;


  // Waffen und Sprengzubehör
  Local(++i) = _BMR;  //Bumerang
  Local(++i) = FLNT;
  Local(++i) = TFLN;
  Local(++i) = D18Z;  //Smoke Bombe
  Local(++i) = CU8Z; //WireCutter
  Local(++i) = AXE1; //Wurfaxt
  Local(++i) = SFLN; //Superflint
  Local(++i) = FBMP; //Brandbombe
  Local(++i) = EFLN; //TeraFlint
  Local(++i) = SFLF; //SuperBrandFlint
  Local(++i) = EFL0; //Nitroflint
  Local(++i) = TRP1; //Torpedo
  Local(++i) = EFL2; //MAcroFlint
  Local(++i) = IBMP;  //Infernobombe
  Local(++i) = EFL6;  //Random Flint
  Local(++i) = SFLI;  //SuperInfernoFlint
  Local(++i) = EFL3; //Tetra Flint
  Local(++i) = SW01; //Messer
  Local(++i) = BLFL; //Blitzflint
  Local(++i) = PIWP;//Sturmpistole
  Local(++i) = ACWP;//AntiContent Waffe
  Local(++i) = SNWP;//Schallpistole
  Local(++i) = GRN2;  // einzelne Handgranate
  Local(++i) = SW02;  //Kampfmesser 
  Local(++i) = GLWP; //Granatwerfer
  Local(++i) = BZWP; //Bazooka
  Local(++i) = MPWP;// Maschinengewehr
  Local(++i) = PPWP;//Partik. Pistole
  Local(++i) = SHGN; //Shotgun
  Local(++i) = ASHG; // abgesägte Schrotflinte
  Local(++i) = SOC2;//Dual Soccom
  Local(++i) = SW03; //Klinge
  Local(++i) = TRP3; //Torpedo Gold
  Local(++i) = MIN_; //Fakemine
  Local(++i) = SMI_; //FakeSchwarmmine
  Local(++i) = LMI_; //FakeLaserMine
  Local(++i) = MINE; //Mine
  Local(++i) = STRK;//HandRakete
  Local(++i) = ENWP;//Energiegewehr
  Local(++i) = FTWP;//Flammenwerfer
  Local(++i) = PTWP;//Partikellader
  Local(++i) = SCOT;//Scount (sniper)
  Local(++i) = SW04;  //Hammer (nahkampf)
  Local(++i) = MIWP; //Minigun
  Local(++i) = CWBO; //Bomb (alt)
  Local(++i) = SSPR; //Explosiv Kit
  Local(++i) = PGWP;//PumpGun
  Local(++i) = GGWP; //PartikelKanone  (Energiekanone)
  Local(++i) = TRP2; //Tetra-Torpedo
  Local(++i) = BLWP; //Partikelblaster
  Local(++i) = ELWP; //Partikelgewehr
  Local(++i) = M249; //M249 Waffe
  Local(++i) = P90_; //P90 Waffe
  Local(++i) = C40_; //PlastikSprengstoff + 2 Zünder
  Local(++i) = MFUS;
  Local(++i) = SFUS;
  Local(++i) = SMIN; //Schwarmmine
  Local(++i) = LMIN; //LaserMine
  Local(++i) = AMIN; //Alienmine
  Local(++i) = TRP4; //Todestorpedo
  Local(++i) = SNSS; //Nuclear bomb
  Local(++i) = SC_2; // Gaus Sniper
  Local(++i) = ATO2; //Atomtorpedo
  s4 = i;



  // Fahrzeuge
  Local(++i) = IDOL; //Statuen
  Local(++i) = GIDL;
  Local(++i) = WLRZ; //Holzlore
  Local(++i) = SLBT; //Boot
  Local(++i) = BALN; //Ballon
  Local(++i) = CATA; //Katapult
  Local(++i) = BRDG; //Holzbrückenstück
  Local(++i) = WAGN; //Waggon
  Local(++i) = STMG; //Dampfmaschine
  Local(++i) = XBOW; //Armbrust
  Local(++i) = BLMP; //Luftschiff
  Local(++i) = TNNE; //Tonne
  Local(++i) = LORY; //Lore
  Local(++i) = CANN;  //Haubitze
  Local(++i) = CTW3; //6 Geschütze für den Geschützturm
  Local(++i) = CTW5;
  Local(++i) = CTW7; 
  Local(++i) = ACT1;
  Local(++i) = ACT3;
  Local(++i) = LICN;
  Local(++i) = SUB1; //Uboot
  Local(++i) = LORG; //GoldLore
  Local(++i) = LAMP;  //portable Lampe
  Local(++i) = CENM;//Emergency power module
  Local(++i) = CVLA;//Leichter Jäger
  Local(++i) = TANK;// Panzer
  Local(++i) = SHTL;// Shuttle
  Local(++i) = SCR_; //Scorpion
  Local(++i) = PRE_; //Predetor
  Local(++i) = TSP_; //Technosspähre
  Local(++i) = CGC_; //Bomber Cougar fregatte
  Local(++i) = MAM_; //Mammoth
  Local(++i) = TCN_; // Tucan
  Local(++i) = CCD_; //Concordia
  Local(++i) = CAN_; //Cancer
  Local(++i) = ST_3;  //Die Magnetsphäre
  s5 = i;



  // Gesamtgröße der Liste festhalten
  listsize = s5+1;
  return(1);
}


/* Aufrufe von anderen Objekten */

public func CategoryStart(num)
{
  if (num==1) return(0);
  if (num==2) return(s1+1);
  if (num==3) return(s2+1);
  if (num==4) return(s3+1);
  if (num==5) return(s4+1);
  return(0);
}

public func CategorySize(num)
{
  if (num==1) return(s1+1);
  if (num==2) return(s2-s1);
  if (num==3) return(s3-s2);
  if (num==4) return(s4-s3);
  if (num==5) return(s5-s4);
  return(0);
}

public GetListSize: return(listsize);

public func FindCategory(num)
{
  if (num <= s1) return(1);
  if (num <= s2) return(2);
  if (num <= s3) return(3);
  if (num <= s4) return(4);
  if (num <= s5) return(5);
  return(0);
}

