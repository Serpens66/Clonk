/*-- Sichtradius appendto Raumschiffe --*/


#strict 3

#appendto CVLA
#appendto FLAK
#appendto PRC1
#appendto CAN_
#appendto CCD_
#appendto CGC_
#appendto DRA_
#appendto MAM_
#appendto PRE_
//#appendto PRR_  //reperaturdrohne geht eh nicht
#appendto SCR_
#appendto TSP_
#appendto TCN_
//#appendto SHTL  wird bei switchlights der objekte geregelt
//#appendto TANK

func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(FindObject(SSSI)) return(100);
    return(nil);
}

public func Completion(a,b,c,d)
{
  SetPlrViewRange(FogViewRange(),this(),1); 
return(_inherited(a,b,c,d));
}

public func IsTransferabl() {
  return (1);
}

func IsBulletTarget() {return(1);}