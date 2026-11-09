/*-- Schockwelle --*/

#strict 3

#include SWAV   // Nukleardruckwelle

private func ScreenGamma()
{
  // kurze blaue Bildschirmfärbung
  SetGamma(RGB( 48-(GetPhase() ?? 0)*4,  nil, 96-(GetPhase() ?? 0)*8),
           RGB(152-(GetPhase() ?? 0)*2,128,176-(GetPhase() ?? 0)*4),
           RGB(255,255,255), 4);
}

