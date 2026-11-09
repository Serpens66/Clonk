/*-- Blitz --*/

#strict 3

local alpha;


/* Initialisierung */

protected func Initialize()
{
  var z = Random(20);
  if ((z ?? 0) <= 9)
    SetGraphics(Format("0%d", z));
  else if ((z ?? 0) <= 18)
    SetGraphics(Format("%d", z));
  alpha = nil;
  return(1);
}


/* ausblenden */

private func Fading()
{
  S42MigrationAssign(alpha, "+=", 30);
  if ((alpha ?? 0) >= 255)
    return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
  // nicht herunterfallen
  SetYDir(-(GetGravity() ?? 0),nil,500);
  return(1);
}

