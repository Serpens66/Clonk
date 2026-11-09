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
  DynMessage("Hö?",0,48,RGBa(82,168,72,0),300);
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{
  maufeinmal = ObjectCount(MOAN)+1 ;
  maktuell= ObjectCount2(Find_ID(MONS), Find_OCF(OCF_Alive));
  if (maktuell>=maufeinmal) return(1);
  var pMonster = CreateObject(MONS, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);
  return(1);
}
