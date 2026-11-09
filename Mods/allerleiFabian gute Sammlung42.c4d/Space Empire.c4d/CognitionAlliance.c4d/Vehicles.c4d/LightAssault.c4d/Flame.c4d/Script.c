/*-- Flamme --*/

#strict


local step;

/* Aufruf durch den Jäger */

public func SetOwnerObject()
{
  SetAction("FadeIn",Par(0));
  // ganz ausgeblendet
  SetClrModulation(RGBa(255,255,255,255));
  step = 250;
  // immer hinter dem Jäger
  SetObjectOrder(this(),Par(0));
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
  step -= 25;
  if (step < 0) step = 0;
  if (!step)
    SetAction("Burn");
  SetClrModulation(RGBa(255,255,255,step));
  return(1);
}

private func StepOff()
{
  step += 15;
  if (step >= 255)
    return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,step));
  return(1);
}

/* Timer-Call */

private func Burning()
{
  // Objekte anbrennen
  var obj;
  while (obj = FindObject(0, -27,-8,54,16, OCF_Inflammable(),0,0,NoContainer(),obj))
    if (!Random(6))
      Incinerate(obj);
  // Funken der Flamme
  if (GetAction() eq "SwitchOff")
    return(1);
  return(1);
}

