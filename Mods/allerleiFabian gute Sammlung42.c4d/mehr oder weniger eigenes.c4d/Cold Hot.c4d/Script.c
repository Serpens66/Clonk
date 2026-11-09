/*-- Kältewarnung --*/

#strict 3

protected func Initialize()
{
  SetAlive(nil);
  return;
}

//kältefarbe
protected func Warning(target,schaden)
{
if (!(S42MigrationEqual(GetAction(), "WarnCold"))) SetAction("WarnCold", target);
  // Zielobjekt verschwunden
  if (!GetActionTarget()) return RemoveObject();
  DoEnergy(-(schaden ?? 0),GetActionTarget(),nil);
  // Ziel gestorben
  if (!GetAlive(GetActionTarget())) return RemoveObject();
  if((GetActTime() ?? 0)>37) RemoveObject();
  return;
}

// hitzefarbe
protected func WarningX(target,schaden)
{
if (!(S42MigrationEqual(GetAction(), "WarnColdX"))) SetAction("WarnColdX", target);
  // Zielobjekt verschwunden
  if (!GetActionTarget()) return RemoveObject();
  DoEnergy(-(schaden ?? 0),GetActionTarget(),nil);
  // Ziel gestorben
  if (!GetAlive(GetActionTarget())) return RemoveObject();
   if((GetActTime() ?? 0)>37) RemoveObject();
  return;
}

