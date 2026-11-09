/*-- Schockwelle --*/

#strict

#include SWAV   // Nukleardruckwelle

private func ScreenGamma()
{
  // kurze blaue Bildschirmfärbung
  SetGamma(RGB( 48-GetPhase()*4,  0, 96-GetPhase()*8),
           RGB(152-GetPhase()*2,128,176-GetPhase()*4),
           RGB(255,255,255), 4);
}

