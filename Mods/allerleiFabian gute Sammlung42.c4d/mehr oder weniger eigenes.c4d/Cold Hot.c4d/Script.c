/*-- Kältewarnung --*/

#strict 2

protected func Initialize()
{
  SetAlive(0);
  return;
}

//kältefarbe
protected func Warning(target,schaden)
{
if (!(GetAction() == "WarnCold")) SetAction("WarnCold", target);
  // Zielobjekt verschwunden
  if (!GetActionTarget()) return RemoveObject();
  DoEnergy(-schaden,GetActionTarget(),0);
  // Ziel gestorben
  if (!GetAlive(GetActionTarget())) return RemoveObject();
  if(GetActTime()>37) RemoveObject();
  return;
}

// hitzefarbe
protected func WarningX(target,schaden)
{
if (!(GetAction() == "WarnColdX")) SetAction("WarnColdX", target);
  // Zielobjekt verschwunden
  if (!GetActionTarget()) return RemoveObject();
  DoEnergy(-schaden,GetActionTarget(),0);
  // Ziel gestorben
  if (!GetAlive(GetActionTarget())) return RemoveObject();
   if(GetActTime()>37) RemoveObject();
  return;
}

