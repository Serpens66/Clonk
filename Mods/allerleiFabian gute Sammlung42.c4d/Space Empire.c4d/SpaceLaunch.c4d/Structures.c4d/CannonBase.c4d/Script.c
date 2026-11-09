/*-- Geschützplattform --*/

#strict
#include BOB1

local basement, manager;
local fill, glow, weapon;


/* Initialisierung */

protected func Initialize()
{
  manager = CreateObject(BMAN,0,0,GetOwner());
  manager->Init(this());
  manager->InitLight(-10,-12);
  // Glühen
  glow = CreateObject(SCBG,0,0,GetOwner());
  glow->Init(this());
  fill = 0;
  return(1);
}

protected func Construction()
{
  basement = CreateObject(BS23,0,+8,GetOwner());
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
  CreateMenu(GetID(), caller, 0,2, "Waffenwechsel");
  AddMenuItem("kein Geschütz", "ChangeWeapon", SYCC,caller, 0,0,0, 128,0, GetCost());
  if (GetPlrKnowledge(GetOwner(),SCN1))
    AddMenuItem("Artilleriegeschütz","ChangeWeapon", SCN1,caller, 0,0,0, 128,0, GetCost(SCN1));
  if (GetPlrKnowledge(GetOwner(),SCN2))
    AddMenuItem("Bazookageschütz",   "ChangeWeapon", SCN2,caller, 0,0,0, 128,0, GetCost(SCN2));
  return(1);
}

protected func ChangeWeapon(ID)
{
  // nochmal das gleiche gewählt?
  if (ID == SYCC)
    ID = 0;
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
  weapon = CreateObject(ID, 0,0, GetOwner());
  weapon->Init(this());
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

public func GetWeapon()
{
  return(GetID(weapon));
}


/* Timer-Call */

protected func CheckEnergy()
{
  // so viel Energie wie möglich aufnehmen
  var fe = Min(GetEnergy(), GetMaxFill()-fill);
  // Energie auffüllen
  fill += fe;
  DoEnergy(-fe);
  return(1);
}

public GetFill: return(fill/3);
public GetMaxFill: return(10000);


/* Energie überprüfen */

public func UseEnergy(value)
{
  // genug Energie da?
  if (fill < value) return(0);
  // verbrauchen
  fill -= value;
  return(1);
}


/* Eigenschaften */

public IsEnergyConsumer: return(1);
public InfoBarSize: return(5);
public GetMaxDamage: return(150);

