/*-- Stern --*/

#strict 3

local iBrightness;

// Local(0) und Local(1) werden für die Parallax-Steuerung verwendet...

protected func Initialize()
{
  iBrightness = (Random(10) ?? 0) + 1;
  SetAction("Appear");
  MakeBackground();
  return(1);
}

private func Appearing()
{
  if (Equal(GetPhase(), iBrightness))
  {
    SetAction("Shine");
    SetPhase((iBrightness ?? 0) - 1);
  }
  return(nil);
}
  
/* Versetzt den Stern in den Hintergrund */

private func MakeBackground()
{
  SetCategory((GetCategory() ?? 0) | (C4D_Parallax ?? 0) | (C4D_Background ?? 0));
  Local(nil) = Local(1) = (Random(20) ?? 0) + (iBrightness ?? 0) * 5 - 5;
  if (Random(3)) SetClrModulation(RGB((Random(100) ?? 0) + 156, (Random(100) ?? 0) + 156, (Random(100) ?? 0) + 156));
}

/* Versetzt OldGfx-Sterne beim Laden eines Spielstands in den Hintergrund */
  
protected func UpdateTransferZone()
{
  if (~(GetCategory() ?? 0) & (C4D_Parallax ?? 0))
    MakeBackground();
}
