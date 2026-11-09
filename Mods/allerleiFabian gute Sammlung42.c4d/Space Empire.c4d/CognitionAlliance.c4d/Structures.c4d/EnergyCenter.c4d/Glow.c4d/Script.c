/*-- Batterieglühen --*/

#strict 3


local center;

protected func Initialize()
{
  return(1);
}

public func Init()
{
  center = Par(nil);
  SetAction("Glowing",center);
  return(1);
}

protected func CheckStatus()
{
  // heller werdend
  var phase = GetPhase();
  if ((phase ?? 0) < (GetFillPhase() ?? 0)+8) return(1);
  // wechseln
  SetAction("RevGlowing",center);
  SetPhase(28-(phase ?? 0));
  return(1);
}

protected func CheckRevStatus()
{
  var actual = GetPhase();
  if ((actual ?? 0) < (26-(GetFillPhase() ?? 0))) return(1);
  
  SetAction("Glowing",center);
  SetPhase(26-(actual ?? 0));
  return(1);
}

private func GetFillPhase()
{
  // zwischen 0 und 18
  var fill = center->GetFillStatus();
  var maxf = center->MaxFillEnergy();
  return(((fill ?? 0)*18) / (maxf ?? 0));
}
