/*-- Glüheffekt --*/

#strict

local mode, step, alpha;   // Werte für mode:  0 = links,  1 = rechts,  2 = beide (gebündelt)


/* Object-Call */

public func Init(lasertower, par_mode)
{
  mode = par_mode;
  step = 0;
  if (mode == 0)  SetAction("GlowLeft", lasertower);
  if (mode == 1)  SetAction("GlowRight",lasertower);
  if (mode == 2)  SetAction("GlowBoth", lasertower);
  // anfangs nicht zu sehen
  alpha = 255;
  SetClrModulation(RGBa(255,255,255,255));
  return(1);
}


/* Timer-Call */

private func Check()
{
  ++step;
  if (mode < 2)
    if (step <= 5)  alpha -= 51;
              else  alpha += 100;
  else
    if (step <= 10) alpha -= 26;
               else alpha += 51;
  // im Wertebereich bleiben
  if (alpha < 0)   alpha = 0;
  if (alpha > 255) return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
  return(1);
}

