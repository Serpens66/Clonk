/*-- Sichtradius appendto neue Gebäude --*/


#strict 3

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





//public func PreDestruction()
//{
//if (FindObject2(Find_AtRect(-1,-1,2,2),Find_ID(_ATW))) {RemoveObject(FindObject2(Find_AtRect(-5,-5,10,20),Find_ID(_ATW)));}
//_inherited();
//}

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(FindObject(SSSI)) return(100);
    return(nil);
}


public func Completion(a,b,c)
{
  SetPlrViewRange(FogViewRange(),this(),1); 
  return(_inherited(a,b,c));
}