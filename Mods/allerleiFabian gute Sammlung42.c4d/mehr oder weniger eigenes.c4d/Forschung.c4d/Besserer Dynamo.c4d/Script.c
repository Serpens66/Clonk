/*-- Verbessertet Dynamo --*/

#strict 3

// wird direkt in Dynamo Objekt gemacht

func GetResearchTime(pBuilding,pClonk) {return(100);} // wird nur verwendet, wenn szenarioforschung aktiv, standardksoten einer Forshcung, wenn nichts returned wird sind dann 50
func GetResearchCosts(pBuilding,pClonk) {return(10);} // wird nur verwendet, wenn szenarioforschung aktiv, standardksoten einer Forshcung, wenn nichts returned wird sind dann 5, ansonsten wird der Wert anhand der Forschungsstufe bestimmt