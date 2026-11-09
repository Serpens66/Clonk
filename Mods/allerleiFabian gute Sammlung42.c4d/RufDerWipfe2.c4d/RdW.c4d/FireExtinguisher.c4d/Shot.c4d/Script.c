/*-- Löschschuss --*/

#strict 3


/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  return(1);
}

/* Brand löschen */

public func ExtinguishFire()
{
  var obj = FindObject2(Find_AtPoint(),Find_OCF(OCF_OnFire));
  if (!obj) return(1);
  if(Random((GetDefHeight(GetID(obj)) ?? 0)*(GetCon(obj) ?? 0)/100/2)) return nil;
  Extinguish(obj);
  Sound("Pshshsh");
  return(Remove());
}

/* Treffer */

protected func Hit()
{
  var ph = BoundBy(GetPhase(),nil,6);
  SetAction("Attach");
  SetPhase(ph);
  return(1);
}

/* Weg damit */

public func Remove()
{
  return(RemoveObject());
}

