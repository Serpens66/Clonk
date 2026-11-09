/*-- Chemiefabrik --*/

#strict 3
#include BOB1

local basement, producer, manager;  // Objektpointer für Hilfsobjekte


public func IsWaste(){ return(10);}


protected func Construction()
{
  basement = CreateObject(BS16,nil,8,GetOwner());
  return(1);
}

protected func Initialize()
{
  manager=CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(-7,-14);
  
  producer = CreateObject(C_F1,nil,nil,GetOwner());
  producer->Init(this());
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

// EnoughEnergy // wird in factory skript festgelegt
func EnoughEnergy() 
{
    return(producer->EnoughEnergy());
}

// Eigenschaften

public func GetMaxDamage() { return(100); }
public func InfoBarSize() { return(4); }
public func InfoBarPos() { return(4); }
public func IsEnergyConsumer() { return(1); }