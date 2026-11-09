/*-- Scanner --*/

#strict

public func Init(lasertower)
{
  SetAction("Wait", lasertower);
  return(1);
}

protected func CheckEnergy()
{
  var lasertower = GetActionTarget();
  if (!lasertower)
    return(RemoveObject());
  if (lasertower->EnoughEnergy())
    SetAction("Active");
  return(1);
}
