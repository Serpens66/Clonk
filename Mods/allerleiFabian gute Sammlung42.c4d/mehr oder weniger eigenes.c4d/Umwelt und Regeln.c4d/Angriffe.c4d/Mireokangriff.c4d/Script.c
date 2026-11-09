/*-- mireokattacken --*/

#strict 3
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
  DynMessage("Was zum Teufel ist das in der Luft?",nil,48,RGBa(27,53,23,nil));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{
  maufeinmal = (ObjectCount(AN02) ?? 0)+1 ;
  maktuell= ObjectCount2(Find_ID(MROK), Find_OCF(OCF_Alive));
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pMonster = CreateObject(MROK, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);
  return(1);
}
