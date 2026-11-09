/*-- Monsterattacken --*/

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
  DynMessage("Jump! Jump!",nil,48,RGBa(31,192,43,nil));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{
  maufeinmal = (ObjectCount(JUMA) ?? 0)+1 ;
  maktuell= ObjectCount2(Find_ID(ALN2), Find_OCF(OCF_Alive));
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pMonster = CreateObject(ALN2, Random(LandscapeWidth()), 50, NO_OWNER);
  SetR(-180, pMonster);
  return(1);
}


