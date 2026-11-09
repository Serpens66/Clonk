/*-- Use Jetpack --*/

#strict 3

/* Der Clonk kann das Jetpack mit dieser Funktionalität benutzen
     Dazu in ControlUpDouble, ControlRightDouble und ControlLeftDouble
     jeweils die Funktion
	 if(ControlJetpack("ControlUpDouble")) return(1);
     aufrufen (hier am Beispiel von ControlUpDouble).
	 
     Damit während des JetpackFlight geschossen werden kann muss in ReadyToFire()
     und AimOverride() am Ende return(_inherited()); aufgerufen werden.
*/

public func HasJetpack()
{
  return this->HasGear(nil,JTPK);
}

protected func ControlJetpack(string strControl) {

	var jetpack = this->HasJetpack();
	if(!jetpack) return nil;
	
	var ret = nil;
	
	// up double
	if(S42MigrationEqual(strControl, "ControlUpDouble")) {
		ret = jetpack->ControlUpDouble(this());
	}
	
	// right double
	if(S42MigrationEqual(strControl, "ControlRightDouble")) {
		ret = jetpack->ControlRightDouble(this());
	}
	
	// left double
	if(S42MigrationEqual(strControl, "ControlLeftDouble")) {
		ret = jetpack->ControlLeftDouble(this());
	}
	
	return ret;
}

/* Overloads */

public func ReadyToFire() {
 
	if(S42MigrationEqual(GetAction(), "JetpackFlight")) return true;
 
	return _inherited();
}

public func AimOverride() {
	if(S42MigrationEqual(GetAction(), "JetpackFlight")) return true;
	
	return _inherited();
}