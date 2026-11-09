/*-- Predatorattacken --*/

#strict
local maufeinmal,maktuell;

func Activate()
{
  MessageWindow(Format("%v Feinde aufeinmal. Aktuell leben %v",maufeinmal,maktuell),Par());
  return(1);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
DynMessage("Predator Invasion!",0,48,RGBa(27,53,23,0));
  return(1);
}

/* Predators */
private func DoPredators()
{
  maufeinmal =ObjectCount(PRA_)+1;
  maktuell= ObjectCount(PRE_);
  if (maktuell>=maufeinmal) return(1);
  var pPredator = CreateObject(PRE_, Random(LandscapeWidth()), 50, -1);
  CreateContents(BRA_,pPredator);
  SetR(-180, pPredator);
  return(1);
}
