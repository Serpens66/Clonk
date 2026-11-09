/*-- Geschützplattform --*/

#strict 3
#include BOB1

local basement, manager;
local fill, glow, weapon;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,nil,nil,GetOwner());
  manager->Init(this());
  manager->InitLight(-10,-12);
  // Glühen
  glow = CreateObject(SCBG,nil,nil,GetOwner());
  glow->Init(this());
  fill = nil;
  return(1);
}

protected func Construction()
{
  basement = CreateObject(BS23,nil,+8,GetOwner());
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
  if (glow)    RemoveObject(glow);
  ChangeWeapon();       // Geschütz abnehmen & verkaufen
  return(inherited());
}


/* Steuerung */

protected func ContainedUp(caller)
{
  [Kanonensteuerung]
  if (!weapon) {
  // Anzeige
  ShowInfoBar(IBRE, this(), GetFill(), GetMaxFill(), 5, 1);return(1);}
  // Clonk in die Kanone verlegen
  weapon->Entering(caller);
 // Anzeige
  ShowInfoBar(IBRE, this(), GetFill(), GetMaxFill(), 5, 1);
  return(1);
}

protected func ContainedDig(caller)
{
  [Waffenwechsel]
  CreateMenu(GetID(), caller, nil,2, "Waffenwechsel");
  AddMenuItem("kein Geschütz", "ChangeWeapon", SYCC,caller, nil,nil,nil, 128,nil, GetCost());
  if (GetPlrKnowledge(GetOwner(),SCN1))
    AddMenuItem("Artilleriegeschütz","ChangeWeapon", SCN1,caller, nil,nil,nil, 128,nil, GetCost(SCN1));
  if (GetPlrKnowledge(GetOwner(),SCN2))
    AddMenuItem("Bazookageschütz",   "ChangeWeapon", SCN2,caller, nil,nil,nil, 128,nil, GetCost(SCN2));
  return(1);
}

protected func ChangeWeapon(ID)
{
  // nochmal das gleiche gewählt?
  if (Equal(ID, SYCC))
    ID = nil;
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
  weapon = CreateObject(ID, nil,nil, GetOwner());
  weapon->Init(this());
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

public func GetWeapon()
{
  return(GetID(weapon));
}


/* Timer-Call */

protected func CheckEnergy()
{
  // so viel Energie wie möglich aufnehmen
  var fe = Min(GetEnergy(), (GetMaxFill() ?? 0)-(fill ?? 0));
  // Energie auffüllen
  S42MigrationAssign(fill, "+=", (fe ?? 0));
  DoEnergy(-(fe ?? 0));
  return(1);
}

public func GetFill(...) { return((fill ?? 0)/3);
}
public func GetMaxFill(...) { return(10000);
}


/* Energie überprüfen */

public func UseEnergy(value)
{
  // genug Energie da?
  if ((fill ?? 0) < (value ?? 0)) return(nil);
  // verbrauchen
  S42MigrationAssign(fill, "-=", (value ?? 0));
  return(1);
}


/* Eigenschaften */

public func IsEnergyConsumer(...) { return(1);
}
public func InfoBarSize(...) { return(5);
}
public func GetMaxDamage(...) { return(150);
}

