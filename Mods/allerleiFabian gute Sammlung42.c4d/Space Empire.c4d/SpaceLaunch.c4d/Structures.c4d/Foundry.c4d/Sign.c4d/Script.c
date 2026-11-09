/*-- Anzeigen-Objekt --*/

#strict

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
    if (show==1)  return(SetAction("Nothing"));
    if (show==2)  return(SetAction("ShowB4M"));
    if (show==3)  return(SetAction("ShowE4M"));
    if (show==4)  return(SetAction("ShowB4E"));
    return(1);
  }
  if (show == oldshow)
    return(1);
  SetAction("Flicker");
  show = 0;
  return(1);
}

