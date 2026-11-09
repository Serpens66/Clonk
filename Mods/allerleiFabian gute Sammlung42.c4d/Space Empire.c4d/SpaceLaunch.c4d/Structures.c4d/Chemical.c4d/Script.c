/*-- Chemielabor --*/

#strict 3
#include BOB1

local basement, manager;
local factory;

public func IsWaste(){ return(10);}


/* Factory-Objekt und Fundament erzeugen bzw. löschen */

protected func Initialize(...) {
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(),1); // mit energiesymbol
  factory = CreateObject(SOF1,nil,nil,GetOwner());
  factory->Init(this(),15,nil);
  manager->InitLight(-7,-14);
  return(1);
}

protected func Construction(...) {
  basement = CreateObject(BS14,nil,+8,GetOwner());
  return(1);
}
  
protected func Destruction(...) {
  PreDestruction();
  if (basement) RemoveObject(basement);
  return(1);
}

public func PreDestruction(...) {
  if (factory)
  {
    factory->StopProduction();
    RemoveObject(factory);
  }
  if (manager) RemoveObject(manager);
  return(inherited());
}


/* Steuerung */

protected func ContainedUp(...) {
  [Produktion]
  factory->KeyUp(Par(nil));
  return(1);
}

protected func ContainedDig(...) {
  [Auftragsmenü]
  factory->KeyDig(Par(nil));
  return(1);
}


/* Inhalt von Loren übernehmen */

private func DoorClosed(...) {
  var num = -1;
  var obj;
  while (obj = Contents(S42MigrationStep(num, 1)))
    if (obj->~IsLorry())
    {
      GrabContents(obj);
      SetCommand(obj,"Exit");
    }
  return(1);
}

// EnoughEnergy // wird in factory skript festgelegt
func EnoughEnergy() 
{
    return(factory->EnoughEnergy());
}
  
/* Eigenschaften */

public func InfoBarSize(...) { return(2);
}
public func GetMaxDamage(...) { return(100);
}
public func IsEnergyConsumer() { return(1); }
