/*-- Molochattacken --*/

#strict 3
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
DynMessage("Komische Fliegeviecher",nil,48,RGBa(27,53,23,nil));
  return(1);
}

/* Eventuell Moloch erstellen */
private func DoMolochs()
{
  maufeinmal =(ObjectCount(MOA_) ?? 0)+1;
  maktuell= ObjectCount(MOO_);
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pMoloch = CreateObject(MOO_, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMoloch);
  return(1);
}
