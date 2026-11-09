/*-- Hauptbasis --*/

#strict
#include SDWU     // Script basiert auf Wohneinheit
#include XFER

local weapon;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(-2,-9);
}

protected func Construction()
{
  basement = CreateObject(BS20,0,+8,GetOwner());
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
  CreateMenu(GetID(), caller, 0,2, "Waffenwechsel",0,1);
  AddMenuItem("keine Waffe", "ChangeWeapon", SYNW,caller, 0,0,0, 128,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Laser",          "ChangeWeapon", SLSW,caller, 0,0,0, 128,0, GetCost(SLSW));
  if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Maschinengewehr","ChangeWeapon", STW1,caller, 0,0,0, 128,0, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Vulcan-Waffe",   "ChangeWeapon", STW4,caller, 0,0,0, 128,0, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Elektrowaffe",   "ChangeWeapon", STW3,caller, 0,0,0, 128,0, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Raketenwerfer", "ChangeWeapon", STW2,caller, 0,0,0, 128,0, GetCost(STW2));

}

public func HasWeaponSelection(){ return(1);}  // sagt dem Uplink, dass hier Waffenwechsel möglich ist


// Für die Sensoren die Waffenwechselfunktionen, manchmal auch in Sensoren appendto:
public func SensorenenWaffenwechsel(zahl)
{
if(zahl==1) {ChangeWeapon(SYNW);return(1);} //keine Waffe
if(zahl==2 && GetPlrKnowledge(GetOwner(),SLSW)) {ChangeWeapon(SLSW);return(1);} //Laser
if(zahl==3 && GetPlrKnowledge(GetOwner(),STW1)) {ChangeWeapon(STW1);return(1);} //Maschinenegewehr
if(zahl==4 && GetPlrKnowledge(GetOwner(),STW4)) {ChangeWeapon(STW4);return(1);} //Vulcan
if(zahl==5 && GetPlrKnowledge(GetOwner(),STW3)) {ChangeWeapon(STW3);return(1);} //Elektrowaffe
if(zahl==6 && GetPlrKnowledge(GetOwner(),STW2)) {ChangeWeapon(STW2);return(1);} //Raketenwerfer
PlayerMessage(GetOwner(),Format("Für Geschütz auf %s fehlt die Forschung",this()),this());return(1);
}

protected func ChangeWeapon(ID)
{
  // nochmal das gleiche gewählt?
  if (ID == SYNW) ID = 0;
  if (ID == GetID(weapon)) return(0);
  if (!ID && !weapon)      return(0);
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
  if (weapon)
    RemoveObject(weapon);
  // keine draufsetzen?
  if (!ID) return(1);
  // neue Waffe erzeugen
  weapon = CreateObject(ID, 0,-100,GetOwner());
  weapon->Init(this(),1,-41,-19);
  return(1);
}

private func GetCost(ID)
{
  // Waffe bereits verwendet?
  if (!ID && !weapon)      return(0);
  if (ID == GetID(weapon)) return(0);
  // Kostenberechnung
  var cost = 0;
  if (ID)     cost += GetValue(0,ID);         // neue Waffe kostet was
  if (weapon) cost -= GetValue(weapon)*4/5;   // alte wird für 80% des Wertes verkauft
  return(cost);
}


/* Eigenschaften */

public IsEnergyConsumer: return(weapon != 0);
public InfoBarSize: return(4);
public InfoBarPos:  return(6);
public GetMaxDamage: return(600);

