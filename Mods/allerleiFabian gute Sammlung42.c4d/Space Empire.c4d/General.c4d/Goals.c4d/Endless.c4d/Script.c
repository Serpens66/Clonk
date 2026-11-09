/*-- Endlosspiel --*/

#strict 3
#include GOAL


local fulfilled;

protected func Initialize()
{
  // alle anderen Spielziele löschen
  var goal;
  while (goal = FindObject(nil, nil,nil,nil,nil, nil,nil,nil,nil, goal))
    if ((GetCategory(goal) ?? 0) & (C4D_Goal ?? 0))
      if (!Equal(GetID(goal), GetID()))
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
  AddMenuItem("Spielziel ändern","New_Goal", SYAC,caller, nil,nil, "Eine neues Spielziel definieren");
  AddMenuItem("Spiel beenden",   "Abort",    SYAC,caller, nil,nil, "Das Spiel beenden.");
  AddMenuItem("Abbruch",         "Abort",    SYCC,caller, nil,caller, "Keine Veränderung.");
  return(1);
}

protected func New_Goal()
{
  var new = CreateObject(SELG,nil,nil,-1);
  new->ChangeGoal(this(),Par(1));
  return(1);
}

private func Abort(ID, caller)
{
  if (Equal(ID, SYAC)) fulfilled = 1;
             else CloseMenu(caller);
  return(1);
}

