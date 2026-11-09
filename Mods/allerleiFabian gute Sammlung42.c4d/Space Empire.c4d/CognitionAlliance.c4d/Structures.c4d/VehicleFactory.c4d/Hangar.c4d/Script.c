/*-- Fahrzeugfabrik-Hangar --*/

#strict

#include XFER   // Transfer-Script

local Factory, caller;
local vehicle, slot;
local target;           // Fahrzeug, das reingeholt bzw. rausgeschafft werden soll

public func IsTransferabl() {
  return (1);
}



/* Initialisierung */

public func Init(par_factory)
{
  Factory = par_factory;
}


/* Menü mit Hangar-Funktionen */

public func OpenHangarMenu(par_caller, sel_veh)
{
  // Menü öffnen
  if (!sel_veh)    // par_caller ist nur gültig, wenn sel_veh nicht angegeben
    caller = par_caller;
  slot = 0;
  CreateMenu(GetID(),caller,0,0, "Hangarmenü",0,1);
  // Menü-Überwachung
  var mec = CreateObject(SMEC);
  mec->Init(caller, this(), "HangarMenu");
  // Teil 1: Fahrzeuge vor dem Hangartor
  var i, obj;
  while (obj = FindObject(0, -40,-40,80,80, 0,0,0, NoContainer(), obj))
    if (obj->~IsVehicleProduct())
    {
      AddMenuItem(Format("Holen: %s", GetName(obj)), "VehicleEnter", CVFS,caller, 0,obj);
      ++i;
      mec->AddInfo(0);
    }
  // Teil 2: Fahrzeuge im Hangar
  obj = 0;
//  && !(GetDefCoreVal("NoGet", "DefCore", GetID(obj)))
  while (obj = FindObject(0, 0,0,0,0, 0,0,0, this(), obj))
  {
if(GetDefCoreVal("NoGet", "DefCore", GetID(obj))) continue;
    AddMenuItem("%s", "OpenVehicleMenu", GetID(obj),caller, 0,obj);
    ++i;
    mec->AddInfo(obj);
    if (obj == sel_veh)
      SelectMenuItem(i, caller);
  }
}

public func HangarMenu_Changed(selection, obj)
{
  // nichts anzeigen, wenn kein Fahrzeug angegeben
  if (!obj)
    return(PlayerMessage(GetOwner(), "", Factory));
  // Fahrzeugname
  var str = GetName(obj);
  // Beschädigung
  var i = obj->~GetMaxDamage();
  if (i)
    str = Format("%s|Status:  %d / %d HP", str, i-GetDamage(obj), i);
  // Bewaffnung
  var weapon;
  var count = obj->~GetSlotCount();
  for (i = 1; i <= count; ++i)
  {
    weapon = obj->~GetWeapon(i);
    if (weapon) str = Format("%s|Slot %d:  %s", str, i, GetName(0,weapon));
           else str = Format("%s|Slot %d:  [leer]", str, i);
  }
  // Text ausgeben
  PlayerMessage(GetOwner(), "@%s", Factory, str);
}

public func HangarMenu_Closed()
{
  PlayerMessage(GetOwner(), "", Factory);
}


/* Fahrzeugmenü */

protected func OpenVehicleMenu(ID, veh)
{
  // neues Menü öffnen
  if (!ID) ID = GetID(vehicle);
      else vehicle = veh;
  // Slots zählen
  var count = vehicle->~GetSlotCount();
  if (count <= 1)
    slot = 1;
  CreateMenu(ID, caller, 0, 2*(count >= 1));
  // erste Zeile: Herausholen, Abbruch, Slotauswahl
  AddMenuItem("Herausholen", "VehicleExit", CVFS,caller, 0,vehicle, 0,2,1);
  AddMenuItem("Abbruch",  "OpenHangarMenu", SYCC,caller, 0,vehicle);
  // leere Felder ins Menü
  var i;
  if (count > 1)
    for (i = 0; i < 3-count; ++i)
      AddMenuItem("---", "Nothing", SPCR, caller, 0,0,0,128);
  else if (count==1)
    for (i = 0; i < 3; ++i)
      AddMenuItem("---", "Nothing", SPCR, caller, 0,0,0,128);
  // Slotauswahl (erst ab 2 Slots)
  if (count > 1)
    for (i = 1; i <= count; ++i)
      AddMenuItem(Format("Slot %d wählen", i), "ChangeSlot", SYNU, caller, 0,i, 0,130, i+20-(i==slot)*10);
  // Waffensysteme zur Auswahl
  if (count && slot)
  {
    AddMenuItem("[leerer Slot]", "ChangeWeapon", SYNW, caller, 0,0,0, 128,0, GetCost());
    var weapon = vehicle->~GetWeapon(slot);
    var ID;
    i = 0;
    while (ID = vehicle->~GetSlotInfo(slot, i))
    {
      if (weapon == ID) AddMenuItem("%s", "ChangeWeapon", ID, caller, 0,0,0, 128,0, GetCost(ID));
                   else AddMenuItem("%s", "ChangeWeapon", ID, caller, 0,0,0, 130,1, GetCost(ID));
      ++i;
    }
    // Cursor setzen
    if (count > 1)
      SelectMenuItem(4+slot-count, caller);
  }
}

