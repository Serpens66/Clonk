/*-- Hauptbasis --*/

#strict 3
#include SDWU     // Script basiert auf Wohneinheit
#include XFER

local weapon;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-2,-9);
}

protected func Construction()
{
  basement = CreateObject(BS20,nil,+8,GetOwner());
}
  
public func PreDestruction()
{
  if (weapon) RemoveObject(weapon);
  return(inherited());     // erledigt auch den "manager"
}


/* Steuerung */

protected func ContainedDig(caller)
{
  [Waffenwechsel]
  // nichts tun, wenn hier ein Uplink drauf ist
  if (GetUplink())
    return(inherited(caller));
  SelectWeapons(caller);
}

public func SelectWeapons(caller)
{
  [Waffenwechsel]
  // Menü zum Waffenwechsel
  CreateMenu(GetID(), caller, nil,2, "Waffenwechsel",nil,1);
  AddMenuItem("keine Waffe", "ChangeWeapon", SYNW,caller, nil,nil,nil, 128,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Laser",          "ChangeWeapon", SLSW,caller, nil,nil,nil, 128,nil, GetCost(SLSW));
  if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Maschinengewehr","ChangeWeapon", STW1,caller, nil,nil,nil, 128,nil, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Vulcan-Waffe",   "ChangeWeapon", STW4,caller, nil,nil,nil, 128,nil, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Elektrowaffe",   "ChangeWeapon", STW3,caller, nil,nil,nil, 128,nil, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Raketenwerfer", "ChangeWeapon", STW2,caller, nil,nil,nil, 128,nil, GetCost(STW2));

}

public func HasWeaponSelection(){ return(1);}  // sagt dem Uplink, dass hier Waffenwechsel möglich ist


// Für die Sensoren die Waffenwechselfunktionen, manchmal auch in Sensoren appendto:
public func SensorenenWaffenwechsel(zahl)
{
if(Equal(zahl, 1)) {ChangeWeapon(SYNW);return(1);} //keine Waffe
if(And(Equal(zahl, 2), GetPlrKnowledge(GetOwner(),SLSW))) {ChangeWeapon(SLSW);return(1);} //Laser
if(And(Equal(zahl, 3), GetPlrKnowledge(GetOwner(),STW1))) {ChangeWeapon(STW1);return(1);} //Maschinenegewehr
if(And(Equal(zahl, 4), GetPlrKnowledge(GetOwner(),STW4))) {ChangeWeapon(STW4);return(1);} //Vulcan
if(And(Equal(zahl, 5), GetPlrKnowledge(GetOwner(),STW3))) {ChangeWeapon(STW3);return(1);} //Elektrowaffe
if(And(Equal(zahl, 6), GetPlrKnowledge(GetOwner(),STW2))) {ChangeWeapon(STW2);return(1);} //Raketenwerfer
PlayerMessage(GetOwner(),Format("Für Geschütz auf %s fehlt die Forschung",this()),this());return(1);
}

protected func ChangeWeapon(ID)
{
  // nochmal das gleiche gewählt?
  if (Equal(ID, SYNW)) ID = nil;
  if (Equal(ID, GetID(weapon))) return(nil);
  if (And(!ID, !weapon))      return(nil);
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
  if (weapon)
    RemoveObject(weapon);
  // keine draufsetzen?
  if (!ID) return(1);
  // neue Waffe erzeugen
  weapon = CreateObject(ID, nil,-100,GetOwner());
  weapon->Init(this(),1,-41,-19);
  return(1);
}

private func GetCost(ID)
{
  // Waffe bereits verwendet?
  if (And(!ID, !weapon))      return(nil);
  if (Equal(ID, GetID(weapon))) return(nil);
  // Kostenberechnung
  var cost = nil;
  if (ID)     S42MigrationAssign(cost, "+=", (GetValue(nil,ID) ?? 0));         // neue Waffe kostet was
  if (weapon) S42MigrationAssign(cost, "-=", (GetValue(weapon) ?? 0)*4/5);   // alte wird für 80% des Wertes verkauft
  return(cost);
}


/* Eigenschaften */

public func IsEnergyConsumer(...) { return(!Equal(weapon, nil));
}
public func InfoBarSize(...) { return(4);
}
public func InfoBarPos(...) {  return(6);
}
public func GetMaxDamage(...) { return(600);
}

