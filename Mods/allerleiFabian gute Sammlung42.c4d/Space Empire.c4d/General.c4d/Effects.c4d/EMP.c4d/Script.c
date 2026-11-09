/*-- EMP --*/

#strict

local timer;


/* globale EMP-Funktion */

global func HasEMP(target)
{
  if (!target) target = this();
  return(FindObject(SEMP,0,0,0,0,0,"Active",target));
}


/* Initialisierung */

public func SetTarget(target, par_timer)
{
  // kein Ziel?
  if (!target) return(RemoveObject());
  // schon ein EMP da?
  var obj;
  if (obj = FindObject(GetID(), 0,0,0,0, 0,0,target))
  {
    obj->Add2Timer(par_timer);
    return(RemoveObject());
  }
  // Reaktion des Ziels überprüfen
  if (!target->~EMP_Reaction(par_timer))
    return(RemoveObject());
  // Ziel auch komplett?
  if (GetCon(target) < 100)
    return(RemoveObject());
  // Initialisieren
  SetPosition(GetX(target), GetY(target));
  SetVertex(0,0, GetVertex(0,0,target));
  SetVertex(0,1, GetVertex(0,1,target));
  SetAction("Inactive", target);
  Add2Timer(par_timer);
}

public func Add2Timer(value)
{
  // EMP-Stärke erhöhen
  timer += value;
  if ((timer > 100) && (GetAction() eq "Inactive"))
    SetAction("Active");
  GetActionTarget()->~EMP_Reaction(timer);
}


/* Timer-Call */

protected func Countdown()
{
  // Timer herunterzählen
  --timer;
  if (timer <= 0) RemoveObject();
  if ((timer <= 100) && (GetAction() eq "Active"))
    SetAction("Inactive");
}


/* Ziel zerstört */

protected AttachTargetLost:
  return(RemoveObject());

