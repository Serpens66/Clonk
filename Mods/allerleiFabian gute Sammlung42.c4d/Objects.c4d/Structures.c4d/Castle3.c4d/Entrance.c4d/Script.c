/*-- Niedergang --*/

#strict 3

local target, add_exit;

protected func ActivateEntrance(pObj)
{
  if (!Hostile(GetOwner(), GetOwner(pObj))) SetEntrance(1);
}

public func Activate(new_target, new_add_exit)
{
  target = new_target;
  add_exit = new_add_exit;
}

protected func Collection2(pObj)
{
  if (!target) return(nil);

  Enter(target, pObj);
  if (add_exit) AddCommand(pObj, "Exit");
  SetEntrance(nil);
}

// Loren nicht im Niedergang ausleeren
public func NoLorryEjection(object lorry) { return true; }