private func Nothing()
{
  return(OpenVehicleMenu());
}

private func ChangeSlot(par0, num)
{
  slot = num;
  Sound("Click");
  return(OpenVehicleMenu());
}

private func ChangeWeapon(ID)
{
  // Fahrzeug ist doch hoffentlich noch da?
  if (!vehicle || (Contained(vehicle) != this()))
    return(Sound("Error"));
  // ein sinnvoller Wechsel?
  var weapon = vehicle->~GetWeapon(slot);
  if (ID == SYCC) ID = 0;
  if (ID == weapon)   return(OpenVehicleMenu());
  if (!ID && !weapon) return(OpenVehicleMenu());
  // genug Geld da?
  var cost = GetCost(ID);
  if (GetWealth(GetOwner()) < cost)
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", Factory);
    Sound("Error");
    return(0);
  }
  // bezahlen
  DoWealth(GetOwner(), -cost);
  if (cost > 0) Sound("UnCash");
           else Sound("Cash");
  // und schließlich noch Ausrüstung wechseln
  vehicle->SetWeapon(slot, ID);
  // zurück zum Menü
  OpenVehicleMenu();
}

public func GetCost(ID)
{
  // Waffe bereits verwendet?
  var weapon = vehicle->~GetWeapon(slot);
  if (!ID && !weapon) return(0);
  if (ID == weapon)   return(0);
  // Kostenberechnung
  var cost = 0;
  if (ID)     cost += GetValue(0,ID);         // neue Waffe kostet was
  if (weapon) cost -= GetValue(0,weapon)*4/5;   // alte wird für 80% des Wertes verkauft
  return(cost);
}


/* Funktionen */

public func VehicleEnter(par0, veh)
{
  if (!Inside(GetX(veh)-GetX(), -40, +40) ||
      !Inside(GetY(veh)-GetY(), -40, +40))
  {
    Sound("Error");
    PlayerMessage(GetOwner(), "Fahrzeug muss vor|dem Hangartor stehen!", Factory);
    return(0);
  }
  return(VehicleExit(0,veh));
}

public func VehicleExit(par0, veh)
{
  // Tor öffnen
  SetAction("OpenGate");
  target = veh;
}


/* Action-Call */

protected func GateOpen()
{
  // nichts tun, wenn kein Zielfahrzeug festgelegt
  if (!target) return(0);
  // rein oder raus damit?
  if (Contained(target) == this())
  {
    // Fahrzeug rausschmeissen
    Exit(target);
    if(target->~IsHazardShip())
    SetPosition(GetX(), GetY()+0-GetBottom(target), target);
  else  SetPosition(GetX(), GetY()+18-GetBottom(target), target);
    target = 0;
    return(1);
  }
  // Zielfahrzeug außer Reichweite?
  if (!Inside(GetX(target)-GetX(), -40, +40) ||
      !Inside(GetY(target)-GetY(), -40, +40))
    return(0);
  // Inhalt erst ins Gebäude entladen
  var obj;
  while (obj = Contents(0,target))
    Enter(Factory, obj);
  // und rein!
  Enter(this(), target);
  target = 0;
  return(1);
}

private func GetBottom(obj)
{
  var bottom = -1000;   // Hauptsache, weit im Negativen
  var i;
  for (i = 0; i < GetVertexNum(obj); ++i)
    if (GetVertex(i,1,obj) > bottom)
      bottom = GetVertex(i,1,obj);
  return(bottom);
}


/* Timer-Call */

protected func RepairVehicles()
{
  // alle Fahrzeuge im Hangar automatisch reparieren
  var veh;
  while (veh = FindObject(0, 0,0,0,0, 0,0,0, this(), veh))
  {
    // was im Hangar steht, wird Eigentum des Spielers
    SetOwner(GetOwner(), veh);
    // muss beschädigt und reparierbar sein
    if (!GetDamage(veh))     continue;
    if (!veh->~Repairable()) continue;
    // Strom sollte auch da sein
    if (!Factory->EnoughEnergy()) continue;
    // Geld überprüfen
    if (!GetWealth(GetOwner())) continue;
    DoWealth(GetOwner(), -1);
    // Fahrzeug reparieren
    DoDamage(-5, veh);
  }
}

