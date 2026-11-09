/*-- Vogelattacken --*/

#strict
local maufeinmal,maktuell;

func Activate()
{
  MessageWindow(Format("%v Feinde aufeinmal. Aktuell leben %v",maufeinmal,maktuell),Par());
  return(1);
}

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  DynMessage("*Löcher pick*",0,48,RGBa(220,146,20,0));
  return(1);
}

/* Eventuell Vogel erstellen */
private func DoVogel()
{
  maufeinmal = ObjectCount(VOAN)+1 ;
  maktuell= ObjectCount2(Find_ID(BIRD), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pVogel = CreateObject(BIRD, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pVogel);
  return(1);
}
