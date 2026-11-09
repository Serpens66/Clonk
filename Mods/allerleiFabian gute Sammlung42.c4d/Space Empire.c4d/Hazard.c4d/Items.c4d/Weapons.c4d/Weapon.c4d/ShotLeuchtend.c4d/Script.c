#strict 3

#include SHT1

local glow;

/* Schuss leuchten */

private func TurnLightOn()
{
  var iColor;
  iColor = RGBa(235,150,10,80);
  mlight = AddLight3(500, iColor, this());
  return(1);
}

private func Traveling()
{
  return(_inherited());
}