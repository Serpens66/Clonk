/*-- Berserkerattacken --*/

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
  DynMessage("Gigantische Alien Würmer!",nil,48,RGBa(83,174,49,nil));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{ 
  maufeinmal = (ObjectCount(AN03) ?? 0)+1 ;
  maktuell= ObjectCount2(Find_ID(ALN1));
  if ((maktuell ?? 0)>=(maufeinmal ?? 0)) return(1);
  // Gebäude der Spieler finden und in der Nähe dieser erstellen
  var building;
  for (building in FindObjects( Find_Category(C4D_Structure) ) )
  {
  if(And((ObjectCount2(Find_ID(ALN1)) ?? 0) < (ObjectCount(AN03) ?? 0)+1, !Random(3)))
  var pMonster = CreateObject(ALN1, (GetX(building) ?? 0)+(RandomX(-200,200) ?? 0), (GetY(building) ?? 0)+(RandomX(-80,10) ?? 0), -1);
  }
  return(1);
}
