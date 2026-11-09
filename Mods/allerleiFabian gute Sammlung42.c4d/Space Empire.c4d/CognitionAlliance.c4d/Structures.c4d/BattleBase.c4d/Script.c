/*-- Kampfbasis --*/

#strict
#include BOB1 // Gebäude-Hilfsobjekt
#include CNBS
#include XFER

local basement, manager, weapon1, weapon2, dir;

protected func Construction()
{
  basement=CreateObject(BS22,0,8,GetOwner());
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
  if (weapon1) RemoveObject(weapon1);
  if (weapon2) RemoveObject(weapon2);
  return(inherited());
}

protected func Initialize()
{  
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(0,-10);
  return(1);
}

protected func ContainedDig(caller)
{
  [Verkauf/Waffenwechsel]
  // nichts tun, wenn hier ein Uplink drauf ist
  if (GetUplink())
    return(inherited(caller));
  SelectWeapons(caller);
  return(1); 
}

public func SelectWeapons(caller)
{
  [Waffenwechsel]
  // Menü zum Waffenwechsel
  CreateMenu(GetID(), caller, this(),1, "Waffenwechsel",0,1);
  
  // linke Waffe
//  AddMenuItem("linke Waffe",       "noop",         SYW1,caller, 0,0,"linken Waffenslot besetzen",  2,0);
  AddMenuItem("Links: keine Waffe",       "ChangeWeapon", SYNW,caller, 0,0,"keine Bewaffnung",            2,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),CPG1))
    AddMenuItem("Links: Plasmakanone1",   "ChangeWeapon", CPG1,caller, 0,0,0,                             2,0, GetCost(CPG1));
  if (GetPlrKnowledge(GetOwner(),CPG2))
    AddMenuItem("Links: Plasmakanone2",   "ChangeWeapon", CPG2,caller, 0,0,0,                             2,0, GetCost(CPG2));
  if (GetPlrKnowledge(GetOwner(),CMG1))
    AddMenuItem("Links: Maschinengewehr", "ChangeWeapon", CMG1,caller, 0,0,0,                             2,0, GetCost(CERA));
  
  // rechte Waffe  
//  AddMenuItem("rechte Waffe",      "noop",         SYW1,caller, 0,0,"rechten Waffenslot besetzen", 2,1);
  AddMenuItem("Rechts: keine Waffe",       "ChangeWeapon", SYNW,caller, 0,1,"keine Bewaffnung",            2,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),CPG1))
    AddMenuItem("Rechts: Plasmakanone1",   "ChangeWeapon", CPG1,caller, 0,1,0,                             2,0, GetCost(CPG1));
  if (GetPlrKnowledge(GetOwner(),CPG2))
    AddMenuItem("Plasmakanone2",   "ChangeWeapon", CPG2,caller, 0,1,0,                             2,0, GetCost(CPG2));
  if (GetPlrKnowledge(GetOwner(),CMG1))
    AddMenuItem("Rechts: Maschinengewehr", "ChangeWeapon", CMG1,caller, 0,1,0,                             2,0, GetCost(CERA));
  
  return(1);
}

// ne kleine Funktion, die nix macht
private func noop() { return(1); }

public HasWeaponSelection: return(1);   // sagt dem Uplink, dass hier Waffenwechsel möglich ist

protected func ChangeWeapon(ID)
{
  dir = Par(1);
  // nochmal das gleiche gewählt?
  if (ID == SYNW) ID = 0;
  if (dir) {
    if (ID == GetID(weapon1)) return(0);
    if (!ID && !weapon1)      return(0);
  }
  else {
    if (ID == GetID(weapon2)) return(0);
    if (!ID && !weapon2)      return(0);
  }
  // genug Geld da?
  var cost = GetCost(ID);
  if (GetWealth(GetOwner()) < cost)
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", this());
    Sound("Error");
    return(0);
  }
  // bezahlen
  DoWealth(GetOwner(), -cost);
  if (cost > 0) Sound("UnCash");
           else Sound("Cash");
  // alte Waffe weg
  if (dir)
    if (weapon1)
      RemoveObject(weapon1);
  if (!dir)
    if (weapon2)
      RemoveObject(weapon2);
  // keine draufsetzen?
  if (!ID) return(1);
  // neue Waffe erzeugen
  if (dir) {
    weapon1 = CreateObject(ID, 0,-100,GetOwner());
    weapon1->Init(this(),1,-33,-2,1);  //links
  }    
  else {
    weapon2 = CreateObject(ID, 0,-100,GetOwner());
    weapon2->Init(this(),1,+33,-2,-1);  // rechts
  }
  return(1);
}

private func GetCost(ID)
{
  var cost = 0;
  if (dir) {
    // Waffe bereits verwendet?
    if (!ID && !weapon1)      return(0);
    if (ID == GetID(weapon1)) return(0);
    // Kostenberechnung
    if (ID)     cost += GetValue(0,ID);         // neue Waffe kostet was
    if (weapon1) cost -= GetValue(weapon1)*4/5;   // alte wird für 80% des Wertes verkauft
  }
  else {
    // Waffe bereits verwendet?
    if (!ID && !weapon2)      return(0);
    if (ID == GetID(weapon2)) return(0);
    // Kostenberechnung
    if (ID)     cost += GetValue(0,ID);         // neue Waffe kostet was
    if (weapon2) cost -= GetValue(weapon2)*4/5;   // alte wird für 80% des Wertes verkauft
  }
  return(cost);
}


/* Energie */

public func EnoughEnergy()
{
  if (weapon1)
    return(GetEnergy() > weapon1->~EnergyNeed());
  // wenn keine Waffe montiert, kein Energie-Symbol anzeigen
  return(1);
}

// Eigenschaften

public func GetMaxDamage() { return(800); }
public func InfoBarSize() { return(5); }
public func InfoBarPos() { return(6); }
public func IsEnergyConsumer() { return(1); }
