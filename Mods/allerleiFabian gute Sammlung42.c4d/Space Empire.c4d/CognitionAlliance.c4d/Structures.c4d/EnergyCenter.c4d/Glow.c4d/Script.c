/*-- Batterieglühen --*/

#strict


local center;

protected func Initialize()
{
  return(1);
}

public func Init()
{
  center = Par(0);  
  SetAction("Glowing",center);
  return(1);
}

protected func CheckStatus()
{
  // heller werdend
  var phase = GetPhase();
  if (phase < GetFillPhase()+8) return(1);
  // wechseln
  SetAction("RevGlowing",center);
  SetPhase(28-phase);
  return(1);
}

protected func CheckRevStatus()
{
  var actual = GetPhase();
  if (actual < (26-GetFillPhase())) return(1);
  
  SetAction("Glowing",center);
  SetPhase(26-actual);
  return(1);
}

private func GetFillPhase()
{
  // zwischen 0 und 18
  var fill = center->GetFillStatus();
  var maxf = center->MaxFillEnergy();
  return((fill*18) / maxf);
}
