/*-- Windkraftanlage --*/

#strict
#include BOB2
#include XMTR

local basement, manager;
local windwing;


/* Initialisierung */

protected Initialize:
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(1,1);
  windwing = CreateObject(SWEW,0,0,GetOwner());
  windwing->Init(this());
  SetAction("Stand");
  return(1);

protected Construction:
  basement = CreateObject(BS05,0,+8,GetOwner());
  return(1);
  
protected Destruction:
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);

public PreDestruction:
  if (manager)  RemoveObject(manager);
  if (windwing) RemoveObject(windwing);
  return(inherited());


/* Steuerung */

protected func ControlUp(clonk)
{
  [Statusanzeige]
  if (!inherited(clonk)) return(1);   // wenn verfeindeter Spieler
  var wind = Abs(GetWind(GetX(windwing)-GetX(), GetY(windwing)-GetY()));
  ShowInfoBar(IBRE, this(), wind,100, 5,1,1);
  return(1);
}


/* TimerCall */

private CheckWindwing:
  // Komplett, aber kein Windrad da: reduzieren für Neuaufbau/Reparatur
  if (GetCon()==100 && !windwing) DoCon(-1);
  return(1);


/* Anzeigentext */

private func InfoBarText(barID, fill, maxf)
{
  if (barID == IBRE)
    return(Format("@Wind:  %d", fill));
  return(0);
}


/* Eigenschaften */

public InfoBarPos:  return(-10);
public InfoBarSize: return(1);
public GetMaxDamage: return(80);

