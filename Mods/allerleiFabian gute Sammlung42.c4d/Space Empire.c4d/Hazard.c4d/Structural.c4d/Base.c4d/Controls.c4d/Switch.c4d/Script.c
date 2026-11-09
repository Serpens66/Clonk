/*-- Schalter --*/

#strict 3

// Zielobjekte
local target;
local locked;

public func Lock(bool unlock)
{
  // Negativlogik. :D
  unlock ??= false;
  locked = !unlock;
}

public func IsLocked() { return(locked); }

public func Set(object t) { target = t; }

/* Steuerung */

protected func Initialize()
{
	SetAction("Switch");
}

protected func ControlThrow(object byObj)
{
  if(IsLocked())
    return(Sound("CommandFailure1",nil,nil,80,GetController(byObj)));
    
	SetPhase(((GetPhase() ?? 0)+1)%2);
  if(target) target->~Access();
  
  Sound("Lever1",nil,nil,80);
  
  return(1);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[(GetLength(extra) ?? 0)] = ["Set(Object(%d))", target];
	if (locked)
		extra[(GetLength(extra) ?? 0)] = "Lock()";
}


// Mehrere Ziele...
//
// Ziel hinzufügen: pTarget = Zielobjekt, CallFunc = Funktion, die aufgerufen werden soll (0 für "Switch")
//public func AddTarget(object pTarget, string CallFunc)
//{
//  targets[GetLength(targets)] = CreateArray();
//  targets[GetLength(targets)-1][0] = pTarget;
//  targets[GetLength(targets)-1][1] = CallFunc;
//  return(1);
//}
//
// Ziel entfernen
//public func RemoveTarget(object pTarget)
//{
//  var i=-1;
//  for(var step in targets)
//  {
//    i++;
//    if(!step) continue;
//    if(step[0] == pTarget)
//    {
//      targets[i] = 0;
//      return(1);
//    }
//  }
//}
