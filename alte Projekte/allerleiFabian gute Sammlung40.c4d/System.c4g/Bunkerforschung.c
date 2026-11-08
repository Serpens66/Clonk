/*-- Bunker Forschung --*/

//damit es reicht ein Objekt davon zu erforschen, um alle Bunker und DekoTeile erforscht zu haben.

#strict

//die Bunkerteile
#appendto BT5C   
#appendto BE5C
#appendto HL0D
#appendto HR0D
#appendto BH5C
#appendto RR5C
#appendto B15C
#appendto HR6Y
#appendto BG5C
#appendto BL5C
#appendto BR5C
#appendto BW5C
#appendto CH5C
#appendto BP5C
//jetzt die Dekoobjekte
#appendto VENT
#appendto MVNT
#appendto SCR2
#appendto SCR1
#appendto HSGN
#appendto CLVM
#appendto LCKR
#appendto SPVM
#appendto PLNT
#appendto PLT2
#appendto PSTR
#appendto CGLO
#appendto MPPJ
#appendto LBPC
#appendto GSBO
#appendto GSTA
#appendto BLG3 //hintergrundlicht
#appendto CLG3 //Deckenlicht
#appendto GADG // deko apparaturen


public func GetResearchLink() { return(BBUN); } //wenn das erforscht, sind alle appendtos erforscht.
public func GetResearchBase2() {return(GetID());}   // sich selbst als voraussetzung, damits nicht direkt erforscht werden kann.  GetResearchBase() wird im forschungsbaumskript überschrieben, deswegen 2 nehmen.