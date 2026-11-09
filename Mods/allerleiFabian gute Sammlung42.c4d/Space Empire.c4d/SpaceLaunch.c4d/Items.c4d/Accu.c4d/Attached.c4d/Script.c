/*-- Akku --*/

#strict

private func CheckEnergy()
{
  // EMP-Einwirkung?
  var battery = GetActionTarget();
  if (HasEMP(battery))
    return(1);
  var lowenergy = (GetAction() eq "LowNRG");
  var batteryfill = battery->GetFill();
  // auf hohes Energieniveau bringen...
  if (lowenergy && (batteryfill >= 2800))
  {
    SetAction("HighNRG");
    battery->DoFill(-800);
    return(1);
  }
  // ...oder auf niedriges Niveau
  if (!lowenergy && (batteryfill <= 1600))
  {
    SetAction("LowNRG");
    battery->DoFill(+800);
  }
  return(1);
}

public func Remove(clonk)
{
  // Akku abnehmen
  if (GetAction() eq "HighNRG") ChangeDef(SACU);   // voll
                           else ChangeDef(SACE);   // leer
  // dem Clonk geben
  if (clonk) Enter(clonk);
}

protected func AttachTargetLost()
{
  Remove();
}

