/*-- Berserkerattacken --*/

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
  DynMessage("Gigantische Alien Würmer!",0,48,RGBa(83,174,49,0));
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{ 
  maufeinmal = ObjectCount(AN03)+1 ;
  maktuell= ObjectCount2(Find_ID(ALN1));
  if (maktuell>=maufeinmal) return(1);
  // Gebäude der Spieler finden und in der Nähe dieser erstellen
  var building;
  for (building in FindObjects( Find_Category(C4D_Structure) ) )
  {
  if(ObjectCount2(Find_ID(ALN1)) < ObjectCount(AN03)+1 && !Random(3))
  var pMonster = CreateObject(ALN1, GetX(building)+RandomX(-200,200), GetY(building)+RandomX(-80,10), -1);
  }
  return(1);
}
