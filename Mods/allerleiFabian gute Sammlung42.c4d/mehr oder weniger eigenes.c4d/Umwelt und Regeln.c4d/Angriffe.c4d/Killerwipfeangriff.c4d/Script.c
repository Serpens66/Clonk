/*-- Killerwipfeattacken --*/

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
  DynMessage("Wie kuschlig, au, die beißen!",0,48,RGBa(83,0,49,0));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{ 
  maufeinmal = ObjectCount(AN04)+1 ;
  maktuell= ObjectCount2(Find_ID(WIPF), Find_OCF(OCF_Alive),Find_Func("IsThreat"));
  if (maktuell>=maufeinmal) return(1);
  var pMonster = CreateObject(WIPF, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);
  pMonster->MakeKillerWipf();
  return(1);
}
