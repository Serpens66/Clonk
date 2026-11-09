/*-- Fahrzeugfabrik --*/

#strict 3
#include BOB1   // Gebäude-Hilfsobjekt
#include XFER   // Transfer-Script

local basement, producer, manager, hangar;

public func IsWaste(){ return(10);}

/* Initialisierung */

protected func Construction()
{
  basement = CreateObject(BS30,nil,8,GetOwner());
}

protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  if (hangar)  RemoveObject(hangar);
  if (producer)
  {
    producer->StopProduction();
    RemoveObject(producer);
  }
  return(inherited());
}

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(-20,39);
  producer = CreateObject(C_F3,nil,nil,GetOwner());
  hangar = CreateObject(CVFH,-29,31,GetOwner());
  producer->Init(this(),hangar);
  hangar->Init(this());
}


/* Steuerung */

protected func ContainedUp(caller)
{
  [Hangarmenü]
  if (HasEMP()) return(Sound("Error"));
  // Oben-Taste führt zum Hangarmenü
  hangar->OpenHangarMenu(caller);
}

protected func ContainedDig(caller)
{
  [Aufträge]
  if (HasEMP()) return(Sound("Error"));
  producer->KeyDig(caller);
}

// EnoughEnergy // wird in factory skript festgelegt
func EnoughEnergy() 
{
    return(producer->EnoughEnergy());
}


/* Eigenschaften */

public func GetMaxDamage() { return(120); }
public func InfoBarSize()  { return(5); }
public func InfoBarPos()   { return(4); }
public func IsEnergyConsumer() { return(1); }
