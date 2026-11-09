/*-- Spielzielsteuerung --*/

#strict

local unfinished;   // Anzahl der Spielziele, die NICHT erfüllt werden müssen


/* Initialisierung */

protected func Initialize()
{
  // ist das hier GOAL selber? (also nicht #included)
  if (GetID() == GOAL)
  {
    // dringend benötigte Objekte erstellen
    CreateObject(SFXM,0,0,-1);   // Effektmanager
    CreateObject(STWL,0,0,-1);   // Warenliste für Handelssystem
    // Warteaktion
    return(SetAction("Init"));
  }
  // GOAL-Objekt mit Timer erstellen
  if (!FindObject(GOAL))
    CreateObject(GOAL,0,0,-1);
}

protected func PostInit()   // Nachinitialisierung für GOAL
{
  // alle Spielziele müssen erfüllt werden
  unfinished = 0;
  // ein CheckFulfilled-Broadcast an alle
  var goal;
  while (goal = FindObject(0, 0,0,0,0, 0, 0,0, 0, goal))
    if (GetCategory(goal) & C4D_Goal())
      goal->~IsFulfilled();
}


/* Timer */

private func EnoughGoals(player)
{
  // Alle Spielziele für einen Spieler überprüfen
  var goal, count, restgoals;
  while (goal = FindObject(0, 0,0,0,0, 0,0,0,0, goal))
    if (GetCategory(goal) & C4D_Goal())
      {
        ++count;
        if (!(goal->~IsFulfilled(player)))
          ++restgoals;
      }
  // Mindestanzahl der Spielziele erfüllt?
  return((restgoals <= unfinished) && count);
}

protected func CheckTime()
{
  // alle Spieler überprüfen
  var i, boool;
  var j = GetPlayerCount();
  for (i; i < j; ++i) 
    if (EnoughGoals(GetPlayerByIndex(i)))
    {
      // da hat einer genug Spielziele erfüllt
      boool = 1;
      break;
    }
  if (!boool) return(1);
  // Ziel erfüllt: Vom Szenario abgefangen?
  if (GameCall("OnGoalsFulfilled"))
    return(RemoveObject());
  // alle "Verlierer" rausschmeissen
  for (i; i < j; ++i) 
    if (!EnoughGoals(GetPlayerByIndex(i)))
      EliminatePlayer(GetPlayerByIndex(i));   // tja, leider verloren
  // die restlichen haben gewonnen!
  Sound("Trumpet",1);
  // jetzt ein wenig warten, und dann GameOver
  SetAction("Wait4End");
}

protected func RoundOver()
{
  // Das wars :/
  GameOver();
  return(RemoveObject());
}


/* Object-Call */

public func SetGoal_Unfinished(unf)
{
  unfinished = unf;
}


/* überladbare Defaultfunktionen */

public func IsFulfilled()
{
  return(1);
}

protected func Activate(player)   // Defaultnachrichten
{
  if (IsFulfilled())
    return(MessageWindow("Spielziel erfüllt!", player));
  return(MessageWindow(GetDesc(), player));
}

