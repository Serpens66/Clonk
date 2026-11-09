/*-- Fragment --*/

#strict 3

local timer;

private func Check()
{
  S42MigrationStep(timer, 1);
  if ((timer ?? 0) > 100)
    return(RemoveObject());
  if ((timer ?? 0) > 10)
    CastParticles("TSpark", (130-(timer ?? 0))/40, 10, nil,nil, 20,20, RGB(192,192,160),RGB(255,255,255));
  return(1);
}

