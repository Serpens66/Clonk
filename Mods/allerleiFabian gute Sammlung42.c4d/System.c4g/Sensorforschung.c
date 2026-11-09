/*-- Sensor Forschung --*/

//damit es reicht ein Objekt zu erforschen

#strict

//die sensorteile
#appendto CS4K   
#appendto PS4K
#appendto BS4K
#appendto LS4K
#appendto SS4K
#appendto WS4K
#appendto GTE1
#appendto KC4K



public func GetResearchLink() { return(SSEE); } //wenn das erforscht, sind alle appendtos erforscht.
public func GetResearchBase2() {return(GetID());}   // sich selbst als voraussetzung, damits nicht direkt erforscht werden kann.  GetResearchBase() wird im forschungsbaumskript überschrieben, deswegen 1 nehmen.