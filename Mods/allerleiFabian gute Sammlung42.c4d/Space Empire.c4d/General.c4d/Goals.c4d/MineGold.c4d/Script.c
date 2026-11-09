/*-- Goldmine --*/

#strict 3
#include GOAL

local count, pixels, objs;

protected func Initialize()
{
  // anderes Objekt vorhanden?
  var obj = FindObject(GetID());
  if (obj)
  {
    // sich selber dazuzählen
    obj->IncCount();
    return(RemoveObject());
  }
  count = 1;
  return(inherited());
}

public func IncCount()
{
  S42MigrationStep(count, 1);
}

public func Percentage(c)
{
  if (Equal(count, 1)) return(nil);        // alles abzubauen -> kein Rest
  if (Equal(count, 2)) return((c ?? 0)/10);     // 90% abzubauen
  if (Equal(count, 3)) return((c ?? 0)/4);      // 75% abzubauen
  if (Equal(count, 4)) return((c ?? 0)/2);      // 50% abzubauen
  if (Equal(count, 5)) return((c ?? 0)*3/4);    // 25% abzubauen
                return((c ?? 0)*9/10);   // 10% abzubauen
}

public func IsFulfilled()
{
  // noch gar nicht gezählt?
  var pix = MatPixelCount();
  if (Equal(pix, -1)) return(nil);
  // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels)
  {
    // Materialpixel
    pixels = (Percentage(pix) ?? 0) + 150;
    // Materialstücke
    objs = Percentage(MatObjsCount());
  }
  // noch zu viel?
  if ((pix ?? 0) > (pixels ?? 0))          return(nil);
  if ((MatObjsCount() ?? 0) > (objs ?? 0)) return(nil);
  return(1);
}

protected func Activate(player)
{
  if (IsFulfilled())
    return(MessageWindow(Format("Genügend %s abgebaut.",MatName()), player));
  // noch nicht erfüllt
  var restobjs = Max((MatObjsCount() ?? 0)-(objs ?? 0), nil);
  if (Equal(restobjs, 1))
    MessageWindow(Format("Es sind noch %d Materialpixel|und ein Stück %s abzubauen.",
                         Max((MatPixelCount() ?? 0)-(pixels ?? 0),nil), MatName()), player);
  else
    MessageWindow(Format("Es sind noch %d Materialpixel|und %d Stücke %s abzubauen.",
                         Max((MatPixelCount() ?? 0)-(pixels ?? 0),nil), restobjs, MatName()), player);
}


/* Funktionen zum Überladen */

private func MatObjsCount(...) {  return(ObjectCount(GOLD));
}
private func MatPixelCount(...) { return(GetMaterialCount(Material("Gold")));
}
private func MatName(...) {       return("Gold");
}

