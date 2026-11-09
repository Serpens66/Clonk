/*-- Kampfbasis --*/

#strict 3
#include BOB1 // Gebäude-Hilfsobjekt
#include CNBS
#include XFER

local basement, manager, weapon1, weapon2, dir;

protected func Construction()
{
  basement=CreateObject(BS22,nil,8,GetOwner());
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
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(nil,-10);
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
  CreateMenu(GetID(), caller, this(),1, "Waffenwechsel",nil,1);
  
  // linke Waffe
//  AddMenuItem("linke Waffe",       "noop",         SYW1,caller, 0,0,"linken Waffenslot besetzen",  2,0);
  AddMenuItem("Links: keine Waffe",       "ChangeWeapon", SYNW,caller, nil,nil,"keine Bewaffnung",            2,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),CPG1))
    AddMenuItem("Links: Plasmakanone1",   "ChangeWeapon", CPG1,caller, nil,nil,nil,                             2,nil, GetCost(CPG1));
  if (GetPlrKnowledge(GetOwner(),CPG2))
    AddMenuItem("Links: Plasmakanone2",   "ChangeWeapon", CPG2,caller, nil,nil,nil,                             2,nil, GetCost(CPG2));
  if (GetPlrKnowledge(GetOwner(),CMG1))
    AddMenuItem("Links: Maschinengewehr", "ChangeWeapon", CMG1,caller, nil,nil,nil,                             2,nil, GetCost(CERA));
  
  // rechte Waffe  
//  AddMenuItem("rechte Waffe",      "noop",         SYW1,caller, 0,0,"rechten Waffenslot besetzen", 2,1);
  AddMenuItem("Rechts: keine Waffe",       "ChangeWeapon", SYNW,caller, nil,1,"keine Bewaffnung",            2,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),CPG1))
    AddMenuItem("Rechts: Plasmakanone1",   "ChangeWeapon", CPG1,caller, nil,1,nil,                             2,nil, GetCost(CPG1));
  if (GetPlrKnowledge(GetOwner(),CPG2))
    AddMenuItem("Plasmakanone2",   "ChangeWeapon", CPG2,caller, nil,1,nil,                             2,nil, GetCost(CPG2));
  if (GetPlrKnowledge(GetOwner(),CMG1))
    AddMenuItem("Rechts: Maschinengewehr", "ChangeWeapon", CMG1,caller, nil,1,nil,                             2,nil, GetCost(CERA));
  
  return(1);
}

// ne kleine Funktion, die nix macht
private func noop() { return(1); }

public func HasWeaponSelection(...) { return(1);   // sagt dem Uplink, dass hier Waffenwechsel möglich ist
}

protected func ChangeWeapon(ID)
{
  dir = Par(1);
  // nochmal das gleiche gewählt?
  if (Equal(ID, SYNW)) ID = nil;
  if (dir) {
    if (Equal(ID, GetID(weapon1))) return(nil);
    if (And(!ID, !weapon1))      return(nil);
  }
  else {
    if (Equal(ID, GetID(weapon2))) return(nil);
    if (And(!ID, !weapon2))      return(nil);
  }
  // genug Geld da?
  var cost = GetCost(ID);
  if ((GetWealth(GetOwner()) ?? 0) < (cost ?? 0))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", this());
    Sound("Error");
    return(nil);
  }
  // bezahlen
  DoWealth(GetOwner(), -(cost ?? 0));
  if ((cost ?? 0) > 0) Sound("UnCash");
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
    weapon1 = CreateObject(ID, nil,-100,GetOwner());
    weapon1->Init(this(),1,-33,-2,1);  //links
  }    
  else {
    weapon2 = CreateObject(ID, nil,-100,GetOwner());
    weapon2->Init(this(),1,+33,-2,-1);  // rechts
  }
  return(1);
}

private func GetCost(ID)
{
  var cost = nil;
  if (dir) {
    // Waffe bereits verwendet?
    if (And(!ID, !weapon1))      return(nil);
    if (Equal(ID, GetID(weapon1))) return(nil);
    // Kostenberechnung
    if (ID)     S42MigrationAssign(cost, "+=", (GetValue(nil,ID) ?? 0));         // neue Waffe kostet was
    if (weapon1) S42MigrationAssign(cost, "-=", (GetValue(weapon1) ?? 0)*4/5);   // alte wird für 80% des Wertes verkauft
  }
  else {
    // Waffe bereits verwendet?
    if (And(!ID, !weapon2))      return(nil);
    if (Equal(ID, GetID(weapon2))) return(nil);
    // Kostenberechnung
    if (ID)     S42MigrationAssign(cost, "+=", (GetValue(nil,ID) ?? 0));         // neue Waffe kostet was
    if (weapon2) S42MigrationAssign(cost, "-=", (GetValue(weapon2) ?? 0)*4/5);   // alte wird für 80% des Wertes verkauft
  }
  return(cost);
}


/* Energie */

public func EnoughEnergy()
{
  if (weapon1)
    return((GetEnergy() ?? 0) > (weapon1->~EnergyNeed() ?? 0));
  // wenn keine Waffe montiert, kein Energie-Symbol anzeigen
  return(1);
}

// Eigenschaften

public func GetMaxDamage() { return(800); }
public func InfoBarSize() { return(5); }
public func InfoBarPos() { return(6); }
public func IsEnergyConsumer() { return(1); }
