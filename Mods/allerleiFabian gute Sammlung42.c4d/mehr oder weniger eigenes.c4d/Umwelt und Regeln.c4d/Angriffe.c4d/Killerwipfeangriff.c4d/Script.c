/*-- Killerwipfeattacken --*/

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
  DynMessage("Wie kuschlig, au, die beißen!",nil,48,RGBa(83,nil,49,nil));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{ 
  maufeinmal = (ObjectCount(AN04) ?? 0)+1 ;
  maktuell= ObjectCount2(Find_ID(WIPF), Find_OCF(OCF_Alive),Find_Func("IsThreat"));
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  var pMonster = CreateObject(WIPF, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);
  pMonster->MakeKillerWipf();
  return(1);
}
