/*-- Akku --*/

#strict 3

private func CheckEnergy()
{
  // EMP-Einwirkung?
  var battery = GetActionTarget();
  if (HasEMP(battery))
    return(1);
  var lowenergy = (!!SEqual(GetAction(), "LowNRG"));
  var batteryfill = battery->GetFill();
  // auf hohes Energieniveau bringen...
  if (And(lowenergy, ((batteryfill ?? 0) >= 2800)))
  {
    SetAction("HighNRG");
    battery->DoFill(-800);
    return(1);
  }
  // ...oder auf niedriges Niveau
  if (And(!lowenergy, ((batteryfill ?? 0) <= 1600)))
  {
    SetAction("LowNRG");
    battery->DoFill(+800);
  }
  return(1);
}

public func Remove(clonk)
{
  // Akku abnehmen
  if (!!SEqual(GetAction(), "HighNRG")) ChangeDef(SACU);   // voll
                           else ChangeDef(SACE);   // leer
  // dem Clonk geben
  if (clonk) Enter(clonk);
}

protected func AttachTargetLost()
{
  Remove();
}

