/*-- Löschschuss --*/

#strict


/* Initialisierung */

protected func Initialize()
{
  SetAction("Fly");
  return(1);
}

/* Brand löschen */

public func ExtinguishFire()
{
  var obj = FindObject2(Find_AtPoint(),Find_OCF(OCF_OnFire()));
  if (!obj) return(1);
  if(Random(GetDefHeight(GetID(obj))*GetCon(obj)/100/2)) return 0;
  Extinguish(obj);
  Sound("Pshshsh");
  return(Remove());
}

/* Treffer */

protected func Hit()
{
  var ph = BoundBy(GetPhase(),0,6);
  SetAction("Attach");
  SetPhase(ph);
  return(1);
}

/* Weg damit */

public func Remove()
{
  return(RemoveObject());
}

