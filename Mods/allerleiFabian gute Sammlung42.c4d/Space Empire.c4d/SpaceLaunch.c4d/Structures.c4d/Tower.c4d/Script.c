/*-- Wachturm --*/

#strict
#include BOB1

local basement, manager;
local weapon;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(-3,-19);
}

protected func Construction()
{
  basement = CreateObject(BS13,0,+8,GetOwner());
}

protected func Destruction()
{
  PreDestruction();
  if (basement) RemoveObject(basement);
}

public func PreDestruction()
{
  if (manager) RemoveObject(manager);
  ChangeWeapon();     // Waffe abnehmen & verkaufen
  inherited();
}


/* Steuerung */

protected func ContainedDig(caller)
{
  [Waffenwechsel]
  CreateMenu(GetID(), caller, 0,2, "Waffenwechsel",0,1);
  AddMenuItem("keine Waffe", "ChangeWeapon", SYNW,caller, 0,0,"keine Bewaffnung", 128,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Maschinengewehr","ChangeWeapon", STW1,caller, 0,0,0, 128,0, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Vulcan-Waffe",   "ChangeWeapon", STW4,caller, 0,0,0, 128,0, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Raketenwerfer",  "ChangeWeapon", STW2,caller, 0,0,0, 128,0, GetCost(STW2));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Elektrowaffe",   "ChangeWeapon", STW3,caller, 0,0,0, 128,0, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Laser",          "ChangeWeapon", SLSW,caller, 0,0,0, 128,0, GetCost(SLSW));
  return(1);
}

// Für die Sensoren die Waffenwechselfunktionen:
public func SensorenenWaffenwechsel(zahl)
{
if(zahl==1) ChangeWeapon(SYNW); //keine Waffe
if(zahl==2) ChangeWeapon(SLSW); //Laser
if(zahl==3) ChangeWeapon(STW1); //Maschinenegewehr
if(zahl==4) ChangeWeapon(STW4); //Vulcan
if(zahl==5) ChangeWeapon(STW3); //Elektrowaffe
if(zahl==6) ChangeWeapon(STW2); //Raketenwerfer
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
  weapon->Init(this(),1);
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


/* Clonks im Wachturm */

protected func Collection2(clonk)
{
  // neuen Clonkschatten erzeugen
  if (GetID(clonk) != HZCK)
    return(0);
  CreateObject(STOF,0,0,GetOwner())->Init(this(), clonk);
  return(1);
}

protected func Ejection(clonk)
{
  // Clonkschatten löschen
  var figure;
  while (figure = FindObject(STOF, 0,0,0,0, 0,0,this(), 0,figure))
    if (figure->GetClonk() == clonk)
      return(RemoveObject(figure));
  return(1);
}


/* Energie */

public func EnoughEnergy()
{
  if (weapon)
    return(GetEnergy() > weapon->~EnergyNeed());
  return(GetEnergy());
}


/* Eigenschaften */

public IsEnergyConsumer: return(weapon != 0);
public InfoBarSize: return(2);
public GetMaxDamage: return(150);

