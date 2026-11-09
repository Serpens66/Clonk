/*-- Spielzielauswahl --*/

#strict

local sel, caller;


/* Initialisierung */

protected func Initialize()
{
  // anderes Objekt vorhanden? Sich selbst löschen!
  if (FindObject(GetID()))
    return(RemoveObject());
  SetAction("Remind");
}

protected func Remind()
{
  if (CountGoals())
    return(Log("Spielzielauswahl noch nicht abgeschlossen!"));
  return(Log("Noch kein Spielziel ausgewählt!"));
}


/* Hilfsfunktionen */

private func CountGoals()
{
  // Zahl der gewählten Spielziele
  var goal, count;
  while (goal = FindObject(0, 0,0,0,0, 0,0,0,0, goal))
    if (GetCategory(goal) & C4D_Goal())
      if (GetID(goal) != GetID())
        ++count;
  return(count);
}

public func ChangeGoal(obj, caller)
{
  RemoveObject(obj);
  Activate(caller);
}


/* Hauptmenü */

protected func Activate(player)
{
  caller = GetCursor(player);
  return(Select_Next());
}

private func Select_Next()
{
  // Menü öffnen
  CreateMenu(GetID(),caller,0,0,"Spielzielauswahl",0,1);
  if (GetPlayerCount() > 1)
    AddMenuItem("Melee",      "Select_Melee",  MELE, caller, 0,0, "Ein Kampf auf Leben und Tod, wahlweise auch in Teams.");
  AddMenuItem("Punktejagd",   "Select_Points", GVAL, caller, 0,0, "Wer zuerst die festgelegte Anzahl Punkte erreicht, hat gewonnen.");
  AddMenuItem("Rohstoffabbau","Select_Mine",   GGMI, caller, 0,0, "Abbau von Gold, Öl, Erzen, usw.");
  AddMenuItem("Kooperativ",   "Select_Coop",   GHNT, caller, 0,0, "Monster- und Alienjagd oder kooperatives Siedeln.");
  AddMenuItem("Endlosspiel",  "Select_Endless",GENL, caller, 0,0, "Einfach mal so drauflosspielen.");
  if (CountGoals())
  {
    AddMenuItem("Ziel löschen","Select_Delete",SYDL, caller, 0,0, "Zum Löschen unliebsamer Spielziele.");
    AddMenuItem("Bestätigen",  "Select_Done",  SYAC, caller, 0,0, "Ziele akzeptieren und Zielauswahl beenden.");
  }
}


/* Melee */

private func Select_Melee()
{
  CreateMenu(MELE,caller,0,0,"Melee",0,1);
  AddMenuItem("Kill them all",   "Select_Melee2",MELE, caller, 0,0, "Das Ziel ist, alle gegnerischen Clonks umzubringen.");
  AddMenuItem("Capture the Flag","Select_Melee2",CTFL, caller, 0,0, "Es reicht, die Flaggen eines Gegners zu verkaufen.");
  AddMenuItem("Zurück",          "Select_Next",  SYCC, caller, 0,0, "Zurück zum Hauptmenü.");
}

private func Select_Melee2(goal)
{
  // Ergebnis der vorherigen Auswahl festhalten
  if (goal == MELE) sel = 1;
  if (goal == CTFL) sel = 2;
  // wenn nur zwei Spieler, gleich weiter
  if (GetPlayerCount() < 3)
    return(Select_Melee3(MELE));
  // Menü erstellen
  CreateMenu(MELE,caller,0,0,"Melee",0,1);
  AddMenuItem("Jeder gegen jeden","Select_Melee3",MELE, caller, 0,0, "Alle Gegner müssen besiegt werden.");
  AddMenuItem("Team-Melee",       "Select_Melee3",MEL2, caller, 0,0, "Zwei Spieler können die Runde überleben.");
  AddMenuItem("Zurück",           "Select_Melee", SYCC, caller, 0,0, "Zum vorherigen Menü.");
}

private func Select_Melee3(goal)
{
  // Welche Art von Melee?
  CreateObject(goal,0,0,-1);
  if (sel == 2)
  {
    CreateObject(CTFL,0,0,-1);
    Log("Neues Spielziel: %s CTF", GetName(0,goal));
    return(Select_Next());
  }
  Log("Neues Spielziel: %s", GetName(0,goal));
  return(Select_Next());
}


/* Punktejagd */

private func Select_Points()
{
  CreateMenu(GVAL,caller,0,0,"Punktejagd",0,1);
  AddMenuItem("Siedlungspunkte","Select_Value",GVAL, caller, 0,0, "Hier ist Expansion bis zum Umfallen gefragt.");
  AddMenuItem("Geldpunkte",     "Select_Money",GMNY, caller, 0,0, "Verdiene soviel Geld wie möglich.");
  AddMenuItem("Zurück",         "Select_Next", SYCC, caller, 0,0, "Zum Hauptmenü.");
  return(1);
}

