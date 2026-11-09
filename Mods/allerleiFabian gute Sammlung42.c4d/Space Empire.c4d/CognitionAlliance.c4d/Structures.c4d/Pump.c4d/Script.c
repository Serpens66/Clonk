/*-- Pumpe --*/

#strict
#include SPMP  // Die SpaceLaunch-Pumpe


protected func Construction()
{
  basement = CreateObject(BS07,0,+8,GetOwner());
}  

protected func Initialize()
{
  manager=CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(),1);
  manager->InitLight(1,1);
  // Pumpbetrieb gleich aufnehmen
  SetAction("Pumping");
  CreateContents(BARL);
  Switch = 1;
}


/* Eigenschaften */

public func GetMaxDamage() { return(80); }
public func InfoBarSize() { return(2); }
public func InfoBarPos() { return(2); }
public func IsEnergyConsumer() { return(1); }
