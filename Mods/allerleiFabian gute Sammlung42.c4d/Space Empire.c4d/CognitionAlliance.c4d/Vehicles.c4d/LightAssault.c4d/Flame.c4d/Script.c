/*-- Flamme --*/

#strict 3


local step;

/* Aufruf durch den Jäger */

public func SetOwnerObject()
{
  SetAction("FadeIn",Par(nil));
  // ganz ausgeblendet
  SetClrModulation(RGBa(255,255,255,255));
  step = 250;
  // immer hinter dem Jäger
  SetObjectOrder(this(),Par(nil));
  return(1);
}

public func SwitchOff()
{
  SetAction("SwitchOff");
  return(1);
}


/* Action-Calls */

private func StepIn()
{
  S42MigrationAssign(step, "-=", 25);
  if ((step ?? 0) < 0) step = nil;
  if (!step)
    SetAction("Burn");
  SetClrModulation(RGBa(255,255,255,step));
  return(1);
}

private func StepOff()
{
  S42MigrationAssign(step, "+=", 15);
  if ((step ?? 0) >= 255)
    return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,step));
  return(1);
}

/* Timer-Call */

private func Burning()
{
  // Objekte anbrennen
  var obj;
  while (obj = FindObject(nil, -27,-8,54,16, OCF_Inflammable,nil,nil,NoContainer(),obj))
    if (!Random(6))
      Incinerate(obj);
  // Funken der Flamme
  if (!!SEqual(GetAction(), "SwitchOff"))
    return(1);
  return(1);
}

