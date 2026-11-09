/*-- Glüh-Effekt --*/

#strict 3

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
  if ((phase ?? 0) < (GetFillPhase() ?? 0))
    return(1);
  // wechseln
  SetAction("RevGlowing");
  SetPhase(20-(phase ?? 0));
}

private func CheckRev()
{
  // dunkler werdend
  var phase = GetPhase();
  if ((phase ?? 0) < (Min(25-(GetFillPhase() ?? 0), 20) ?? 0))
    return(1);
  // wechseln
  SetAction("Glowing");
  SetPhase(20-(phase ?? 0));
}

private func GetFillPhase()
{
  // zwischen 0 und 20
  var fill = battery->GetFill();
  var maxf = battery->MaxFillEnergy();
  return((fill ?? 0) * 20 / (maxf ?? 0));
}

