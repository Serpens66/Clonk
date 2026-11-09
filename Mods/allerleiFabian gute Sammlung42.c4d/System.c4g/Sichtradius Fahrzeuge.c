/*-- Sichtradius appendto Fahrzeuge, zusätzlich noch reparierbar und Scahdenssymbol --*/


#strict 2

#appendto LORY
#appendto BALN
#appendto BLMP
#appendto CANN
#appendto CATA
#appendto XBOW
#appendto SLBT
#appendto STMG
#appendto SUB1
#appendto WAGN
#appendto SLBS
#appendto ELEC
#appendto DTXA // saugerlore
#appendto LOR1 // Materialsauger
#appendto LOR3
#appendto LORG
#appendto GRUS // Gerüst
#appendto ST_3 // Magnetssphäre


func FogViewRange() // wird vom Teamview objekt aufgerufen, um anderen nicht-feindlichen Spielern ebenfalls diesen sichtradius mithilfe eines hilfsobjekts zu geben
{
    if(FindObject(SSSI)) return(50);
    return(0);
}


public func Completion()
{
  SetPlrViewRange(FogViewRange(),this(),1); 
}
// gibt wohl ein Minimum von 20 oderso.. egal. 


public func IsRepa() {
  return (1);
}

public func Damage(a,b,c,d,e,f,g)
{
var hp;
var schaden;
hp = GetDefCoreVal("BlastIncinerate","DefCore");
schaden = GetDamage();
if (schaden >= hp) {Incinerate();};
_inherited(a,b,c,d,e,f,g);
}

//transfer via Uplink
public func IsTransferabl() {
  return (1);
}




