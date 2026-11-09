/*-- Forschungsbaum --*/

#strict 2
// static gForschungverbotenlisteVariable; // werden in regelchooser objekt eingeführt
// static gForschungerforschtlisteVariable; // wird nur verwendet, wenn szeario forschungsbaum aktiv, denn dann ist StartZeitalter deaktiviert. Dies wird dann zusätzlich zu den szenarioeinstellungen noch erforscht und wird im szenarioskript festgelegt.
// static gSzenarioStandardForschungFreiwilligVariable //werden in regelchooser objekt eingeführt

// in Szeanrio skript kann noch rein:
// public func ScenarioForschungsEinstellungen() // liefert alle Forschungen, die verboten, erforscht oder freiwillig sein sollen. wenn eintrag "leer" enthaelt oder diese fkt nicht existiert, werden die standardeinstellungen der gelchoosers verwendet
// {
    // var verboten = [_U2A,_X1A,TABA,TAMB,TAGB,BETL,SAK1,KI_T,_SH5,KA5C,S_O2,S_O3,BBUN,CABO]; // Muellverbr.,Luftregene.,kleine, mittlere, große Basis,Beutel, Sack, Kiste, Herausforderungshaus, Ausbildungszentrale, Soldner10 und 15, Bunker, Sauerstoffflasche.
    // for(var a=9;a<23;a++) // alle forschungen ab zeitalter 9 verbieten
        // ArrayAddArray(verboten,gForschungsliste(a));
    // return([
    // ["Verboten",verboten], // theoretisch eine auflistung von IDs
    // ["Freiwillig",["leer"]], // werden zu den standard freiwilligen forschungen zugefuegt
    // ["Erforscht",[ELEF]]  // zusätzlich zu den im szenario eingestellten Bauplänen, soll jeder noch diese dinge zu beginn erforscht haben, (eig nur wichtig, wenn kein forschungslabor gebaut koennen werden soll)
    // ]);
// }



// die Funktion, welche diesen Wissensstand z.b zu Spielstart vergibt. Wird nicht aufgerufen, wenn szenario Forshcungsbaum aktiv.
global func GlobalStartingEra(zahl) // wird in regelchooser aufgerufen, nach erstem Start, oder wenn im spiel an start zeitalter was geändert wurde (dort wird auch vorher vorhandenes wissen gelöscht)
{
    for(var j=0;j<zahl+1;j++) // alle forschungslisten bis zum gewünschten zeitalter durchgehen und forschungen verteilen
    {
        var forschliste = gForschungsliste(j);
        for(var i = 0; i < GetPlayerCount(); i++)
           {
               for(var element in forschliste)
               {
                  if(!(element==_BAL)) // alles außer "umwelt stärken" erforschen. umweltstärken macht umwelt belastbarer undlässt bäume wachsen
                    SetPlrKnowledge(GetPlayerByIndex(i), element);
               }
           }
    }
}

