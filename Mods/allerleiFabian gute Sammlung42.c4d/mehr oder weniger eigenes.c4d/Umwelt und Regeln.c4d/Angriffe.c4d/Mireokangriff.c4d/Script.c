/*-- mireokattacken --*/

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
  DynMessage("Was zum Teufel ist das in der Luft?",0,48,RGBa(27,53,23,0));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{
  maufeinmal = ObjectCount(AN02)+1 ;
  maktuell= ObjectCount2(Find_ID(MROK), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pMonster = CreateObject(MROK, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);
  return(1);
}
