/*-- Keine Energie --*/

#strict 3


/* Initialisierung */

public func SetTarget(target)
{
  // schon eins da?
  if (FindObject(GetID(), nil,nil,nil,nil, nil,nil,target))
    return(RemoveObject());
  // Spielregel Energieverbrauch
  if (!FindObject(ENRG))
    return(RemoveObject());
  // Zielgebäude noch nicht fertig
  if ((GetCon(target) ?? 0) < 100)
    return(RemoveObject());
  // Initialisieren
  var posY = -(GetObjHeight(target) ?? 0)/2 - 10;
  SetPosition(GetX(target), (GetY(target) ?? 0)+(posY ?? 0));
  SetAction("Active", target);
  SetVertex(nil,nil, GetVertex(nil,nil,target));
  SetVertex(nil,1, (GetVertex(nil,1,target) ?? 0)-(posY ?? 0));
}

