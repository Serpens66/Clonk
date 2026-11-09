/*-- Scorpionattacken --*/

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
DynMessage("Die Scorpions greifen an!",nil,48,RGBa(27,53,23,nil));
  return(1);
}

/* Eventuell Scorpion erstellen */
private func DoScorpions()
{
  maufeinmal =(ObjectCount(SCA_) ?? 0)+1;
  maktuell= ObjectCount(SCR_);
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pScorpion = CreateObject(SCR_, Random(LandscapeWidth()), 50, -1);
  CreateContents(BRA_,pScorpion);
  SetR(-180, pScorpion);
  return(1);
}
