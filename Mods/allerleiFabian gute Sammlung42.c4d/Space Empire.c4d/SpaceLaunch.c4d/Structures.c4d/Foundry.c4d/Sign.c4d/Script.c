/*-- Anzeigen-Objekt --*/

#strict 3

local furnace, show;


/* Initialisierung */

public func Init(building, par_furnace)
{
  // wird vom Hochofen aufgerufen
  SetAction("Nothing", building);
  furnace = par_furnace;
  show = 1;
  return(Check());
}

/* Action-Call */

protected func Check()
{
  var oldshow = show;
  show = furnace->GetSignInfo();
  if (!oldshow)
  {
    if (Equal(show, 1))  return(SetAction("Nothing"));
    if (Equal(show, 2))  return(SetAction("ShowB4M"));
    if (Equal(show, 3))  return(SetAction("ShowE4M"));
    if (Equal(show, 4))  return(SetAction("ShowB4E"));
    return(1);
  }
  if (Equal(show, oldshow))
    return(1);
  SetAction("Flicker");
  show = nil;
  return(1);
}

