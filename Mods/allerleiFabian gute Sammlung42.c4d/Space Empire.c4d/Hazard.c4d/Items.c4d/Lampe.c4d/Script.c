/*-- Neonlamp --*/

#strict

local mlight;

func Initialize() 
{
  var iF = 170,
      iS = 240,
      iH = 240;
  SetColorDw(HSL(iF, iS, iH));
}

public func Entrance()
{
  TurnLightOff();
}

public func Departure()
{
  if (!mlight && LightNeeded)  TurnLightOn();
}

private func CheckNight()
{
  if (!mlight && LightNeeded)  TurnLightOn();
  if (mlight && !LightNeeded)  TurnLightOff();
}

private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(240,240,255,45);
  mlight = AddLight(1000, iColor, this());
  return(1);
}

private func TurnLightOff()
{
  if (mlight) RemoveObject(mlight);
  return(1);
}

func IsEquipment() { return(1); }
public func IsRefineryProduct() { return(1); }

public func Completion()
{
  SetPlrViewRange(200); 
}