/*-- Windkraftanlage --*/

#strict 3
#include BOB2
#include XMTR

local basement, manager;
local windwing;


/* Initialisierung */

protected func Initialize(...) {
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(1,1);
  windwing = CreateObject(SWEW,nil,nil,GetOwner());
  windwing->Init(this());
  SetAction("Stand");
  return(1);
}

protected func Construction(...) {
  basement = CreateObject(BS05,nil,+8,GetOwner());
  return(1);
}
  
protected func Destruction(...) {
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction(...) {
  if (manager)  RemoveObject(manager);
  if (windwing) RemoveObject(windwing);
  return(inherited());
}


/* Steuerung */

protected func ControlUp(clonk)
{
  [Statusanzeige]
  if (!inherited(clonk)) return(1);   // wenn verfeindeter Spieler
  var wind = Abs(GetWind((GetX(windwing) ?? 0)-(GetX() ?? 0), (GetY(windwing) ?? 0)-(GetY() ?? 0)));
  ShowInfoBar(IBRE, this(), wind,100, 5,1,1);
  return(1);
}


/* TimerCall */

private func CheckWindwing(...) {
  // Komplett, aber kein Windrad da: reduzieren für Neuaufbau/Reparatur
  if (And(Equal(GetCon(), 100), !windwing)) DoCon(-1);
  return(1);
}


/* Anzeigentext */

private func InfoBarText(barID, fill, maxf)
{
  if (Equal(barID, IBRE))
    return(Format("@Wind:  %d", fill));
  return(nil);
}


/* Eigenschaften */

public func InfoBarPos(...) {  return(-10);
}
public func InfoBarSize(...) { return(1);
}
public func GetMaxDamage(...) { return(80);
}

