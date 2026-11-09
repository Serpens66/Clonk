/*-- in neuer Productionfabrik herstellbar, also refinery --*/

// Raffinerie stellt all diese Produkte her und auch IsAnvilProduct

#strict 3
#appendto JTPK
#appendto HSHD
#appendto LMIN
#appendto SMI1
#appendto LAMP
#appendto MEDI
#appendto KNIK
#appendto NVDN
#appendto DRSU
#appendto KLAS
#appendto KSLM
#appendto KRFL

#appendto AIRT
#appendto HARM
#appendto FLSH
#appendto BETL


// #appendto STAP  // Munitionskisten , sollen in waffenfabrik herstellbar sein.
// #appendto ENAP
// #appendto GRAP
// #appendto GSAP
// #appendto MIAP
// #appendto PTAP
// #appendto STLM 
// #appendto SHMU
// #appendto SNI_ 


#appendto MINE
#appendto AIRS
#appendto DK8Z
#appendto E78Z
#appendto AM5P
#appendto CU8Z
#appendto TRP1  //Torpedo
#appendto TRP2
#appendto ARWP
#appendto XARP
#appendto FARP
#appendto TAAZ
#appendto THLM
#appendto FLAG

#appendto SB52 // Verfolger Bombe
#appendto GOTA // Tac
#appendto AMIN // Alienmine
#appendto STRH // Partikelkonverter
#appendto GRN2 // Granate
#appendto RKB2 // silv raketen
#appendto SEN6 // Sense
#appendto AXE7 // Wurfaxt
#appendto GOBE // Betäubungsgranate
#appendto WREJ // EJD 
#appendto KSDL // Sattel
#appendto WRBO // besserer bumerang
#appendto SPER // Speer
#appendto LZD_ // LZD
#appendto _BLG // Blendgranate
#appendto KC4K // Schlüsselkarte fuer sensoren
#appendto BTWP // Schweißbrenner
#appendto CNBF // Akku
#appendto LMI_ // Fakelasermine
#appendto MIN_ // Fakemine
#appendto SMI_ // fakeschwarmmine
#appendto _Y3B // alter Feuerlöscher
#appendto SFEX // neuer feuerlöscher
#appendto UPLI // Uplink
#appendto _SKT // Schildupgrade klein
#appendto 2SKT // schildupgrade groß
#appendto JB4K // Jetbelt
#appendto TB5B // neue zelttasche
#appendto MFUS // bewegungszünder plastiksprengstoff
#appendto SFUS  // Zeitzünder plastiksprengstoff
#appendto C40_ // Plastiksprengstoff





public func IsRefineryProduct() { return(1); }

