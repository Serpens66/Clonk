/*-- Uplink (Heimatbasis) --*/

#strict 3
#include UPL0   // einige allgemeine Uplinkfunktionen

local freigegeben;


/* Animation starten */

public func Init(target)
{
  inherited(target);
  // Nachricht
  PlayerMessage(GetOwner(), "Ausgerichtet auf|Heimatbasis", this());
  Local(nil) = nil;
}


/* Timer-Call */

protected func CheckBuilding()
{
  // Gebäude noch da?
  if (!inherited()) return(nil);
  // Gold automatisch verkaufen
  var gold, sold = nil;
  while (gold = FindContents(GOLD, building))
  {
    RemoveObject(gold);
    DoWealth(GetOwner(), +5);
    sold = 1;
  }
  var clonk;
  while (clonk = FindObject(nil,nil,nil,nil,nil, OCF_CrewMember, nil,nil, building, clonk))
    while (gold = FindContents(GOLD, clonk))
    {
      RemoveObject(gold);
      DoWealth(GetOwner(), +5);
      sold = 1;
    }
  if (sold) Sound("Cash");
}


/* Direkter Kauf */

public func DirectBuy(clonk, ID, count)
{
  // kein caller, damit kein Menü geöffnet wird
  caller = nil;
  // für jedes count eins (versuchen zu) kaufen
  while (count)
  {
    BuyWare(ID);
    S42MigrationStep(count, -1);
  }
}


/*- Heimatbasis -*/

public func KeyUp(par_caller)
{
  if (inherited(par_caller))
 BuyMenu();
}

public func KeyDig(par_caller)
{
  if (inherited(par_caller))
    SellMenu();
}




/* Kaufmenü */

private func KaufFlaggeMenu(selection)
{
  // kein caller?
  if (!caller) return(nil);
  // Menü mit allen kaufbaren Objekten
  CreateMenu(GetID(building), caller, nil,2, "Es gibt nichts zu kaufen", nil,nil,1);
  // Flagge
    AddMenuItem("Kauf: %s", "BuyFlagge", FLAG, caller, GetHomebaseMaterial(GetOwner(),FLAG));
  // Menüeintrag setzen, falls einer angegeben ist
  SelectMenuItem(selection, caller);
}


private func BuyMenu(selection)
{
  // kein caller?
  if (!caller) return(nil);

  SetCommand(caller,"Buy",building);  // funktioniert, weil zum uplink einfach unsichtbare Flagge aufs Gebäude gesetzt wird

//deaktiviert, weil Kauf via Flagge weniger lagged.
/*
  // Menü mit allen kaufbaren Objekten
  CreateMenu(GetID(building), caller, 0,2, "Es gibt nichts zu kaufen", 0,0,1);
  // alle Homebasematerials durchgehen
  var i, ID;
  while (ID = GetHomebaseMaterial(GetOwner(),0,i,C4D_All()))
  {
    AddMenuItem("Kauf: %s", "BuyWare", ID, caller, GetHomebaseMaterial(GetOwner(),ID), i);
    ++i;
  }
  // Menüeintrag setzen, falls einer angegeben ist
  SelectMenuItem(selection, caller);
*/

}


/* Ware gekauft */

private func BuyFlagge(ID, selection)
{
  // genug finanzielle Rücklagen?
  if ((GetWealth(GetOwner()) ?? 0) < (GetValue(nil,ID) ?? 0))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", building);
    Sound("Error");
    return(KaufFlaggeMenu(selection));
  }
  // Ware auch noch da?
  if (!GetHomebaseMaterial(GetOwner(),ID))
  {
    Sound("Error");
    return(KaufFlaggeMenu(selection));
  }
  // Ware erzeugen
  var ware = CreateContents(ID, building);
  DoHomebaseMaterial(GetOwner(), ID, -1);
  // frische Clonks müssen noch zur Crew hinzugefügt werden
  if ((GetOCF(ware) ?? 0) & (OCF_CrewMember ?? 0))
    MakeCrewMember(ware, GetOwner());
  // erst noch bezahlen
  DoWealth(GetOwner(), -(GetValue(nil,ID) ?? 0));
  Sound("Cash");
  // und fertig
  return(KaufFlaggeMenu(selection));
}

private func BuyWare(ID, selection)
{
  // genug finanzielle Rücklagen?
  if ((GetWealth(GetOwner()) ?? 0) < (GetValue(nil,ID) ?? 0))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", building);
    Sound("Error");
    return(BuyMenu(selection));
  }
  // Ware auch noch da?
  if (!GetHomebaseMaterial(GetOwner(),ID))
  {
    Sound("Error");
    return(BuyMenu(selection));
  }
  // Ware erzeugen
  var ware = CreateContents(ID, building);
  DoHomebaseMaterial(GetOwner(), ID, -1);
  // frische Clonks müssen noch zur Crew hinzugefügt werden
  if ((GetOCF(ware) ?? 0) & (OCF_CrewMember ?? 0))
    MakeCrewMember(ware, GetOwner());
  // erst noch bezahlen
  DoWealth(GetOwner(), -(GetValue(nil,ID) ?? 0));
  Sound("Cash");
  // und fertig
  return(BuyMenu(selection));
}


