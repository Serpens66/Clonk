/*-- Wachturm --*/

#strict 3
#include BOB1

local basement, manager;
local weapon;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this(), 1);
  manager->InitLight(-3,-19);
}

protected func Construction()
{
  basement = CreateObject(BS13,nil,+8,GetOwner());
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
  CreateMenu(GetID(), caller, nil,2, "Waffenwechsel",nil,1);
  AddMenuItem("keine Waffe", "ChangeWeapon", SYNW,caller, nil,nil,"keine Bewaffnung", 128,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),STW1))
    AddMenuItem("Maschinengewehr","ChangeWeapon", STW1,caller, nil,nil,nil, 128,nil, GetCost(STW1));
  if (GetPlrKnowledge(GetOwner(),STW4))
    AddMenuItem("Vulcan-Waffe",   "ChangeWeapon", STW4,caller, nil,nil,nil, 128,nil, GetCost(STW4));
  if (GetPlrKnowledge(GetOwner(),STW2))
    AddMenuItem("Raketenwerfer",  "ChangeWeapon", STW2,caller, nil,nil,nil, 128,nil, GetCost(STW2));
  if (GetPlrKnowledge(GetOwner(),STW3))
    AddMenuItem("Elektrowaffe",   "ChangeWeapon", STW3,caller, nil,nil,nil, 128,nil, GetCost(STW3));
  if (GetPlrKnowledge(GetOwner(),SLSW))
    AddMenuItem("Laser",          "ChangeWeapon", SLSW,caller, nil,nil,nil, 128,nil, GetCost(SLSW));
  return(1);
}

// Für die Sensoren die Waffenwechselfunktionen:
public func SensorenenWaffenwechsel(zahl)
{
if(Equal(zahl, 1)) ChangeWeapon(SYNW); //keine Waffe
if(Equal(zahl, 2)) ChangeWeapon(SLSW); //Laser
if(Equal(zahl, 3)) ChangeWeapon(STW1); //Maschinenegewehr
if(Equal(zahl, 4)) ChangeWeapon(STW4); //Vulcan
if(Equal(zahl, 5)) ChangeWeapon(STW3); //Elektrowaffe
if(Equal(zahl, 6)) ChangeWeapon(STW2); //Raketenwerfer
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
  weapon->Init(this(),1);
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


/* Clonks im Wachturm */

protected func Collection2(clonk)
{
  // neuen Clonkschatten erzeugen
  if (!Equal(GetID(clonk), HZCK))
    return(nil);
  CreateObject(STOF,nil,nil,GetOwner())->Init(this(), clonk);
  return(1);
}

protected func Ejection(clonk)
{
  // Clonkschatten löschen
  var figure;
  while (figure = FindObject(STOF, nil,nil,nil,nil, nil,nil,this(), nil,figure))
    if (Equal(figure->GetClonk(), clonk))
      return(RemoveObject(figure));
  return(1);
}


/* Energie */

public func EnoughEnergy()
{
  if (weapon)
    return((GetEnergy() ?? 0) > (weapon->~EnergyNeed() ?? 0));
  return(GetEnergy());
}


/* Eigenschaften */

public func IsEnergyConsumer(...) { return(!Equal(weapon, nil));
}
public func InfoBarSize(...) { return(2);
}
public func GetMaxDamage(...) { return(150);
}

