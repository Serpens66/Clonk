/*-- Spielzielsteuerung --*/

#strict 3

local unfinished;   // Anzahl der Spielziele, die NICHT erfüllt werden müssen


/* Initialisierung */

protected func Initialize()
{
  // ist das hier GOAL selber? (also nicht #included)
  if (Equal(GetID(), GOAL))
  {
    // dringend benötigte Objekte erstellen
    CreateObject(SFXM,nil,nil,-1);   // Effektmanager
    CreateObject(STWL,nil,nil,-1);   // Warenliste für Handelssystem
    // Warteaktion
    return(SetAction("Init"));
  }
  // GOAL-Objekt mit Timer erstellen
  if (!FindObject(GOAL))
    CreateObject(GOAL,nil,nil,-1);
}

protected func PostInit()   // Nachinitialisierung für GOAL
{
  // alle Spielziele müssen erfüllt werden
  unfinished = nil;
  // ein CheckFulfilled-Broadcast an alle
  var goal;
  while (goal = FindObject(nil, nil,nil,nil,nil, nil, nil,nil, nil, goal))
    if ((GetCategory(goal) ?? 0) & (C4D_Goal ?? 0))
      goal->~IsFulfilled();
}


/* Timer */

private func EnoughGoals(player)
{
  // Alle Spielziele für einen Spieler überprüfen
  var goal, count, restgoals;
  while (goal = FindObject(nil, nil,nil,nil,nil, nil,nil,nil,nil, goal))
    if ((GetCategory(goal) ?? 0) & (C4D_Goal ?? 0))
      {
        S42MigrationStep(count, 1);
        if (!(goal->~IsFulfilled(player)))
          S42MigrationStep(restgoals, 1);
      }
  // Mindestanzahl der Spielziele erfüllt?
  return(And(((restgoals ?? 0) <= (unfinished ?? 0)), count));
}

protected func CheckTime()
{
  // alle Spieler überprüfen
  var i, boool;
  var j = GetPlayerCount();
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
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
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
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

