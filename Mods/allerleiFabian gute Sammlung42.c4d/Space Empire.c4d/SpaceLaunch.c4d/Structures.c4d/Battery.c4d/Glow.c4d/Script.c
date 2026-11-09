/*-- Glüh-Effekt --*/

#strict

local battery;


/* Initialisierung */

public func Init(par_battery)
{
  // wird von der Batterie aufgerufen
  battery = par_battery;
  SetAction("Glowing", battery);
}


/* Action-Call */

private func CheckGlow()
{
  // heller werdend
  var phase = GetPhase();
  if (phase < GetFillPhase())
    return(1);
  // wechseln
  SetAction("RevGlowing");
  SetPhase(20-phase);
}

private func CheckRev()
{
  // dunkler werdend
  var phase = GetPhase();
  if (phase < Min(25-GetFillPhase(), 20))
    return(1);
  // wechseln
  SetAction("Glowing");
  SetPhase(20-phase);
}

private func GetFillPhase()
{
  // zwischen 0 und 20
  var fill = battery->GetFill();
  var maxf = battery->MaxFillEnergy();
  return(fill * 20 / maxf);
}