private func Select_Value()
{
  CreateMenu(GVAL,caller);
  AddMenuItem("400 Siedlungspunkte", "Select_Points2", GVAL, caller, 400, 4, "",2,1);
  AddMenuItem("600 Siedlungspunkte", "Select_Points2", GVAL, caller, 600, 6, "",2,1);
  AddMenuItem("800 Siedlungspunkte", "Select_Points2", GVAL, caller, 800, 8, "",2,1);
  AddMenuItem("1200 Siedlungspunkte","Select_Points2", GVAL, caller,1200,12, "",2,1);
  AddMenuItem("2000 Siedlungspunkte","Select_Points2", GVAL, caller,2000,20, "",2,1);
  AddMenuItem("3000 Siedlungspunkte","Select_Points2", GVAL, caller,3000,30, "",2,1);
  AddMenuItem("5000 Siedlungspunkte","Select_Points2", GVAL, caller,5000,50, "",2,1);
  AddMenuItem("Zurück","Select_Points",SYCC,caller);
}

private func Select_Money()
{
  CreateMenu(GMNY,caller);
  AddMenuItem("400 Geldpunkte", "Select_Points2", GMNY, caller, 400, 4, "",2,1);
  AddMenuItem("600 Geldpunkte", "Select_Points2", GMNY, caller, 600, 6, "",2,1);
  AddMenuItem("800 Geldpunkte", "Select_Points2", GMNY, caller, 800, 8, "",2,1);
  AddMenuItem("1000 Geldpunkte","Select_Points2", GMNY, caller,1000,10, "",2,1);
  AddMenuItem("1500 Geldpunkte","Select_Points2", GMNY, caller,1500,15, "",2,1);
  AddMenuItem("2500 Geldpunkte","Select_Points2", GMNY, caller,2500,25, "",2,1);
  AddMenuItem("Zurück","Select_Points",SYCC,caller);
}

private func Select_Points2(goal, count)
{
  var i;
  for (i = 1; i <= count; ++i)
    CreateObject(goal,0,0,-1);
  Log("Neues Spielziel: %s", GetName(0,goal));
  return(Select_Next());
}


/* Rohstoffabbau */

private func Select_Mine()
{
  CreateMenu(GHNT,caller,0,0,"Rohstoffabbau",0,1);
  // Materialzählung noch nicht fertig
  if (GetMaterialCount(Material("Gold")) == -1)        AddMenuItem("Hinweis","Select_Hint", INFO, caller, 0,0, "Kleiner Hinweis.");
  // Rohstoffabbau
  if (GetMaterialCount(Material("Gold")) > 500)        AddMenuItem("Gold abbauen",    "Select_Mine2", GGMI, caller, 0,0, "Einen bestimmten Prozentsatz der Goldvorkommen abbauen.");
  if (GetMaterialCount(Material("Crystal")) > 500)     AddMenuItem("Kristall abbauen","Select_Mine2", GCMI, caller, 0,0, "Der angegebene Anteil des Kristalls muss abgebaut werden.");
  if (GetMaterialCount(Material("Ore")) > 500)         AddMenuItem("Erz abbauen",     "Select_Mine2", GOMI, caller, 0,0, "Baue einen bestimmten Anteil der Erzvorkommen ab.");
  if (GetMaterialCount(Material("Uranore")) > 500)     AddMenuItem("Uranerz abbauen", "Select_Mine2", GUMI, caller, 0,0, "Vom Uranerz die festgelegte Menge abernten.");
  if (GetMaterialCount(Material("TitaniumOre")) > 500) AddMenuItem("Titanerz abbauen","Select_Mine2", GTMI, caller, 0,0, "Eine bestimmte Menge Titanerz muss abgebaut werden.");
  AddMenuItem("Zurück","Select_Next", SYCC, caller, 0,0, "Zum Hauptmenü.");
}

private func Select_Mine2(goal)
{
  CreateMenu(goal,caller);
  AddMenuItem("Abbau: alles","Select_Points2", goal, caller,100,1);
  AddMenuItem("Abbau: 90%%", "Select_Points2", goal, caller, 90,2);
  AddMenuItem("Abbau: 75%%", "Select_Points2", goal, caller, 75,3);
  AddMenuItem("Abbau: 50%%", "Select_Points2", goal, caller, 50,4);
  AddMenuItem("Abbau: 25%%", "Select_Points2", goal, caller, 25,5);
  AddMenuItem("Abbau: 10%%", "Select_Points2", goal, caller, 10,6);
  AddMenuItem("Zurück","Select_Coop", SYCC, caller);
}

