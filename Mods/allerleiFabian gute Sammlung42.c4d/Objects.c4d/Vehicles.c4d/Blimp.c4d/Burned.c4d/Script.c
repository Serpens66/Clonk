/* Verbranntes Luftschiff */

#strict
local mlight;

/* Abbrennen: Eventuell verzögerte ClearDir-Aufrufe verwerfen */

public func ClearDir() {}




public func Incineration() {
  //  brennen leuchtet
TurnLightOn();
return _inherited();
}




private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,50);
  mlight = AddLight(1000, iColor, this());
  return(1);
}

private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}

