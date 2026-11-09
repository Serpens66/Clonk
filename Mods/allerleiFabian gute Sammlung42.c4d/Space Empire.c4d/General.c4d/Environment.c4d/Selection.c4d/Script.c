// 2do:
//  GameOptions():  "if (count == 2)"


/*-- Seitenauswahl --*/

/*  Aufgaben dieses Objekts:
 *   - Umweltsteuerungsobjekt in den Szenarioeigenschaften
 *   - Erzeugung der grafischen Seitenauswahlobjekte
 *   - Erzeugung und Überwachung der Einstellungsmenüs
 */

#strict 3

local count, caller;


protected func Initialize()
{
  SetAction("Init");
}

private func PostInit()
{
  // andere Objekte dieser Art löschen
  count = 1;
  var obj;
  while (obj = FindObject(GetID()))
  {
    S42MigrationStep(count, 1);
    RemoveObject(obj);
  }
  return(Check());
}


/* ActionCall: für jeden beigetretenen Spieler Seitenauswahl erzeugen */

protected func Check()
{
  var i, player;
  for (i = nil; (i ?? 0) < (GetPlayerCount() ?? 0); S42MigrationStep(i, 1))
  {
    player = GetPlayerByIndex(i);
    if (!FindObjectOwner(SEL1, player))
      CreateObject(SEL1,nil,nil,player);
  }
}


/* Spieleinstellungen */

protected func GameOptions()
{
  // wenn Objekt 2x ausgewählt, gleich weiter
  if ((count ?? 0) <= 2)
    return(1);
  // alle außer Spieler 1 müssen warten
  if (!Equal(GetOwner(), GetPlayerByIndex(nil)))
  {
    SetAction("Wait");
    return(1);
  }
  // Objekt unsichtbar machen
  SetVisibility(VIS_None);
  // erster Spieler macht die Einstellungen
  caller = GetHiRank(GetOwner());
  CreateMenu(HZCK,caller,nil,nil,"Spieleinstellungen",nil,nil,1);
  AddMenuItem("Details...",    "AcceptOptions", SYAC, caller, nil,nil, "Spiel mit diesen Einstellungen starten.");
  AddMenuItem("Spiel starten", "AcceptOptions", SYAC, caller, nil,nil, "Spiel mit diesen Einstellungen starten.");
  return(ShowSettings());
}

private func CheckOptions()
{
  // Menü erzeugen, wenn keins da
  if (!GetMenu(caller))
    GameOptions();
}

private func AcceptOptions()
{
  CloseMenu(caller);
}


/* Anzeige */

private func CountGoals()
{
  // Zahl der gewählten Spielziele
  var goal, count;
  while (goal = FindObject(nil, nil,nil,nil,nil, nil,nil,nil,nil, goal))
    if ((GetCategory(goal) ?? 0) & (C4D_Goal ?? 0))
      S42MigrationStep(count, 1);
  return(count);
}

private func GetGoal(goal)
{
  // Zahl der gewählten Spielziele
  while (goal = FindObject(nil, nil,nil,nil,nil, nil,nil,nil,nil, goal))
    if ((GetCategory(goal) ?? 0) & (C4D_Goal ?? 0))
      return(goal);
  return(nil);
}

private func GetGoalDesc(goal)
{
  var str = GetName(goal);
  var ID = GetID(goal);
  if (Or(Or(Or(Or((Equal(ID, GGMI)), (Equal(ID, GCMI))), (Equal(ID, GOMI))), (Equal(ID, GUMI))), (Equal(ID, GTMI))))
    str = Format("%s (%d%)", str, 100-((str->Percentage(100)) ?? 0));
  return(str);
}

protected func ShowSettings()
{
  var str = "";
  // Spielziele
  var count = CountGoals();
  if (Equal(count, nil))
    str = "spätere Spielzielauswahl|";
  else if (Equal(count, 1))
    str = Format("Spielziel:  %s|", GetGoalDesc(GetGoal()));
  else
  {
    str = "Spielziele:|";
    // ...
    
    
  }
  Message(str);
}

