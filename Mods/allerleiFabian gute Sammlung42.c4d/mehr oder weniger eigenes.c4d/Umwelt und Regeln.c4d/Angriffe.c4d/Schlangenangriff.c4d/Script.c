/*-- Snakeattacken --*/

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
  DynMessage("*schlängel...*",0,48,RGBa(31,209,53,0));
  return(1);
}

/* Eventuell Schlange erstellen */
private func DoSchlange()
{
  maufeinmal = ObjectCount(SNAN)+1 ;
  maktuell= ObjectCount2(Find_ID(SNKE), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pSchlange = CreateObject(SNKE, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pSchlange);
  return(1);
}
