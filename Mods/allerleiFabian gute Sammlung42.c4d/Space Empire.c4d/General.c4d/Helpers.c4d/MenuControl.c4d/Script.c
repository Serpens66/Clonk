/*-- Menü-Überwachung --*/

#strict 3

local object;      // Objekt, das das Menü enthält
local target;      // Objekt, an das die Aufrufe gehen
local funcname;    // String, der die aufzurufende Funktion angibt
local menuID;      // ID des Menüsymbols
local selection;   // letzte Menü-Auswahl
local infocount;   // Anzahl an gespeicherten Informationen
                   // die Informationen werden in den nummerierten Locals gespeichert


/* Initialisierung */

protected func Initialize()
{
  // irgendwohin verschieben, wo es nicht stört
  SetPosition(nil,nil);
}

public func Init(par_obj, par_target, par_func, removeothers)
{
  object = par_obj;
  target = par_target;
  funcname = par_func;
  menuID = GetMenu(object);
  // Check soll gleich mal einen Änderungsaufruf machen
  selection = -1;
  // andere Menü-Überwachungen für das gleiche Objekt löschen
  if (!removeothers) return(1);
  var obj;
  while (obj = FindObject(SMEC, nil,nil,nil,nil, nil,nil,nil,nil, obj))
    if (Equal(obj->GetTarget(), target))
      RemoveObject(obj);
}

public func AddInfo(info)
{
  Local(infocount) = info;
  S42MigrationStep(infocount, 1);
}

public func GetTarget()
{
  return(target);
}


/* Timer-Call */

protected func Check()
{
  // Menü abgebrochen?
  if (!Equal(GetMenu(object), menuID))
  {
    ObjectCall(target, Format("%s_Closed",funcname));
    return(RemoveObject());
  }
  // Auswahl geändert?
  if (!Equal(GetMenuSelection(object), selection))
  {
    selection = GetMenuSelection(object);
    ObjectCall(target, Format("%s_Changed",funcname), selection, Local(selection));
  }
}

