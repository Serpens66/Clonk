/*-- Pumpe --*/

#strict 3
#include BOB2

local basement, manager;
local Switch, TransferReady;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  // Pumpbetrieb gleich aufnehmen
  SetAction("Pumping");
  CreateContents(BARL);
  Switch = 1;
  TransferReady = nil;
}

protected func Construction()
{
  basement = CreateObject(BS06,nil,+8,GetOwner());
}
  
protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  var obj;
  while (obj = Contents())
    RemoveObject(obj);
  return(inherited());
}


/* Steuerung */

protected func ControlDig(caller)
{
  [Ein/Ausschalten]
  if (Hostile(GetController(caller),GetOwner())) return(nil);
  if (HasEMP()) return(Sound("Error"));
  Switch = !Switch;
  if (Switch) SwitchOn();
         else SwitchOff();
}

private func SwitchOn()
{
  SetAction("Pumping");
  Message("eingeschaltet",this());
}

private func SwitchOff()
{
  SetAction("Idle");
  Message("ausgeschaltet",this());
}

/* Pumpkontrolle */

public func AcceptTransfer()
{
  return(And(Switch, EnoughEnergy()));
}


/* Timer-Call */

private func CheckEnergy()
{
  // bei EMP ausschalten
  if (HasEMP()) SetAction("Idle");
  // Pump-Aktion nach EMP reeaktivieren
  if (ActIdle())
    if (And(Switch, EnoughEnergy())) SetAction("Pumping");
                             else return(nil);
  // Energieverbrauch
  DoEnergy(-10);
  // keine Energie mehr?
  if (!GetEnergy()) SetAction("Idle");
}


/* Energie */

public func EnoughEnergy(...) { return((GetEnergy() ?? 0) >= 10);
}
public func IsEnergyConsumer(...) { return(1);
}


/* Eigenschaften */

public func InfoBarSize(...) {  return(1);
}
public func GetMaxDamage(...) { return(50);
}

