/*-- Endlosspiel --*/

#strict
#include GOAL


local fulfilled;

protected func Initialize()
{
  // alle anderen Spielziele löschen
  var goal;
  while (goal = FindObject(0, 0,0,0,0, 0,0,0,0, goal))
    if (GetCategory(goal) & C4D_Goal())
      if (GetID(goal) != GetID())
        RemoveObject(goal);
  return(1);
}

public func IsFulfilled()
{
  return(fulfilled);
}

protected func Activate(player)
{
  // anderes Spielziel, Spiel beenden, Abbruch
  var caller = GetCursor(player);
  CreateMenu(GetID(),caller);
  AddMenuItem("Spielziel ändern","New_Goal", SYAC,caller, 0,0, "Eine neues Spielziel definieren");
  AddMenuItem("Spiel beenden",   "Abort",    SYAC,caller, 0,0, "Das Spiel beenden.");
  AddMenuItem("Abbruch",         "Abort",    SYCC,caller, 0,caller, "Keine Veränderung.");
  return(1);
}

protected func New_Goal()
{
  var new = CreateObject(SELG,0,0,-1);
  new->ChangeGoal(this(),Par(1));
  return(1);
}

private func Abort(ID, caller)
{
  if (ID == SYAC) fulfilled = 1;
             else CloseMenu(caller);
  return(1);
}

