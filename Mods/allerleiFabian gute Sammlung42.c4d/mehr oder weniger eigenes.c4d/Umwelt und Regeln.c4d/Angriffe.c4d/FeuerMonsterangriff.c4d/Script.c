/*-- FeuerMonsterattacken --*/

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
  DynMessage("Hö?*Feuerspei*",0,48,RGBa(189,60,53,0));
  return(1);
}

/* Eventuell FeuerMonster erstellen */
private func DoFeuerMonsters()
{
  maufeinmal = ObjectCount(FEAN)+1 ;
  maktuell= ObjectCount2(Find_ID(FMNS), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pFireMonster = CreateObject(FMNS, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pFireMonster);
  return(1);
}
