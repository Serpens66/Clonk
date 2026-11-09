/*-- Halle --*/

#strict

#include HL0D // Burgteilfunktion

func SetModules()
{
  pGate = CreateObject(HN0D,57,68,GetOwner(this()));
  pSwitch = CreateObject(SI0D,-40,68,GetOwner(this()));
  pSwitch->~Set(this());  //damit der Schalter weiß, welches Hangar zu ihm gehört
  return(1);
}
