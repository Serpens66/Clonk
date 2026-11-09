/*-- Kältewarnung --*/

#strict 3

protected func Initialize()
{
  SetAlive(nil);
  return;
}

protected func Warning()
{
  // Zielobjekt nimmt Schaden vor Kälte
  DoEnergy(-2,GetActionTarget());
  // Zielobjekt verschwunden
  if (!GetActionTarget()) return RemoveObject();
  // Ziel gestorben
  if (!GetAlive(GetActionTarget())) return RemoveObject();
  // Ziel nimmt keinen Schaden wenn in freundlicher Basis
  if (InBase(GetActionTarget()))
    SetAction("WarnColdX",GetActionTarget());
  return;
}

protected func WarningX()
{
  if (InBase(GetActionTarget()))
  {
    if((GetActTime() ?? 0)>300) RemoveObject();
  } else {
    SetAction("WarnCold",GetActionTarget());
  }
  return;
}

private func InBase(object obj)
{
  return obj &&
    obj->Contained() &&
    !S42MigrationEqual(obj->Contained()->GetBase(), NO_OWNER) &&
    !Hostile(obj->Contained()->GetBase(), obj->GetOwner());
}
