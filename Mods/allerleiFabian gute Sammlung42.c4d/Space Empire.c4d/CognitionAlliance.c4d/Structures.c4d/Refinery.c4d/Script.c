/*-- Raffinerie --*/

#strict
#include BOB1  // Gebäude-Hilfsobjekt


local basement, producer, manager;

public func IsWaste(){ return(10);}

protected func Construction()
{
  basement = CreateObject(BS24,0,8,GetOwner()); 
  return(1);
}

protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  
  if (producer) {
    producer->StopProduction();
    RemoveObject(producer);
  }
  return(inherited());
}


protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(-59,35);
  
  producer = CreateObject(C_F2,0,0,GetOwner()); 
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
  producer->StopProduction();
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
