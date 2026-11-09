/*-- Gebäude die Ziel des "nur Gebäude" Feuermodus sind (weniger Schaden) --*/

// nicht sicher, ob das so noch aktuell ist... bulletextratarget wird auch in den sichtradius skripten gemacht und ismachine stimmt bei den alten gebäuden ja eher nicht... hmmm

#strict 3


//alte GEbäude

#appendto ANVL
#appendto BRDG
#appendto CTW0
#appendto CST1
#appendto CST2
#appendto CST3
#appendto CHEM
#appendto DRCK
#appendto ELEV
#appendto FNDR
#appendto GIDL
#appendto HUT1
#appendto HUT2
#appendto HUT3
#appendto IDOL
#appendto IGLO
#appendto POWR
#appendto PUMP
#appendto RSRC
#appendto SAWM
#appendto SGNL
#appendto TWR1
#appendto TWR2
#appendto WMIL
#appendto WTWR
#appendto WRKS
#appendto DH8Z
#appendto D_8Z
#appendto F_EL
#appendto ELE_
#appendto AIRL
#appendto CHLP
#appendto SLP1
#appendto AIAL
#appendto FH9I
#appendto GRS1
#appendto GRS3
#appendto LUPU


//neue Gebäude

#appendto SPDM
#appendto CNBS
#appendto CBBS
#appendto CCHF
#appendto CDFT
#appendto CENC
#appendto CFUR
#appendto CGDT
#appendto CH2R
#appendto CHOL
#appendto CPMP
#appendto CREF
#appendto CENR
#appendto CSOC
#appendto CVFC
#appendto CWPP
#appendto SAPP
#appendto SBTR
#appendto SCNB
#appendto SCHM
#appendto SDRK
#appendto SDWU
#appendto SFDY
#appendto SGEO
#appendto SLTW
#appendto SLD1
#appendto SMFC
#appendto SMBS
#appendto STTM
#appendto STOW
#appendto STRM
#appendto SWEF
#appendto SWEG
#appendto CFCP
#appendto CH2P
#appendto CPBX
#appendto TELE

#appendto GSTA



public func IsBulletExtraTarget(){ return(true); }
public func IsMachine(){ return(true); }

