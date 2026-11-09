/*-- Glüheffekt --*/

#strict 3

local mode, step, alpha;   // Werte für mode:  0 = links,  1 = rechts,  2 = beide (gebündelt)


/* Object-Call */

public func Init(lasertower, par_mode)
{
  mode = par_mode;
  step = nil;
  if (Equal(mode, nil))  SetAction("GlowLeft", lasertower);
  if (Equal(mode, 1))  SetAction("GlowRight",lasertower);
  if (Equal(mode, 2))  SetAction("GlowBoth", lasertower);
  // anfangs nicht zu sehen
  alpha = 255;
  SetClrModulation(RGBa(255,255,255,255));
  return(1);
}


/* Timer-Call */

private func Check()
{
  S42MigrationStep(step, 1);
  if ((mode ?? 0) < 2)
    if ((step ?? 0) <= 5)  S42MigrationAssign(alpha, "-=", 51);
              else  S42MigrationAssign(alpha, "+=", 100);
  else
    if ((step ?? 0) <= 10) S42MigrationAssign(alpha, "-=", 26);
               else S42MigrationAssign(alpha, "+=", 51);
  // im Wertebereich bleiben
  if ((alpha ?? 0) < 0)   alpha = nil;
  if ((alpha ?? 0) > 255) return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
  return(1);
}