global func gForschungfreiwilligliste() // eine liste von IDs dessen Forschung als "freiwillig" im Forschungslabor gekennzeichnet werden soll. Dh. sie sind nicht nötig, um die nächste Forshcungsstufe freizuschalten.
{   // auch werden diese IDs aus den Voraussetzungen in getresearchbaselist gestrichen, damit sie wirklich freiwillig sind
    // alle hazardwaffen und ruestungen sind freiwillig
    var aForschungfreiwilliglisteVariable = [HATR,CTW3,CTW5,CTW7,ACT1,ACT3,LICN,CS4K,PS4K,BS4K,LS4K,SS4K,WS4K,GTE1,KC4K,_BAO,_BAL,GU01,GU02,GU03,GU04,GU05,GU06,GU07,GU08,GU09,GU10,GU11,GU12,GU13,GU14,GU15,GU16,GU17,GU18,GU19,GU20,
        ERIT,_BA2,TR33,WAL4,SIGN,TAAZ,THLM,LUPU,QIID,QBII,RKB2,0J41,0K41,SF02,SS02,SB52,LF25,SF25,HK39,OD59,DO59,CMG1,CPG1,SCN1,STW1,CPG2,SCN2,STW2,STW3,STW4,SLSW,IDOL,GIDL,_SH5,BETL,SAK1,KI_T,CONS,CNS2,
        GRAP,ENAP,MIAP,STAP,GSAP,SNI_,SHMU,PTAP,STLM,MFUS,SFUS,PLR1,PLR2,PLR3,S_RB,S_LJ,S_CJ,S_MA,S_M2,S_L2,S_LA,S_TA,S_CA,S_C2,S_BA,S_PA,S_P2,PNGU,_Y4C,BZWP,MEZL,ENWP,FTWP,GLWP,MIWP,GGWP,PIWP,PGWP,ACWP,
        SNWP,BLWP,PTWP,PPWP,ELWP,WRST,MPWP,M249,SC_2,P90_,ASHG,SHGN,SCOT,SOC2,WC_3,WC_1,WC_0,WC_2,SW01,SW02,SW03,SW04,SMI1,LMIN,MINE,LMI_,MIN_,SMI_,LOOC,DE_5,SBBA,TAUC,_SKT,2SKT,GIGN,TWR1,IMLO,TG6B,TMST];
    ArrayAddArray(aForschungfreiwilliglisteVariable,gSzenarioStandardForschungFreiwilligVariable); // alle vom szenario als freiwillig eingestuften
    ArrayAddArray(aForschungfreiwilliglisteVariable,gForschungverbotenliste()); // alle verbotenen forschungen als freiwillig markieren, da sie sonst Forschungsfortschritt unmoeglich machen.
    return(aForschungfreiwilliglisteVariable);
}

global func gForschungverbotenliste() // wird vom szenario festgelegt
{
    if(gForschungverbotenlisteVariable==0) // sollte nie eintreffen, da in regelchooser standard gesetzt wird.
        gForschungverbotenlisteVariable = []; // erstmal leere liste
    return(gForschungverbotenlisteVariable);
}

global func gPlayerAge(iPlr) // gibt den aktuellen wissensstand eines spielers zurück, die zahl richtet sich nach den Einteilungen in gForschungsliste()
{
    // var age = -1;
    // for(var a=0;a<23;a++) // aktuell gibts 22 "zeitalter" in der gForschungsliste, 
    // {
        // for(var idDef in gForschungsliste(a)) // geht alle ids in der jeweiligen liste durch
        // {
            // if(GetPlrKnowledge(iPlr,idDef))
                // age++; // wenn mind. eines davon erforscht, dann ist der spieler in diesem Zeitalter
                // break; // diese for schleife kann abgebrochen werden, es reicht zu wissen, das mind. eines davon erforscht ist 
        // }
    // }
    // return(age);
    var breaken=0;   // Unterschied zur vorherigen Variante: wenn z.b alles von a==1 erforscht ist, gibt funktion nun 2, anstatt 1 zurück.
    for(var a=0;a<24;a++) // aktuell gibts 22 "zeitalter" in der gForschungsliste, 23 wenn alles erforscht
    {
        for(var idDef in gForschungsliste(a)) // geht alle ids in der jeweiligen liste durch
        {
            if(!GetPlrKnowledge(iPlr,idDef))
                breaken = 1;
                break; // sobald etwas nicht erforscht ist, break und a returnen.
        }
        if(breaken || GetLength(gForschungsliste(a))==0) break;
    }
    return(a);
}

