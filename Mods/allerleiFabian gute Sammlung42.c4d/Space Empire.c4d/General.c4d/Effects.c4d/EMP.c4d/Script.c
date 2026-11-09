/*-- EMP --*/

#strict 3

local timer;


/* globale EMP-Funktion */

global func HasEMP(target)
{
  if (!target) target = this();
  return(FindObject(SEMP,nil,nil,nil,nil,nil,"Active",target));
}


/* Initialisierung */

public func SetTarget(target, par_timer)
{
  // kein Ziel?
  if (!target) return(RemoveObject());
  // schon ein EMP da?
  var obj;
  if (obj = FindObject(GetID(), nil,nil,nil,nil, nil,nil,target))
  {
    obj->Add2Timer(par_timer);
    return(RemoveObject());
  }
  // Reaktion des Ziels überprüfen
  if (!target->~EMP_Reaction(par_timer))
    return(RemoveObject());
  // Ziel auch komplett?
  if ((GetCon(target) ?? 0) < 100)
    return(RemoveObject());
  // Initialisieren
  SetPosition(GetX(target), GetY(target));
  SetVertex(nil,nil, GetVertex(nil,nil,target));
  SetVertex(nil,1, GetVertex(nil,1,target));
  SetAction("Inactive", target);
  Add2Timer(par_timer);
}

public func Add2Timer(value)
{
  // EMP-Stärke erhöhen
  S42MigrationAssign(timer, "+=", (value ?? 0));
  if (And(((timer ?? 0) > 100), (!!SEqual(GetAction(), "Inactive"))))
    SetAction("Active");
  GetActionTarget()->~EMP_Reaction(timer);
}


/* Timer-Call */

protected func Countdown()
{
  // Timer herunterzählen
  S42MigrationStep(timer, -1);
  if ((timer ?? 0) <= 0) RemoveObject();
  if (And(((timer ?? 0) <= 100), (!!SEqual(GetAction(), "Active"))))
    SetAction("Inactive");
}


/* Ziel zerstört */

protected func AttachTargetLost(...) {
  return(RemoveObject());
}

