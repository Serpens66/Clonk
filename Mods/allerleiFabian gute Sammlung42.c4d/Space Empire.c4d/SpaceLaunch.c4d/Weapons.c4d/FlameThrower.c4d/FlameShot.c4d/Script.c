/*-- Feuerball --*/

#strict

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
  if (GetActTime() % 3) return(1);
  // verspätet entzünden, damit es den Clonk nicht selbst erwischt
  if (GetActTime() == 3) Incinerate();
  // immer schön weiter fliegen
  SetXDir(dirX);
  SetYDir(dirY + Random(25) - 10 - GetActTime());
  // bleibt an Wänden und am Boden haften
  if (GBackSolid())
  {
    if (Random(2)) return(Remove());
    SetXDir(0);
    SetYDir(0);
    Extinguish();
  }
  // im Wasser löschen
  if (GBackLiquid())
    return(Remove());
  // ab und zu schon vorzeitig löschen
  if (GetActTime() > 25)
    if (Random(2))
      return(Remove());
  // ungut für Gebäude, Fahrzeuge und Clonks
  if (Random(40)) return(1);
  var obj;
  while (obj = FindObject(0, 0,1,0,0, 0,0,0, NoContainer(), obj))
    if (GetCategory(obj) & C4D_Living())
      DoEnergy(-5, obj);
    else
      DoDamage(+1, obj);
  return(1);
}

private func Remove()
{
  return(RemoveObject());
}