global func gForschungsliste(a) 
{
    var Liste = [];
    if(a==0) // zu beginn erforscht
    {
        Liste = [HUT1,HUT2,RSRC,SIDL,IGLO,CAVE,AXTB,LAGF,_DYN,WMIL,CNKT,ANVL,FACK,LNKT]; // Bambushütte, Holzhütte, Forschungslabor, Schneemann, Iglu, Schacht. Spaltaxt, lagerfeuer, Dynamo, Windmühle, Bausatz, Amboss, Fackel, Leitung       Von anfang an erforscht.
    }
    if(a==1)
    {
        Liste = [IDOL,SAWM,QIID,FRGE,FDRS,WDBR,WRKS,WLRR,KA_S]; 
        //Statue    // freiwillig
        //Sägewerk
        // kleine schneestatue   freiwillig
        //Esse
        //kl. Hochofen
        //Holzfass
        //Werkstatt
        //HolzLore
        // KAserne
    }
    if(a==2)
    {
        Liste = [HUT3,TENP,AEHN,_BMR,SIGN,ELEF,BETL,_SH5,TR33,WAL4,QBII,FH9I]; 
        //Steinhütte
        //Ritterzelt gepackt
        //Alte Handelsstation
        //Bumerang
        //straßenschild    freiwillig
        //Fahrstuhl
        //Beutel       freiwillig
        // Auftragshaus     freiwillig
        // Baumversteck      freiwillig (in funktion gForschungfreiwilligliste() eingefügt)
        // versteck Wand     freiwillig
        // große schneestatue     freiweilig
        //Forsthaus
    }
    if(a==3)  // appendto in Forschungsbuam2.c
    {
        Liste = [CHEM,FLNT,TFLN,GIDL,AM5P,EAWL,B4S7,_Y3B,SPKT,WGTW,GV5Y,GIGN,S_O1,KSDL,SHM6,BARL,WTON,GOL5];
        // frühe Chemiefabrik
        //Feuerstein
        //TFlint
        //Goldstatue     freiwillig
        //Fernrohr
        //Lehmklumpen manschen
        //Fundament (einzeln)
        //früher Feuerlöscher
        //Sperrenbausatz für kleine Fahrstühle
        //HolzWachturm
        //Gangverschluss
        //gr. Straßenschild      freiwillig
        //Stufe 5 Soeldner
        // Sattel
        //Masterschlüssel 
        //MetalFass
        // Wassertonne
        // 5er Gold
    }
    if(a==4)
    {
        Liste = [STFN,CST1,D18Z,SLBT,CATA,BRDG,CU8Z,FLAG,BT7U,JPTH,TWR1,RW4V,AXE7,SEN6,CHPI,DYYN,_USW];
        //Super T-Flint
        //kleineBurg
        //SmokeBombe
        //Boot
        //Katapult
        //HolzBrücke
        //WireCutter
        //Flagge
        //Beobachtungsturm (alarm)
        //Springstein
        //kleiner Turm   freiwillig
        // Seilbahn
        // Wurfaxt
        // Sense
        //Kohlemeiler
        // besserer Dynamo, liefert mehr energie, von 10 auf 25 erhöht
        // Umspannwerk
    }
    if(a==5)
    {
        Liste = [C11B,DH8Z,D_8Z,POWR,PUMP,SFLN,WAGN,SGNL,STMG,XBOW,ARWP,FARP,FBMP,CST2,SPDM,GLAS,SAER,F_EL,FNDR,TWR2,WTWR,BALN,TONN,GRUS,SPER,0J41,LORY];
        //Stein zu Beton
        //Blitzableiter
        //Blitzanzieher
        //Kraftwerk
        //Pumpe
        //SuperFlint
        //Waggon
        //Signal
        //Dampfmaschine
        //Armbrust
        //Pfeile
        //Feuerpfeile
        //Brandbombe
        //Burg
        //Superkuppel
        //normal Glas herstellen
        //Sand aus Erde
        //Metallfahrstuhl
        //Hochofen
        //Stein Wachturm
        //Zauberturm
        //Ballon
        // Feuertonne
        // Gerüst
        // Speer
        // Crumbler Flint    freiwillig
        //Lore
    }
    if(a==6)
    {
        Liste = [TAAZ,THLM,LUPU,EFLN,GRS1,DRCK,BLMP,GRS3,XARP,TNNE,DK8Z,BELV,BEKP,SFLF,E18Z,_BRD,S_O2,DTFF,L11B,EAW2,LF25,SF25,DO59];
        //Taucheranzug    freiwillig
        //Taucherhelm     freiwillig
        //Luftpumpe       freiwillig
        //Tera Flint
        //Grundgerüst
        //Bohrturm
        //Luftschiff
        //Flaschenzug
        //Sprengpfeile
        //Container fuer flschanezug
        // Frog Spring
        //Transport Fahrstuhl
        //Schachtsperre für Trans. Fahrstuhl
        //Super-BrandFlint (superflint und brandbombe)
        //Air Elevator
        //Metal Brücke
        // Soldner stufe 10
        // Tank 
        // Flüssig Granit aus beton
        // große lehmklumpen
        // Doof             freiwillig
    }
    if(a==7)
    {
        Liste = [E78Z,CST3,CANN,GUNP,EFL0,_U2A,HTCH,HATR,KA5C,TETF,0K41,SF02,TG6B]; 
        //Frog spring 2
        //Schloss
        //Haubitze
        // Schießpulver
        //Nitro- Flint
        //Müllverbrennungsanlage
        //kleine Luke
        //Forschung Handelsstation Transfer
        // Ausbildungscamp
        // Tera-T-Flint
        // Super Crumbler Flint    freiwillig
        // Splitter Flint         freiwillig
        // LAve Flint       frewillig
        // Säure Flint       frewillig
        // Tunnelgate
    }
    if(a==8)
    {
        Liste = [CABO,CTW0,CTW3,CTW5,CTW7,ACT1,ACT3,LICN,ADM1,SUB1,TRP1,EFL2,ADM3,CHBU,SAK1,_X1A,SPD3,PNCR,IBMP,TABA,TAMB,TAGB,BBUN,DTBQ,DTBA,S_O3,LOOC,HK39,OD59];
        //Sauerstoffflasche
        //Geschützturm
        //Geschütz1   freiwillig
        //Geschütz2   freiwillig
        //Geschütz3   freiwillig
        //Geschütz4   freiwillig
        //Geschütz5   freiwillig
        //Geschütz6   freiwillig
        //Munitionspack (schießpulver)
        //U-Boot
        //Torpedo
        //Macro-Flint
        //Batterie
        //Köhlerei
        //Sack                // freiwillig
        //Regenerator (ohne Temp fähigkeit, später erforschbar)
        //MegaKuppel
        //Profibeton
        //Infernobombe
        // kleine Basis
        // mittlere Basis
        // große Basis
        // Bunker
        // Tiefbrunnen
        // Bergwerk
        // Soldner 15
        // stabilere Schlösser           freiwillig
        // Hauskiller       freiwillig
        // Oberdoof         freiwillig
    }
    if(a==9)
    {
        Liste = [CCHF,SDWU,SFDY,SWEG,TB5B,LORG,HTC2,EFL6,SFLI,DE_5,SBBA,B556,TAUC,RKB2,WRBO,DTXA,SB52,IMLO];
        // neues Forschungslabor fuer alles ab zeitalter 10
        // Wohneinheit
        // Futur Gießerei 
        // futur Windmühle
        //Zeltpaket
        //GoldLore
        //große Luke
        //Random Flint
        //Super-InfernoFlint
        //Schutt              frewilllig
        //Sandsäcke            freiwillig
        //Zukunfts Fundament (einzeln)  soblad erforscht gibt es hazardclonks zu kaufen
        // UAV Station           frewillig   
        // Silvester raketen        freiwillig
        // Bumerang verbesert   
        // Saugerlore
        // verfolger Bombe        freiwillig
        // bessere Leitungen, um zukunftsgebäude zu versorgen, da diese mehr energie benötigen, als die alten leitungen transportieren koennen
    }
    if(a==10)
    {
        Liste = [CNBS,GLSH,KNIK,LAMP,FLSH,SCHM,CREF,CRCO,F_0C,SPCO,PLST,CFUR,SGEO,ERER,AILK,EFL3,SMFC,SWEF,OILR,SW01,WC_0,WC_1,WC_2,WC_3,O00F,SPD2,SPD4,UNCR,TOKT,BLFL,SSEE,UPLI,SFEX,ST_4,ST_3,S_O4,_BLG,GOTA,SS02,CVFC];
        // Futur Basis
        // verbesserte Glas Herstellung
        //knicklichter herstellen
        // Lampe (ganz früheres knicklicht)
        //Taschenlampe
        // neue Chemiefabrik
        //Rafinerie
        //Kohle zu Kristall
        //Elektrolyse (Säure Erz)
        //Öl Schwefel
        //Plastikherstellung
        //Heizkraftwerk
        //Geothermisches Werk
        //Erde zu Erz
        //Airlockbausatz
        // Tetra Flint
        //Maschinenfabrik
        //Waffenfabrik
        //Öl Herstellung
        //Messer
        //Waffenbauteile 1+2+3+4
        //Standleuchte
        //SuperTitanKuppel
        //MegaTitanKuppel
        //Ultimativer beton
        //Werkzeugkasten
        //Blitzflint
        //Sensorenzubehör (8 teile)  
        //Uplink
        // neuer Feuerlöscher
        // Bahnhof
        // Magnetsphäre
        // Soldner 20
        // Blendgranate
        // Tac 
        // Super SplitterFlint     freiwillig
        // Raumschifffabrik 
    }
    if(a==11)          
    {
        Liste = [CPMP,GRAP,MEDI,PIWP,STAP,AIRT,ENAP,SBTR,CENM,SDBM,CNBE,PTAP,ACWP,SNWP,_BAO,S_RB,S_LJ,S_MA,S_L2,S_CA,STLM,SHMU,SNI_,GRN2,SW02,TWRF,MIAP,GSAP,MEZL,CH2P,CFCP,CHOL,WRST,LZD_,GOBE];
        //autom. Pumpe
        //Granatenmunitionskiste        freiwillig 
        //Medipack
        //Sturmpistole
        //Munitionskiste         freiwillig
        //Atemflasche
        //Engergiezelle munition         freiwillig
        //Emergency Power Supply Batterie Gebäude
        //Emergency power module energiemodul
        //Deradbombe
        //Plasma Accumulator leerer Akku
        //Partikelbehälter (munition)         freiwillig
        //AntiContent Waffe
        //Schallpistole
        //Umweltbewusststein 1
        //alle einfachen Rüstungen (außer Power. 5 stück)
        //Neue ProjektilMunitionen (3Stück)                   freiwillig
        //Handgranate
        //Kampfmesser
        //ZukunftsWachturm
        //Raketen Munition         freiwillig
        // Benzinkanister (Munition)         freiwillig
        //Motorsäge
        //H2 Produktion
        //Brenstoffzelle gebäude
        // FuturFahrstuhl2 der durch alles graben kann mit energiezellen
                                                 // Munitionsfabrik  deaktiviert und rausgenommen, lieber woanders munition herstellen
        // Stunner
        // LZD
        // Betäubungsgranate
    }
    if(a==12)
    {
        Liste = [GLWP,JTPK,BZWP,CDFT,HARM,HSHD,SMBS,SPC1,SCNB,CH2R,CVLA,FLAK,PRC1,MPWP,KI_T,PPWP,_SKT,S_TA,PNGU,ASHG,SOC2,SW03,TRP3,MIN_,SMI_,LMI_,FTWP,PGWP,CGBE,NVDN,WREJ,CMG1,CPG1,SCN1,STW1,CPG2,SCN2,STW2,STW3,STW4,SLSW,SSPR];
        //Granatwerfer
        //Jetpack
        //Bazooka
        //Abwehrturm
        //Rüstung
        //Schildgenerator
        //Hauptbasis
        //Explosiv Siege Ammo
        //Geschützplattform (Canon base)
        //Hydrogen Power Plant
        //Leichter Jäger
        //Flak
        //Partikelgeschütz Hazard
        // Maschinengewehr
        //Kiste                freiwillig
        //Partik. Pistole
        //Schildupgrade     freiwillig
        //Tesla Rüstung
        //Pinguinanzug
        //abgesägte Schrotflinte
        //Dual Soccom
        //Klinge
        //Torpedo Gold
        //Fakemine
        //FakeSchwarmmine
        //FakeLaserMine
        //Flammenwerfer
        //PumpGun
        //leere Hydrogen Bottle
        //Novadekan
        // EJD
        //Minigun Geschütz                       freiwillig
        // Plasma Geschütz                       freiwillig
        // ArtillerieGeschütz                       freiwillig
        // Mascinengewehrgeschütz                       freiwillig
        // ZwillingPlasma Geschütz                       freiwillig
        // Bazooka Geschütz                        freiwillig
        // Raketenwerfer Geschütz                        freiwillig
        // Elektro Geschütz                        freiwillig
        // Vulcan Geschütz                        freiwillig
        //Laser Geschütz                        freiwillig
        //Explosiv Kit

    }
    if(a==13)
    {
        Liste = [TANK,SHTL,SDRK,MINE,SPC2,CENC,SAPP,SBST,STRK,ENWP,CGDT,PTWP,_BAL,_Y4C,S_C2,S_LA,S_M2,S_CJ,SCOT,FL2H,SW04,IMTK,GU01,GU03,GU05,GU07,GU09,GU11,GU13,GU15,GU17,GU19,PLR1,PLR2,PLR3,ERIT,MIWP,CWBO,BTWP,
        STRM,SHGN,TRP2];
        // Panzer
        // Shuttle
        //Bohrturm
        //Mine
        //Cluster siege Ammo
        //Energy Collector
        //Atomkraftwerk
        //Isotope 
        //HandRakete
        //Energiegewehr
        //Guardian Bunker (Wachturm, GuardTower)
        //Partikellader
        //Umwelt stärken
        //Schutzanzug
        //verstärkte Rüstungen (4Stück)
        //Scount (sniper)
        //Flutlicht
        //Hammer (nahkampf)
        //verb. Werkzeugkasten
        //die kleinen Munitionsgürtel (10 stück)      freiwillig
        //Plutoniumumwandlungsprozesse (3Stück)    freiwillig
        //KI Händler erforschen (optional durch eine Regel lässt sich für diese Forschung noch sie selbst hinzufügen, wodurch sie nicht erforschbar ist)   freiwillig
        //Minigun
        //Bomb (alt)
        //Schweißbrenner
        //Transmitter
        //Tetra-Torpedo
    }
    if(a==14)
    {
        Liste = [STOW,SPC4,SPC3,CWPP,GGWP,JB4K,BLWP,ELWP,S_BA,M249,P90_,C40_,MFUS,SFUS,SEGU,AIRS,TELE,CONS,CNS2,LOR1,IFT2,TMST];
        //Guardian Outpost (Tower, Wachturm Spacelaunch)
        //Toxic Siege Ammo
        //Nuclear Siege Ammo
        //Advanced windmühle
        //PartikelKanone  (Energiekanone)
        //Jetbelt
        //Partikelblaster
        //Partikelgewehr
        //SpezialRüstung
        //M249 Waffe
        //P90 Waffe
        //PlastikSprengstoff + 2 Zünder     // zuender freiwillig  
        //Selbstschussanlage (SEGU)
        //Luftschlag
        //Teleporter
        //Konsole 1+2      frewiwillig
        // Materialsauger
        // Ionenflint
        // Temperatur steuern
    }
    if(a==15)
    {
        Liste = [SREP,KLAS,KSLM,KRFL,SMI1,LMIN,AMIN,STRH,S_PA,GU02,GU04,GU06,GU08,GU10,GU12,GU14,GU16,GU18,GU20,_BA2,2SKT,TRP4,CPBX,DKIT,UUU1,STTM];
        //Repair Naniten
        //LaserUpgrade
        //Bio Upgrade
        //WAffenteil Upgrade
        //Schwarmmine
        //LaserMine
        //Alienmine
        //Partikelkonverter (Upgrade)
        //Power Rüstung
        // die großen Munitionsgürtel (10stück)
        //Umweltbewusstsein2
        //großes Schildupgrade                freiwillig
        //Todestorpedo
        //Atombunker
        //DronenKit
        // UplinkTransfer
        // Dieb Transmitter
    }
    if(a==16)
    {
        Liste = [CENR,SLTW,SNSS,CBBS,SCR_,PRE_,CSOC,S_P2,SC_2];
        //Energie Transmitter
        //Guardian Laser
        //Nuclear bomb
        //Battle Fortress Kampfbasis
        //Scorpion
        //Predetor
        //Solar Power Plant
        //verstärkte Power Rüstung
        // Gaus Sniper
    }
    if(a==17)
    {
        Liste = [TSP_];
        // Technosphäre
    }
    if(a==18)
    {
        Liste = [CGC_];
        // Cougar Fregatte
    }
    if(a==19)
    {
        Liste = [MAM_];
        // Mammoth
    }
    if(a==20)
    {
        Liste = [TCN_];
        // Tucan
    }
    if(a==21)
    {
        Liste = [CCD_];
        // Concordia
    }
    if(a==22)
    {
        Liste = [CAN_,ATO2,_B__,LO33,C97Z];
        // Cancer
        // Atomtorpedo
        // Terraforming (soll dann z.b temperatur usw steuern)
        // Alle materialien saugen
        // Wetterstation
    }
    return(Liste);
}
