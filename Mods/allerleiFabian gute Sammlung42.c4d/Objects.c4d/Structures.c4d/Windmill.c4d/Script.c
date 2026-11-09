/*-- Windrad --*/

#strict 3

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
wing = CreateObject(WWNG,nil,nil,GetOwner());
  ObjectSetAction(wing,"Turn",this());
}

/* TimerCall */

private func CheckWindwing()
{
  // Komplett, aber kein Windrad da: reduzieren für Neuaufbau/Reparatur
  if(Equal(GetCon(), 100))
    if(!FindObject(WWNG,nil,nil,nil,nil,nil,"Turn",this()) )
      DoCon(-1);
}



func Destruction()
{
if (wing) 
{ RemoveObject(wing);}

_inherited();
}


