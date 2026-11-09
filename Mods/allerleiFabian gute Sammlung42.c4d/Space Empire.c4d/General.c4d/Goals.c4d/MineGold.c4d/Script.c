/*-- Goldmine --*/

#strict
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
  ++count;
}

public func Percentage(c)
{
  if (count==1) return(0);        // alles abzubauen -> kein Rest
  if (count==2) return(c/10);     // 90% abzubauen
  if (count==3) return(c/4);      // 75% abzubauen
  if (count==4) return(c/2);      // 50% abzubauen
  if (count==5) return(c*3/4);    // 25% abzubauen
                return(c*9/10);   // 10% abzubauen
}

public func IsFulfilled()
{
  // noch gar nicht gezählt?
  var pix = MatPixelCount();
  if (pix == -1) return(0);
  // speichern, wieviel jeweils übrig bleiben darf
  if (!pixels)
  {
    // Materialpixel
    pixels = Percentage(pix) + 150;
    // Materialstücke
    objs = Percentage(MatObjsCount());
  }
  // noch zu viel?
  if (pix > pixels)          return(0);
  if (MatObjsCount() > objs) return(0);
  return(1);
}

protected func Activate(player)
{
  if (IsFulfilled())
    return(MessageWindow(Format("Genügend %s abgebaut.",MatName()), player));
  // noch nicht erfüllt
  var restobjs = Max(MatObjsCount()-objs, 0);
  if (restobjs == 1)
    MessageWindow(Format("Es sind noch %d Materialpixel|und ein Stück %s abzubauen.",
                         Max(MatPixelCount()-pixels,0), MatName()), player);
  else
    MessageWindow(Format("Es sind noch %d Materialpixel|und %d Stücke %s abzubauen.",
                         Max(MatPixelCount()-pixels,0), restobjs, MatName()), player);
}


/* Funktionen zum Überladen */

private MatObjsCount:  return(ObjectCount(GOLD));
private MatPixelCount: return(GetMaterialCount(Material("Gold")));
private MatName:       return("Gold");

