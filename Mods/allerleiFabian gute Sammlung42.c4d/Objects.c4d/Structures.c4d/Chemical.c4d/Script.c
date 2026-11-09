/*-- Chemiefabrik --*/

#strict

/* Nutzt die Grundfunktionalität der Werkstatt. */

#include WRKS
#include BAS3


public func IsWaste(){ return(20);}

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner()); // fuer das Energiezeichen
  manager->Init(this(),1); // wenn initlight nicht aufgerufen wird, gibts kein licht
  producer = CreateObject(C_F8,0,0,GetOwner()); 
  producer->Init(this()); 
  return(1);
}

/* Türsteuerung */

private func SoundOpenDoor()
{
  Sound("SteelGate2");
}
  
private func SoundCloseDoor()
{
  Sound("SteelGate2");
}

func Collection()
{
    Sound("Clonk");
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