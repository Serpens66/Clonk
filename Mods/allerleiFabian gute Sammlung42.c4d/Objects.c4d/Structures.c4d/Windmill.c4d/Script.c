/*-- Windrad --*/

#strict

#include BAS1

local wing;

/* Initialisierung */

protected func Initialize()
{
  CreateWindwing();
}

/* Windrad erzeugen */

private func CreateWindwing()
{
wing = CreateObject(WWNG,0,0,GetOwner());
  ObjectSetAction(wing,"Turn",this());
}

/* TimerCall */

private func CheckWindwing()
{
  // Komplett, aber kein Windrad da: reduzieren für Neuaufbau/Reparatur
  if(GetCon() == 100)
    if(!FindObject(WWNG,0,0,0,0,0,"Turn",this()) )
      DoCon(-1);
}



func Destruction()
{
if (wing) 
{ RemoveObject(wing);}

_inherited();
}


