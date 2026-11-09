/*-- Phoenixattacken --*/

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
  DynMessage("*Ein Feuerschweif am Himmel*",0,48,RGBa(233,115,7,0));
  return(1);
}

/* Eventuell Vogel erstellen */
private func DoVogel()
{
  maufeinmal = ObjectCount(PH_A)+1 ;
  maktuell= ObjectCount2(Find_ID(PHNX), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pVogel = CreateObject(PHNX, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pVogel);
  return(1);
}