/* Verkaufsmenü */

private func SellMenu(selection)
{
  // kein caller?
  if (!caller) return(nil);
  // Menü mit verkaufbaren Waren öffnen
  CreateMenu(GetID(building), caller, nil,2, Format("%s ist leer.",GetName(building)));
  var obj, ID;
  var i, count, value;
  while (obj = FindObject(nil,nil,nil,nil,nil,nil,nil,nil, building, obj))
  {
    // Clonks ignorieren
    if ((GetOCF(obj) ?? 0) & (OCF_CrewMember ?? 0))
      continue;
//NoGet Objecte ignorieren
if(GetDefCoreVal("NoGet", "DefCore", GetID(obj)))
continue;
    // gleiche ID wie zuvor?
    if (Equal(GetID(obj), ID))
    {
      // Zähler erhöhen
      S42MigrationStep(count, 1);
      continue;
    }
    // Menüeintrag für ID erzeugen
    if (ID)
    {
      AddMenuItem("Verkauf: %s", "SellWare", ID, caller, count, i, nil, 128,nil,value);
      S42MigrationStep(i, 1);
    }
    // neue ID, Zähler zurücksetzen
    ID = GetID(obj);
    count = 1;
    value = GetValue(obj);
  }
  // letzten Eintrag noch dazu
  if (ID)
  {
    AddMenuItem("Verkauf: %s", "SellWare", ID, caller, count, i, nil, 128,nil,value);
    S42MigrationStep(i, 1);
  }
  if (Equal(selection, i))
    S42MigrationStep(selection, -1);
  // mit ein paar leeren Einträge auffüllen
  while (Or(((i ?? 0) % 5), (Equal(i, nil))))   // zur Not gesamte erste Zeile füllen
  {
    AddMenuItem("---", "SellNothing", SPCR, caller, nil,i);
    S42MigrationStep(i, 1);
  }
  // zusätzliche Einträge
  AddMenuItem("Ausrichtung wechseln auf Handelsystem",  "ChangeMode", SYUP,caller, nil,nil, "Das Handelssystem wird gewählt. Hier kann sowohl mit Mitspielern Handel getrieben werden, als auch mit einem KI Händler, sofern dieser nicht deaktiviert wurde und erforscht ist", 2,2);
if(GetPlrKnowledge(GetOwner(),UUU1))
  AddMenuItem("Transfer",            "TransferMenu", SYUP,caller, nil,nil, "Transfer zu anderen Basen und Fahrzeugen mit Uplink. Reichweite: 500Meter.", 2,4);
  AddMenuItem("Uplink abnehmen", "AttachTargetLost", SYUP,caller, nil,nil, "Uplink deaktivieren und abnehmen.", 2,3);
  if (building->~HasWeaponSelection())
  {
    AddMenuItem("Waffenwechsel",    "SelectWeapons", SYUP,caller, nil,nil, "Bewaffnung der Basis austauschen.", 2,5);
    S42MigrationStep(i, 1);
  }
if(Equal(freigegeben, nil))
  AddMenuItem("System freigeben", "Freigabe", SYUP,caller, nil,caller, "Uplinksystem wird für alle NichtFeinde freigegeben. Diese können sowohl im Gebäude auf alles zugreifen, als auch aus ihrem eigenen Uplinksystem Dinge aus deinem System per Beam holen oder dir senden. Außerdem werden sie auch geheilt und gelöscht, was sonst nicht der Fall ist", 2,2);
if(Equal(freigegeben, 1))
  AddMenuItem("System sperren", "Freigabe", SYUP,caller, nil,caller, "Uplinksystem wird wieder gesperrt, sodass nur noch der Besitzer Zugriff hat.", 2,2);
  // und noch mehr auffüllen
//  i += 3;
//  while (i % 5)
//  {
//    AddMenuItem("---", "SellNothing", SYUP, caller, 0,i, 0,2,1);
//    ++i;
//  }
  // Menüeintrag setzen, falls einer angegeben ist
  SelectMenuItem(selection, caller);
}


/* Ware verkauft */

private func SellWare(ID, selection)
{
  // Ware überhaupt noch da?
  var ware = FindContents(ID, building);
  if (!ware)
  {
    Sound("Error");
    return(SellMenu(selection));
  }

// wenn in DefCore NoSell, dann nicht verkaufbar
if (GetDefCoreVal("NoSell", "DefCore", ID))
  {
    Sound("Error");PlayerMessage(GetOwner(),"Ist nicht verkäuflich",this());
    return(SellMenu(selection));
  }

  // zuerst das Geld
  DoWealth(GetOwner(), GetValue(ware));
  Sound("Cash");
  // SellTo beachten
  var sellID = ProtectedCall(ware, "SellTo");
  if (!sellID)
    sellID = ID;
  // unter Umständen zum Heimatbasismaterial hinzufügen
  if (GetDefCoreVal("Rebuy", "DefCore", sellID))
    DoHomebaseMaterial(GetOwner(), sellID, +1);
  // Objekt löschen
  RemoveObject(ware);
  // und fertig
  return(SellMenu(selection));
}

private func SellNothing(par0, selection)
{
  Sound("Error");
  return(SellMenu(selection));
}