private func Select_Hint()
{
  MessageWindow("Die Rohstoffabbau-Ziele sind noch nicht verfügbar. Warte einfach mal kurz...", GetController(caller));
}



/* Kooperativ */

private func Select_Coop()
{
  CreateMenu(GHNT,caller,0,0,"Kooperativ",0,1);
  if (GetPlayerCount() > 1)
    AddMenuItem("Siedlung kooperativ","Select_ValueGain", GSET,caller, 0,0, "Hier ist Zusammenarbeit aller Spieler gefragt.");
  AddMenuItem("Alien/Monsterjagd",    "Select_Hunt",      GHNT,caller, 0,0, "Alle Monster, Aliens und gefährliches Getier müssen beseitigt werden.");
  AddMenuItem("Zurück","Select_Next", SYCC, caller, 0,0, "Zum Hauptmenü.");
}

private func Select_ValueGain()
{
  CreateMenu(GSET,caller);
  AddMenuItem("500 Siedlungspunkte",  "Select_Points2", GSET, caller,  500, 1, "",2,1);
  AddMenuItem("1000 Siedlungspunkte", "Select_Points2", GSET, caller, 1000, 2, "",2,1);
  AddMenuItem("1500 Siedlungspunkte", "Select_Points2", GSET, caller, 1500, 3, "",2,1);
  AddMenuItem("2000 Siedlungspunkte", "Select_Points2", GSET, caller, 2000, 4, "",2,1);
  AddMenuItem("4000 Siedlungspunkte", "Select_Points2", GSET, caller, 4000, 8, "",2,1);
  AddMenuItem("8000 Siedlungspunkte", "Select_Points2", GSET, caller, 8000,16, "",2,1);
  AddMenuItem("15000 Siedlungspunkte","Select_Points2", GSET, caller,15000,30, "",2,1);
  AddMenuItem("25000 Siedlungspunkte","Select_Points2", GSET, caller,25000,50, "",2,1);
  AddMenuItem("Zurück","Select_Coop", SYCC, caller);
}

private func Select_Hunt()
{
  CreateObject(GHNT,0,0,-1);
  Log("Neues Spielziel: Monsterjagd");
  return(Select_Next());
}


/* Endlosspiel */

private func Select_Endless(goal)
{
  CreateObject(goal,0,0,-1);
  Log("Keine Spielziele mehr");
}


/* Ziel löschen */

private func Select_Delete()
{
  // neues Menü
  CreateMenu(SYDL,caller);
  var goal;
  while (goal = FindObject(0, 0,0,0,0, 0,0,0,0, goal))
    if (GetCategory(goal) & C4D_Goal())
      if (GetID(goal) != GetID())
        AddMenuItem(Format("%s löschen",GetName(goal)), "Select_Delete2",GetID(goal),caller,0,goal);
  AddMenuItem("Zurück","Select_Next",SYCC,caller);
  return(1);
}

private func Select_Delete2(par0, goal)
{
  Log("Spielziel gelöscht: %s", GetName(goal));
  RemoveObject(goal);
  if (CountGoals())
    return(Select_Delete());
  return(Select_Next());
}


/* Bestätigen */

private func Select_Done()
{
  var goals = Min(CountGoals(),9);
  if (goals < 1)  return(0);
  if (goals == 1) return(Select_Done2());
  // neues Menü
  CreateMenu(GetID(),caller);
  var i;
  while (++i < goals)
    AddMenuItem(Format("%d von %d Zielen zu erfüllen",i,goals), "Select_Done3",SYNU,caller, 0,i, 0,2,i);
  AddMenuItem("Alle Ziele zu erfüllen", "Select_Done3",SYNU,caller, 0,goals, 0,2,goals);
  AddMenuItem("Zurück", "Select_Next", SYCC,caller, 0,0, "Zum Hauptmenü.");
}

private func Select_Done2()
{
  // Ja/Nein-Menü
  CreateMenu(GetID(),caller);
  AddMenuItem("Bestätigen","Select_Done4",SYAC,caller, 0,0, "Spielzielauswahl abschließen.");
  AddMenuItem("Zurück",    "Select_Next", SYCC,caller, 0,0, "Zum Hauptmenü.");
}

private func Select_Done3(par0, count)
{
  // Bestätigung mehrerer Ziele
  FindObject(GOAL)->SetGoal_Unfinished(CountGoals()-count);
  if (count == 1) Log("%d Spielziele bestätigt, eins davon muss erfüllt werden", CountGoals());
             else Log("%d Spielziele bestätigt, %d davon müssen erfüllt werden", CountGoals(),count);
  return(RemoveObject());
}

private func Select_Done4()
{
  // Bestätigung eines Spielziels
  FindObject(GOAL)->SetGoal_Unfinished(0);
  Log("Spielziel bestätigt");
  return(RemoveObject());
}

