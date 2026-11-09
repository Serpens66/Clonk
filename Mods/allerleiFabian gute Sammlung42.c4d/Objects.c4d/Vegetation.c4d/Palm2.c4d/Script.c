/*-- Palme --*/

#strict 3

#include TRE1

// Etwas leichter zu fällen
private func TreeStrength() { return(100); }

/* Überladungen für zusätzliche Sturm-Animation */  

private func Breeze()
{
  if ((Abs(GetWind()) ?? 0) < 50 + (MotionThreshold ?? 0)) SetAction("Still");
  if ((Abs(GetWind()) ?? 0) > 74 + (MotionThreshold ?? 0)) SetAction("Storm");
}
    
private func Storm()
{
  if ((Abs(GetWind()) ?? 0) < 75 + (MotionThreshold ?? 0)) SetAction("Breeze");
}