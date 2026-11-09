/*-- Blitz --*/

#strict

local alpha;


/* Initialisierung */

protected func Initialize()
{
  var z = Random(20);
  if (z <= 9)
    SetGraphics(Format("0%d", z));
  else if (z <= 18)
    SetGraphics(Format("%d", z));
  alpha = 0;
  return(1);
}


/* ausblenden */

private func Fading()
{
  alpha += 30;
  if (alpha >= 255)
    return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
  // nicht herunterfallen
  SetYDir(-GetGravity(),0,500);
  return(1);
}

