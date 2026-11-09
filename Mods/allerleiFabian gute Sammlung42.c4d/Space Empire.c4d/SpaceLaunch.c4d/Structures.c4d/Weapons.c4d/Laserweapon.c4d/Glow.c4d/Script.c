/*-- Glüheffekt --*/

#strict

local step, alpha;


/* Object-Call */

public func Init(building)
{
  step = 0;
  SetAction("Glow", building);
  // anfangs nicht zu sehen
  alpha = 255;
  SetClrModulation(RGBa(255,255,255,255));
}


/* Timer-Call */

private func Check()
{
  ++step;
  if (step <= 5)  alpha -= 51;
            else  alpha += 100;
  // im Wertebereich bleiben
  if (alpha < 0)   alpha = 0;
  if (alpha > 255) return(RemoveObject());
  SetClrModulation(RGBa(255,255,255,alpha));
}

