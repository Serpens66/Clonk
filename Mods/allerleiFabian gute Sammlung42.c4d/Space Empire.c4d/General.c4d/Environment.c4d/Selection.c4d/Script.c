// 2do:
//  GameOptions():  "if (count == 2)"


/*-- Seitenauswahl --*/

/*  Aufgaben dieses Objekts:
 *   - Umweltsteuerungsobjekt in den Szenarioeigenschaften
 *   - Erzeugung der grafischen Seitenauswahlobjekte
 *   - Erzeugung und Überwachung der Einstellungsmenüs
 */

#strict

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
    ++count;
    RemoveObject(obj);
  }
  return(Check());
}


/* ActionCall: für jeden beigetretenen Spieler Seitenauswahl erzeugen */

protected func Check()
{
  var i, player;
  for (i = 0; i < GetPlayerCount(); ++i)
  {
    player = GetPlayerByIndex(i);
    if (!FindObjectOwner(SEL1, player))
      CreateObject(SEL1,0,0,player);
  }
}


/* Spieleinstellungen */

protected func GameOptions()
{
  // wenn Objekt 2x ausgewählt, gleich weiter
  if (count <= 2)
    return(1);
  // alle außer Spieler 1 müssen warten
  if (GetOwner() != GetPlayerByIndex(0))
  {
    SetAction("Wait");
    return(1);
  }
  // Objekt unsichtbar machen
  SetVisibility(VIS_None());
  // erster Spieler macht die Einstellungen
  caller = GetHiRank(GetOwner());
  CreateMenu(HZCK,caller,0,0,"Spieleinstellungen",0,0,1);
  AddMenuItem("Details...",    "AcceptOptions", SYAC, caller, 0,0, "Spiel mit diesen Einstellungen starten.");
  AddMenuItem("Spiel starten", "AcceptOptions", SYAC, caller, 0,0, "Spiel mit diesen Einstellungen starten.");
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
  while (goal = FindObject(0, 0,0,0,0, 0,0,0,0, goal))
    if (GetCategory(goal) & C4D_Goal())
      ++count;
  return(count);
}

private func GetGoal(goal)
{
  // Zahl der gewählten Spielziele
  while (goal = FindObject(0, 0,0,0,0, 0,0,0,0, goal))
    if (GetCategory(goal) & C4D_Goal())
      return(goal);
  return(0);
}

private func GetGoalDesc(goal)
{
  var str = GetName(goal);
  var ID = GetID(goal);
  if ((ID == GGMI) || (ID == GCMI) || (ID == GOMI) || (ID == GUMI) || (ID == GTMI))
    str = Format("%s (%d%)", str, 100-(str->Percentage(100)));
  return(str);
}

protected func ShowSettings()
{
  var str = "";
  // Spielziele
  var count = CountGoals();
  if (count == 0)
    str = "spätere Spielzielauswahl|";
  else if (count == 1)
    str = Format("Spielziel:  %s|", GetGoalDesc(GetGoal()));
  else
  {
    str = "Spielziele:|";
    // ...
    
    
  }
  Message(str);
}

