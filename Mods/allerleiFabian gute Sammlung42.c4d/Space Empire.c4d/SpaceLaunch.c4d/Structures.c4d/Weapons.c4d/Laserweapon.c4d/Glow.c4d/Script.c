/*-- Glüheffekt --*/

#strict 3

local step, alpha;


/* Object-Call */

public func Init(building)
{
  step = nil;
  SetAction("Glow", building);
  // anfangs nicht zu sehen
  alpha = 255;
  SetClrModulation(RGBa(255,255,255,255));
}


/* Timer-Call */

private func Check()
{
  S42MigrationStep(step, 1);
  if ((step ?? 0) <= 5)  S42MigrationAssign(alpha, "-=", 51);
            else  S42MigrationAssign(alpha, "+=", 100);
  // im Wertebereich bleiben
  if ((alpha ?? 0) < 0)   alpha = nil;
  if ((alpha ?? 0) > 255) return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
}

