/*-- Monsterattacken --*/

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
  DynMessage("Kamikaze!",0,48,RGBa(245,66,14,0));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{
  maufeinmal = ObjectCount(KAMA)+1 ;
  maktuell= ObjectCount2(Find_ID(ALN4), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pMonster = CreateObject(ALN4, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);
  return(1);
}
