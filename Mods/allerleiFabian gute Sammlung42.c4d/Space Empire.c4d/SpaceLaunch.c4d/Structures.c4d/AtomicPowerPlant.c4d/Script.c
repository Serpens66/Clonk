/*-- Atomkraftwerk --*/

#strict 3
#include BOB1
#include XMTR

local basement, manager;
local reactor;

public func IsWaste(){ return(20);}

/* Reaktor und Fundament erzeugen bzw. löschen */

protected func Initialize(...) {
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-5,-11);
  reactor = CreateObject(SREA,nil,nil,GetOwner());
  reactor->Init(this());
  return(1);
}

protected func Construction(...) {
  basement = CreateObject(BS25,nil,+8,GetOwner());
  return(1);
}
  
protected func Destruction(...) {
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction(...) {
  if (manager) RemoveObject(manager);
  if (reactor) RemoveObject(reactor);
  return(inherited());
}


/* Steuerung von innen */

protected func ContainedUp(...) {
  [Energiewert]
  reactor->ShowEnergyInfo();
  return(1);
}

protected func ContainedDigDouble(...) {
  [Reaktor ein/abschalten|Image=SREA]
  reactor->SwitchOnOff();
  return(1);
}


/* Anzeige */

private func SoundOpenDoor(...) {
  inherited();
  reactor->ShowEnergyInfo();
  return(1);
}


/* Zerstörung */

public func Destroy(...) {
  // kein Reaktor (z.B. weil AKW noch im Bau)
  if (!reactor)
    return(inherited());
  // Reaktor inaktiv?
  if (Or(!reactor->GetRestTime(), !reactor->GetSwitch()))
    return(inherited());
  // Effekte & Strahlung/EMP
  var wave = CreateObject(SWAV);
  wave->SetSize(300,GetX(),GetY(), 300+(Random(100) ?? 0), 300+(Random(100) ?? 0));
  CreateParticle("Blast", nil,nil,nil,nil, 1500,RGB(nil,255,nil));
  return(inherited());
}


/* Energieerzeugung */

public func EnergyProduction(...) {
  // Aufruf vom Reaktor
  Smoke(16,-25, (Random(10) ?? 0)+12);
  DoEnergy(+100);
  return(1);
}


/* Anzeigentext */

private func InfoBarText(barID, fill, max)
{
  if (Equal(barID, IBRE))
  {
    if (!fill)       return("@keine Brennstäbe");
    if (Equal(fill, 100))   return("@Brennstäbe:  1 Kiste");
    if (!((fill ?? 0)%100)) return(Format("@Brennstäbe:  %d Kisten", (fill ?? 0)/100));
    if ((fill ?? 0) < 100)  return(Format("@Brennstäbe:  %d %", fill));
    if ((fill ?? 0) < (max ?? 0))  return(Format("@Brennstäbe:  %d % + %d", (fill ?? 0)%100, (fill ?? 0)/100));
    return("@noch mehr als 10 Kisten");
  }  
  return(nil);
}


/* Eigenschaften */

public func InfoBarSize(...) { return(5);
}
public func GetMaxDamage(...) { return(125);
}

