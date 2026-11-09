/*-- Keine Energie --*/

#strict


/* Initialisierung */

public func SetTarget(target)
{
  // schon eins da?
  if (FindObject(GetID(), 0,0,0,0, 0,0,target))
    return(RemoveObject());
  // Spielregel Energieverbrauch
  if (!FindObject(ENRG))
    return(RemoveObject());
  // Zielgebäude noch nicht fertig
  if (GetCon(target) < 100)
    return(RemoveObject());
  // Initialisieren
  var posY = -GetObjHeight(target)/2 - 10;
  SetPosition(GetX(target), GetY(target)+posY);
  SetAction("Active", target);
  SetVertex(0,0, GetVertex(0,0,target));
  SetVertex(0,1, GetVertex(0,1,target)-posY);
}

