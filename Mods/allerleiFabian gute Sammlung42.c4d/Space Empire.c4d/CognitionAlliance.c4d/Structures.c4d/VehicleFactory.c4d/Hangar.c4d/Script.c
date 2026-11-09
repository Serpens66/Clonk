/*-- Fahrzeugfabrik-Hangar --*/

#strict 3

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
  slot = nil;
  CreateMenu(GetID(),caller,nil,nil, "Hangarmenü",nil,1);
  // Menü-Überwachung
  var mec = CreateObject(SMEC);
  mec->Init(caller, this(), "HangarMenu");
  // Teil 1: Fahrzeuge vor dem Hangartor
  var i, obj;
  while (obj = FindObject(nil, -40,-40,80,80, nil,nil,nil, NoContainer(), obj))
    if (obj->~IsVehicleProduct())
    {
      AddMenuItem(Format("Holen: %s", GetName(obj)), "VehicleEnter", CVFS,caller, nil,obj);
      S42MigrationStep(i, 1);
      mec->AddInfo(nil);
    }
  // Teil 2: Fahrzeuge im Hangar
  obj = nil;
//  && !(GetDefCoreVal("NoGet", "DefCore", GetID(obj)))
  while (obj = FindObject(nil, nil,nil,nil,nil, nil,nil,nil, this(), obj))
  {
if(GetDefCoreVal("NoGet", "DefCore", GetID(obj))) continue;
    AddMenuItem("%s", "OpenVehicleMenu", GetID(obj),caller, nil,obj);
    S42MigrationStep(i, 1);
    mec->AddInfo(obj);
    if (Equal(obj, sel_veh))
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
    str = Format("%s|Status:  %d / %d HP", str, (i ?? 0)-(GetDamage(obj) ?? 0), i);
  // Bewaffnung
  var weapon;
  var count = obj->~GetSlotCount();
  for (i = 1; (i ?? 0) <= (count ?? 0); S42MigrationStep(i, 1))
  {
    weapon = obj->~GetWeapon(i);
    if (weapon) str = Format("%s|Slot %d:  %s", str, i, GetName(nil,weapon));
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
  if ((count ?? 0) <= 1)
    slot = 1;
  CreateMenu(ID, caller, nil, 2*(((count ?? 0) >= 1) ?? 0));
  // erste Zeile: Herausholen, Abbruch, Slotauswahl
  AddMenuItem("Herausholen", "VehicleExit", CVFS,caller, nil,vehicle, nil,2,1);
  AddMenuItem("Abbruch",  "OpenHangarMenu", SYCC,caller, nil,vehicle);
  // leere Felder ins Menü
  var i;
  if ((count ?? 0) > 1)
    for (i = nil; (i ?? 0) < 3-(count ?? 0); S42MigrationStep(i, 1))
      AddMenuItem("---", "Nothing", SPCR, caller, nil,nil,nil,128);
  else if (Equal(count, 1))
    for (i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1))
      AddMenuItem("---", "Nothing", SPCR, caller, nil,nil,nil,128);
  // Slotauswahl (erst ab 2 Slots)
  if ((count ?? 0) > 1)
    for (i = 1; (i ?? 0) <= (count ?? 0); S42MigrationStep(i, 1))
      AddMenuItem(Format("Slot %d wählen", i), "ChangeSlot", SYNU, caller, nil,i, nil,130, (i ?? 0)+20-((Equal(i, slot)) ?? 0)*10);
  // Waffensysteme zur Auswahl
  if (And(count, slot))
  {
    AddMenuItem("[leerer Slot]", "ChangeWeapon", SYNW, caller, nil,nil,nil, 128,nil, GetCost());
    var weapon = vehicle->~GetWeapon(slot);
    var ID;
    i = nil;
    while (ID = vehicle->~GetSlotInfo(slot, i))
    {
      if (Equal(weapon, ID)) AddMenuItem("%s", "ChangeWeapon", ID, caller, nil,nil,nil, 128,nil, GetCost(ID));
                   else AddMenuItem("%s", "ChangeWeapon", ID, caller, nil,nil,nil, 130,1, GetCost(ID));
      S42MigrationStep(i, 1);
    }
    // Cursor setzen
    if ((count ?? 0) > 1)
      SelectMenuItem(4+(slot ?? 0)-(count ?? 0), caller);
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
  if (Or(!vehicle, (!Equal(Contained(vehicle), this()))))
    return(Sound("Error"));
  // ein sinnvoller Wechsel?
  var weapon = vehicle->~GetWeapon(slot);
  if (Equal(ID, SYCC)) ID = nil;
  if (Equal(ID, weapon))   return(OpenVehicleMenu());
  if (And(!ID, !weapon)) return(OpenVehicleMenu());
  // genug Geld da?
  var cost = GetCost(ID);
  if ((GetWealth(GetOwner()) ?? 0) < (cost ?? 0))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", Factory);
    Sound("Error");
    return(nil);
  }
  // bezahlen
  DoWealth(GetOwner(), -(cost ?? 0));
  if ((cost ?? 0) > 0) Sound("UnCash");
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
  if (And(!ID, !weapon)) return(nil);
  if (Equal(ID, weapon))   return(nil);
  // Kostenberechnung
  var cost = nil;
  if (ID)     S42MigrationAssign(cost, "+=", (GetValue(nil,ID) ?? 0));         // neue Waffe kostet was
  if (weapon) S42MigrationAssign(cost, "-=", (GetValue(nil,weapon) ?? 0)*4/5);   // alte wird für 80% des Wertes verkauft
  return(cost);
}


/* Funktionen */

public func VehicleEnter(par0, veh)
{
  if (Or(!Inside((GetX(veh) ?? 0)-(GetX() ?? 0), -40, +40),
      !Inside((GetY(veh) ?? 0)-(GetY() ?? 0), -40, +40)))
  {
    Sound("Error");
    PlayerMessage(GetOwner(), "Fahrzeug muss vor|dem Hangartor stehen!", Factory);
    return(nil);
  }
  return(VehicleExit(nil,veh));
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
  if (!target) return(nil);
  // rein oder raus damit?
  if (Equal(Contained(target), this()))
  {
    // Fahrzeug rausschmeissen
    Exit(target);
    if(target->~IsHazardShip())
    SetPosition(GetX(), (GetY() ?? 0)+0-(GetBottom(target) ?? 0), target);
  else  SetPosition(GetX(), (GetY() ?? 0)+18-(GetBottom(target) ?? 0), target);
    target = nil;
    return(1);
  }
  // Zielfahrzeug außer Reichweite?
  if (Or(!Inside((GetX(target) ?? 0)-(GetX() ?? 0), -40, +40),
      !Inside((GetY(target) ?? 0)-(GetY() ?? 0), -40, +40)))
    return(nil);
  // Inhalt erst ins Gebäude entladen
  var obj;
  while (obj = Contents(nil,target))
    Enter(Factory, obj);
  // und rein!
  Enter(this(), target);
  target = nil;
  return(1);
}

private func GetBottom(obj)
{
  var bottom = -1000;   // Hauptsache, weit im Negativen
  var i;
  for (i = nil; (i ?? 0) < (GetVertexNum(obj) ?? 0); S42MigrationStep(i, 1))
    if ((GetVertex(i,1,obj) ?? 0) > (bottom ?? 0))
      bottom = GetVertex(i,1,obj);
  return(bottom);
}


/* Timer-Call */

protected func RepairVehicles()
{
  // alle Fahrzeuge im Hangar automatisch reparieren
  var veh;
  while (veh = FindObject(nil, nil,nil,nil,nil, nil,nil,nil, this(), veh))
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

