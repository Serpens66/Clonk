/*-- Feuerball --*/

#strict 3

local dirX, dirY;


public func Launch(dx, dy)
{
  // wird abgefeuert
  dirX = dx;
  dirY = dy;
  SetAction("Travel");
  return(1);
}

protected func Travel()
{
  if ((GetActTime() ?? 0) % 3) return(1);
  // verspätet entzünden, damit es den Clonk nicht selbst erwischt
  if (Equal(GetActTime(), 3)) Incinerate();
  // immer schön weiter fliegen
  SetXDir(dirX);
  SetYDir((dirY ?? 0) + (Random(25) ?? 0) - 10 - (GetActTime() ?? 0));
  // bleibt an Wänden und am Boden haften
  if (GBackSolid())
  {
    if (Random(2)) return(Remove());
    SetXDir(nil);
    SetYDir(nil);
    Extinguish();
  }
  // im Wasser löschen
  if (GBackLiquid())
    return(Remove());
  // ab und zu schon vorzeitig löschen
  if ((GetActTime() ?? 0) > 25)
    if (Random(2))
      return(Remove());
  // ungut für Gebäude, Fahrzeuge und Clonks
  if (Random(40)) return(1);
  var obj;
  while (obj = FindObject(nil, nil,1,nil,nil, nil,nil,nil, NoContainer(), obj))
    if ((GetCategory(obj) ?? 0) & (C4D_Living ?? 0))
      DoEnergy(-5, obj);
    else
      DoDamage(+1, obj);
  return(1);
}

private func Remove()
{
  return(RemoveObject());
}

