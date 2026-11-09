/*-- Werkstatt --*/

#strict

/* Stellt die Grundfunktionalität für Werkstätten zur Verfügung. */

#include CXEC // energie
#include BAS7
#include DOOR

local producer,manager;

public func IsWaste(){ return(10);} 



protected func Destruction()
{
  PreDestruction();
  return(1);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  if (producer) {
    producer->StopProduction();
    RemoveObject(producer);
  }
  return(_inherited());
}

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1); // wenn initlight nicht aufgerufen wird, gibts kein licht
  producer = CreateObject(C_F9,0,0,GetOwner()); 
  producer->Init(this()); 
  return(1);
}

// Steuerung des Fabrikobjektes

protected func ContainedUp(caller)
{
  [Produktion]
  if (HasEMP()) return(Sound("Error"));
  producer->KeyUp(caller);
  return(1);
}

protected func ContainedDig(caller)
{
  [Aufträge]
  if (HasEMP()) return(Sound("Error"));
  producer->KeyDig(caller);
  return(1);
}

public func EMP_Reaction()
{
  // producer->StopProduction(); // zu alt, gibt kein EMP
  return(1);
}

public func EnoughEnergy() // wird in factory skript festgelegt
{
  return(producer->EnoughEnergy());
}

// Eigenschaften

public func GetMaxDamage() { return(100); }
public func InfoBarSize() { return(5); }
public func InfoBarPos() { return(4); }
public func IsEnergyConsumer() { return(1); }



