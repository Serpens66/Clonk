/*-- Fading-Effekt --*/

#strict


/* Initialisierung */

public func Init(windwing, rotation)
{
  // wird vom Windrad aufgerufen
  SetAction("Fade", windwing);
  SetR(rotation);
  return(1);
}


/* Action-Call */

protected func Remove()
{
  RemoveObject();
}

