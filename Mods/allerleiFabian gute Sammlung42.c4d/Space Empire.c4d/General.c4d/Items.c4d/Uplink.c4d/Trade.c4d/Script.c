/*-- Uplink (Handelssystem) --*/

#strict 3
#include UPL0

local input, inputID;


/* Animation starten */

public func Init(target)
{
  inherited(target);
  // Nachricht
  PlayerMessage(GetOwner(), "Ausgerichtet auf|Handelssystem", this());
  Local(nil) = 1;
}


/*- Handelssytem -*/

public func KeyUp(par_caller)
{
  if (inherited(par_caller))
    BuyObjects(1,nil);
}

public func KeyDig(par_caller)
{
  if (inherited(par_caller))
    ExtMenu();
}


/* Kaufmenü */

private func BuyObjects(category, selection)
{
  // Menü mit allen Objekten dieser Kategorie
  CreateMenu(GetID(GetActionTarget()), caller, nil,nil,nil,nil,nil,1);
  var mec = CreateObject(SMEC);   // Menü-Überwachung
  mec->Init(caller,this(), "BuyObjects",1);
  // Übersicht über Kategorien
  AddMenuItem("Kategorie: Materialien", "BuyChangeCat", SYT0,caller, nil,1, "Materialien und Rohstoffe kaufen.",  2, 0 + ((!Equal(category, 1)) ?? 0) * 5);
  AddMenuItem("Kategorie: Gegenstände", "BuyChangeCat", SYT0,caller, nil,2, "Grundlegende Gegenstände kaufen.",   2, 1 + ((!Equal(category, 2)) ?? 0) * 5);
  AddMenuItem("Kategorie: Ausrüstung",  "BuyChangeCat", SYT0,caller, nil,3, "Ausrüstung und Munition kaufen.",    2, 2 + ((!Equal(category, 3)) ?? 0) * 5);
  AddMenuItem("Kategorie: Waffen",      "BuyChangeCat", SYT0,caller, nil,4, "Tragbare Waffen kaufen.",            2, 3 + ((!Equal(category, 4)) ?? 0) * 5);
  AddMenuItem("Kategorie: Fahrzeuge",   "BuyChangeCat", SYT0,caller, nil,5, "Fahrzeuge und großes Gerät kaufen.", 2, 4 + ((!Equal(category, 5)) ?? 0) * 5);
  var i = 6;
  while (S42MigrationStep(i, -1))
    mec->AddInfo(-1);
  // richtige Kategorie anwählen
  var warelist = FindObject(STWL);
  var start = warelist->CategoryStart(category);
  var size = warelist->CategorySize(category);
  // alle IDs durchgehen
  var warenum, ID;
  for (i = nil; (i ?? 0) < (size ?? 0); S42MigrationStep(i, 1))
  {
    warenum = (start ?? 0)+(i ?? 0);
    ID = GetWareID(warenum);               // Ware wird zugelassen:
    if (Or(((category ?? 0) <= 2),                 //  - automatisch in den ersten beiden Kategorien
        GetPlrKnowledge(GetOwner(),ID)))    //  - oder wenn der Bauplan erforschbar ist
    {
      AddMenuItem("Kauf: %s", "BuyWare", ID, caller, AllAvailableAmount(warenum,1), warenum);
      mec->AddInfo(warenum);
    }
  }
  // Menüeintrag setzen, falls einer angegeben ist
  SelectMenuItem(selection, caller);
}

public func BuyObjects_Changed(selection, warenum)
{
  // ungültige Warennummer?
  if (Equal(warenum, -1))
    return(PlayerMessage(GetOwner(), "", this()));
  // billigsten Preis ermitteln
  var cheapest = GetCheapestPrice(warenum);
  // gibt's ein eigenes Angebot?
  var amount, price;
  if (OffersWare(warenum, GetOwner()))
  {
    // Angebotsmenge und Preis
    amount = GetAvailableAmount(warenum, GetOwner());
    price = GetWarePrice(warenum, GetOwner());
    if (And(amount, (Equal(price, cheapest))))
    {
      PlayerMessage(GetOwner(), "@%s|Wert: %d Gold|<c 80ff80>Eigenes Angebot: %d Stück für je %d Gold", this(),
                    GetName(nil,GetWareID(warenum)), GetValue(nil,GetWareID(warenum)), amount, price);
      return(1);
    }
  }
  // andere Spieler der Reihe nach durchgehen
  var player, cplayer, splayer;
  var allamount;
  var i = -1;
  var j = GetPlayerCount();
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
  {
    if ((i ?? 0) >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (Equal(player, GetOwner()))         continue;
    if (!OffersWare(warenum, player)) continue;
    // Angebotsmenge und Preis
    amount = GetAvailableAmount(warenum, player);
    price = GetWarePrice(warenum, player);
    if (Equal(price, cheapest))
    {
      S42MigrationStep(splayer, 1);
      cplayer = player;
      S42MigrationAssign(allamount, "+=", (amount ?? 0));
    }
  }
  // Infos ausgeben
  var infostring;
  if (Equal(splayer, nil)) infostring = "<c c0c0c0>- keine Angebote verfügbar -";
  if (Equal(splayer, 1))
    if (Equal(cplayer, -1)) infostring = Format("KI-Händler: %d Stück für je %d Gold", allamount, cheapest);
                  else infostring = Format("%s: %d Stück für je %d Gold", GetPlayerName(cplayer), allamount, cheapest);
  if ((splayer ?? 0) >= 2) infostring = Format("%d Spieler: %d Stück für je %d Gold", splayer, allamount, cheapest);
  // Info ausgeben
  PlayerMessage(GetOwner(), "@%s|Wert: %d Gold|%s", this(), GetName(nil,GetWareID(warenum)), GetValue(nil,GetWareID(warenum)), infostring);
}

public func BuyObjects_Closed()
{
  PlayerMessage(GetOwner(), "", this());
}


/* Kategorie wechseln */

private func BuyChangeCat(par0, category)
{
  Sound("Click");
  BuyObjects(category, (category ?? 0)-1);
}

/* Ware gekauft */

private func BuyWare(ID, warenum)
{
  // erstmal den billigsten Spieler finden
  var player = GetCheapestPlayer(warenum);
  // keiner bietet die Ware an?
  if (Equal(player, -2))
    return(BuyWareReturn(warenum, nil));
  // eigenes Angebot das billigste?
  if (OffersWare(warenum, GetOwner()))
    if (GetAvailableAmount(warenum, GetOwner()))                                // nur wenn man selbst nicht ausverkauft ist
      if (Equal(GetWarePrice(warenum, player), GetWarePrice(warenum, GetOwner())))   // man selbst bietet auch am billigsten an?
        return(BuyWareReturn(warenum, nil));
  // reicht das Geld?
  var price = GetWarePrice(warenum, player);
  if ((GetWealth(GetOwner()) ?? 0) < (price ?? 0))
    return(BuyWareReturn(warenum, nil));
  // Kauf bei KI-Spieler
  if (Equal(player, -1))
  {
    // Ware erzeugen
    var ware = CreateContents(ID, GetActionTarget());
    S42MigrationStep(Local(warenum, FindObject(SAIT)), -1);
    // Geld überweisen
    DoWealth(GetOwner(), -(price ?? 0));
    // zurück zum Menü
    return(BuyWareReturn(warenum, 1));
  }
  // Ware finden und rüberbeamen
  var ware = FindWare(warenum, player);
  SetOwner(GetOwner(), ware);
  Enter(GetActionTarget(), ware);
  // Geld überweisen
  DoWealth(GetOwner(), -(price ?? 0));
  DoWealth(player, price);
  // zurück zum Menü
  return(BuyWareReturn(warenum, 1));
}

private func BuyWareReturn(warenum, success)
{
  if (success) Sound("Cash");
          else Sound("Error");
  // Menü mit richtiger Kategorie neu öffnen
  var category = FindObject(STWL)->FindCategory(warenum);
  return(BuyObjects(category, GetMenuSelection(caller)));
}


/* Erweitertes Menü */

private func ExtMenu()
{
  // Menü öffnen
  CreateMenu(GetID(GetActionTarget()),caller, nil,nil, "Handelsmenü");
  AddMenuItem("Verkaufsliste", "SellList",  SYT2,caller, nil,nil, "Auflistung der eigenen Angebote.");
  AddMenuItem("Neues Angebot", "SellOffer", SYT1,caller, nil,nil, "Neues Verkaufsangebot erstellen.");
  var i = 2;
  if ((GetPlayerCount() ?? 0) > 1)
  {
    AddMenuItem("Überweisung", "MoneyXfer", SYT3,caller, nil,nil, "Geld an anderen Spieler überweisen.");
    S42MigrationStep(i, 1);
  }
  // mit ein paar leeren Einträge auffüllen
  while ((i ?? 0) % 5)
  {
    AddMenuItem("---", "SellNothing", SPCR, caller, nil,i);
    S42MigrationStep(i, 1);
  }
  // zusätzliche Einträge
  AddMenuItem("Ausrichtung wechseln: Basis",  "ChangeMode", SYUP,caller, nil,nil, "Das Heimatbasissystem wird gewählt. Hat alle Funktionen einer normalen Basis.",   2,2);
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
//  i += 3;
  // und noch mehr auffüllen
//  while (i % 5)
//  {
//    AddMenuItem("---", "SellNothing", SYUP, caller, 0,i, 0,2,1);
//    ++i;
//  }
}


/* Verkaufsliste */

private func SellList(par0, selnum)
{
  // Zahl der Handelswaren gesamt
  var size = FindObject(STWL)->GetListSize();
  // Menü mit allen verkaufbaren Objekten
  CreateMenu(SYT2, caller, nil,nil, "Verkaufsliste", nil,1);
  var mec = CreateObject(SMEC);         // Menü-Überwachung
  mec->Init(caller,this(), "SellList");
  // alle IDs durchgehen
  var selection;
  var i, j;
  for (i = nil; (i ?? 0) < (size ?? 0); S42MigrationStep(i, 1))
    if (OffersWare(i, GetOwner()))
    {
      if (Equal(i, selnum))  selection = j;
      S42MigrationStep(j, 1);
      // Menüeintrag hinzufügen
      AddMenuItem("%s", "EditOffer", GetWareID(i), caller, nil, i);
      mec->AddInfo(i);
    }
  AddMenuItem("Neu...", "SellOffer", SYT1,caller, nil,nil, "Neues Verkaufsangebot erstellen.");
  mec->AddInfo(-1);
  AddMenuItem("Abbrechen", "ExtMenu", SYCC,caller);
  mec->AddInfo(-1);
  // Menüauswahl setzen
  SelectMenuItem(selection, caller);
  return(1);
}

public func SellList_Changed(selection, warenum)
{
  // ungültige Warennummer?
  if (Equal(warenum, -1))
    return(PlayerMessage(GetOwner(), "", this()));
  // billigsten Preis ermitteln
  var cheapest = GetCheapestPrice(warenum, 1, GetOwner());   // Besitzer dabei ignorieren
  var price = GetWarePrice(warenum, GetOwner());
  var amount, allamount;
  // andere Spieler der Reihe nach durchgehen
  var player, cplayer, splayer;
  var i = -1;
  var j = GetPlayerCount();
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
  {
    if ((i ?? 0) >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (Equal(player, GetOwner())) continue;
    if (!OffersWare(warenum, player)) continue;
    // Angebotsmenge und Preis
    amount = GetAvailableAmount(warenum, player);
    price = GetWarePrice(warenum, player);
    if (Equal(price, cheapest))
    {
      S42MigrationStep(splayer, 1);
      cplayer = player;
      S42MigrationAssign(allamount, "+=", (amount ?? 0));
    }
  }
  // Infos ausgeben
  var infostring;
  if (Equal(splayer, nil)) infostring = "<c c0c0c0>- kein anderes Angebot -";
  if (Equal(splayer, 1))
    if (Equal(cplayer, -1)) infostring = Format("KI-Händler: %d Stück für je %d Gold", allamount, cheapest);
                  else infostring = Format("%s: %d Stück für je %d Gold", GetPlayerName(cplayer), allamount, cheapest);
  if ((splayer ?? 0) >= 2) infostring = Format("%d Spieler: %d Stück für je %d Gold", splayer, allamount, cheapest);
  // Einfärbung je nachdem, ob Angebot gültig
  amount = GetAvailableAmount(warenum, GetOwner());
  if (!amount) infostring = Format("<c ff8080>Eigenes Angebot: 0 Stück für je %d Gold|<c ffffff>%s", GetWarePrice(warenum, GetOwner()), infostring);
          else infostring = Format("<c 80ff80>Eigenes Angebot: %d Stück für je %d Gold|<c ffffff>%s", amount, GetWarePrice(warenum, GetOwner()), infostring);
  // Info ausgeben
  PlayerMessage(GetOwner(), "@%s|Wert: %d Gold|%s", this(), GetName(nil,GetWareID(warenum)), GetValue(nil,GetWareID(warenum)), infostring);
  return(1);
}

public func SellList_Closed()
{
  PlayerMessage(GetOwner(), "", this());
}


/* Neues Verkaufsangebot */

private func SellOffer()
{
  // Zahl der Handelswaren gesamt
  var size = FindObject(STWL)->GetListSize();
  // Menü mit allen verkaufaren Objekten
  CreateMenu(SYT1, caller);
  var mec = CreateObject(SMEC);   // Menü-Überwachung
  mec->Init(caller,this(), "BuyObjects");   // gleiche Anzeige wie beim Kaufen benutzen
  // alle IDs durchgehen
  var amount;
  var i;
  for (i = nil; (i ?? 0) < (size ?? 0); S42MigrationStep(i, 1))
    if (!OffersWare(i, GetOwner()))
    {
      amount = GetAvailableAmount(i, GetOwner());
      if (!amount) continue;
      // neuer Menüeintrag
      AddMenuItem("%s", "EditOffer", GetWareID(i), caller, amount, i);
      mec->AddInfo(i);
    }
  AddMenuItem("Abbrechen", "ExtMenu", SYCC,caller);
  mec->AddInfo(-1);
}


/* Angebot ändern */

private func EditOffer(ID, warenum)
{
  // eigenen Preis ermitteln
  var price = GetWarePrice(warenum, GetOwner());
  input = price;     // Startwert
  inputID = ID;      // ID der Ware
  // neues Menü
  CreateMenu(ID, caller, nil,nil, GetName(nil,ID), nil,nil,1);
  var mec = CreateObject(SMEC);                // Menü-Überwachung; hier nur benötigt, um Preisanzeige am Ende zu löschen
  mec->Init(caller,this(), "EditOffer", 1);    // removeothers = 1, damit PlayerMessage (siehe unten) nicht gelöscht wird
  // Ziffern
  var i;
  for (i = nil; (i ?? 0) <= 9; S42MigrationStep(i, 1, true))
    AddMenuItem(Format("Hinzufügen: %d",i), "Edit_AddNumber", SYNU, caller, nil,i,nil, 2,i);

//minuszeichen
    AddMenuItem("Negativ machen", "Edit_AddMinus", PCSO, caller);

  // Steuerbefehle
  if (price)
    AddMenuItem("Altes Angebot löschen", "Edit_DeleteOffer", SYDL, caller, nil,warenum);
  AddMenuItem("Ziffer löschen", "Edit_DeleteNumber", SYBA, caller);
  AddMenuItem("Fertig", "Edit_ChangeOffer", SYAC, caller, nil,warenum);
  // Menüauswahl setzen
  if (price) SelectMenuItem(12, caller);
        else SelectMenuItem(11, caller);
  // eingegebener Wert als Message
  PlayerMessage(GetOwner(), "@<c c0c0c0>Wert der Ware:  %d Gold|<c ffffff>Eigener Preis:  %d Gold", this(), GetValue(nil,ID), input);
}

public func EditOffer_Closed()
{
  PlayerMessage(GetOwner(), "", this());
}


private func Edit_AddMinus()
{
input = -(input ?? 0);
}

private func Edit_AddNumber(par0, number)
{
  // Ziffer hinzufügen
//  if (input >= 1000) return(1);   // höchstens vierstellig
    if ((input ?? 0) >= 10000) return(1);   // höchstens fünfstellig
  input = (input ?? 0)*10 + (number ?? 0);
  // Ausgabe
  PlayerMessage(GetOwner(), "@<c c0c0c0>Wert der Ware:  %d Gold|<c ffffff>Eigener Preis:  %d Gold", this(), GetValue(nil,inputID), input);
  return(1);
}

private func Edit_DeleteNumber()
{
  // Ziffer hinzufügen
  input = (input ?? 0) / 10;
  // Ausgabe
  PlayerMessage(GetOwner(), "@<c c0c0c0>Wert der Ware:  %d Gold|<c ffffff>Eigener Preis:  %d Gold", this(), GetValue(nil,inputID), input);
  return(1);
}

private func Edit_DeleteOffer(par0, warenum)
{
  // bisheriges Angebot löschen, indem Preis auf 0 gesetzt wird
  Local(warenum, FindObjectOwner(STPO, GetOwner())) = nil;
  // zur Angebotsliste wechseln
  return(SellList());
}

private func Edit_ChangeOffer(par0, warenum)
{
  // bisheriges Angebot ändern bzw. überhaupt erstellen
  var offer = FindObjectOwner(STPO, GetOwner());
  Local(warenum, offer) = input;
  // zur Angebotsliste wechseln
  return(SellList(nil,warenum));
}


/* Überweisung */

private func MoneyXfer(ID)
{
  // Betrag bestimmen
  input = nil;
  // neues Menü
  CreateMenu(ID, caller, nil,nil, GetName(nil,ID), nil,nil,1);
  var mec = CreateObject(SMEC);                // Menü-Überwachung; hier nur benötigt, um Preisanzeige am Ende zu löschen
  mec->Init(caller,this(), "MoneyXfer", 1);    // removeothers = 1, damit PlayerMessage (siehe unten) nicht gelöscht wird
  // Ziffern
  var i;
  for (i = nil; (i ?? 0) <= 9; S42MigrationStep(i, 1, true))
    AddMenuItem(Format("Hinzufügen: %d",i), "Xfer_AddNumber", SYNU, caller, nil,i,nil, 2,i);
  // Steuerbefehle
  AddMenuItem("Ziffer löschen", "Xfer_DeleteNumber", SYBA, caller);
  AddMenuItem("Fertig", "Xfer_ChoosePlayer", SYAC, caller);
  // Menüauswahl setzen
  SelectMenuItem(11, caller);
  // eingegebener Wert als Message
  PlayerMessage(GetOwner(), "@Geldbetrag:  %d Gold", this(), input);
}

public func MoneyXfer_Closed()
{
  PlayerMessage(GetOwner(), "", this());
}

private func Xfer_AddNumber(par0, number)
{
  // eine Ziffer hinzufügen
  if ((input ?? 0) >= 1000) return(1);   // höchstens vierstellig
  input = (input ?? 0)*10 + (number ?? 0);
  // Ausgabe
  PlayerMessage(GetOwner(), "Geldbetrag:  %d Gold", this(), input);
}

private func Xfer_DeleteNumber()
{
  // eine Ziffer zurücknehmen
  input = (input ?? 0) / 10;
  // Ausgabe
  PlayerMessage(GetOwner(), "Geldbetrag:  %d", this(), input);
}

private func Xfer_ChoosePlayer()
{
  // neues Menü
  CreateMenu(SGMI,caller, nil,nil,"Überweisen an...",nil,1);
  // alle Spieler durchgehen
  var i, player;
  var j = GetPlayerCount();
  // erstmal alle Spieler durchgehen
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
  {
    player = GetPlayerByIndex(i);
    if (Equal(player, GetOwner())) continue;
    AddMenuItem(Format("%s",GetPlayerName(player)), "Xfer_DoTransfer", SYAC,caller,nil,player);
  }
}

private func Xfer_DoTransfer(par0, player)
{
  // überhaupt so viel Geld da?
  if ((input ?? 0) > (GetWealth(GetOwner()) ?? 0))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", this());
    return(1);
  }
  // Geldüberweisung
  SetWealth(GetOwner(), (GetWealth(GetOwner()) ?? 0)-(input ?? 0));
  SetWealth(player, (GetWealth(player) ?? 0)+(input ?? 0));
  // Sound & Meldungen
  Sound("Cash");
  PlayerMessage(GetOwner(), "%d Gold an %s überwiesen", nil,input, GetPlayerName(player));
  PlayerMessage(player, "%d Gold von %s erhalten", nil,input, GetPlayerName(GetOwner()));
}



/*- Hilfsfunktionen -*/

private func GetWareID(warenum)
{
  // gibt die richtige ID einer bestimmten Warennummer zurück
  return(Local(warenum, FindObject(STWL)));
}

private func GetWarePrice(warenum, player)
{
  // findet den Warenpreis des entsprechenden Spielers bzw. 0, wenn kein Angebot
  if (Equal(player, -1))
  {
    var ait = FindObject(SAIT);
    if (!ait) return(nil);
    return(ait->GetPrice(warenum));
  }
  var offerlist = FindObjectOwner(STPO, player);
  if (!offerlist) return(nil);
  // Angebot gefunden
  return(Local(warenum, offerlist));
}

private func OffersWare(warenum, player)
{
  // bietet der Spieler eine bestimmte Ware an?
  return(GetWarePrice(warenum, player));
}

private func GetCheapestPrice(warenum, doignore, player2ignore)
{
  // billigstes verfügbares Angebot ermitteln
  var cheapest = 10000;   // dieser Preis wird garantiert nicht überboten
  var price, player;
  var i = -1;
  var j = GetPlayerCount();
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
  {
    if ((i ?? 0) >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (And(doignore, (Equal(player, player2ignore)))) continue;
    if (!OffersWare(warenum, player))          continue;
    if (!GetAvailableAmount(warenum, player))  continue;
    price = GetWarePrice(warenum, player);
    if ((price ?? 0) < (cheapest ?? 0))
      cheapest = price;
  }
  return(cheapest);
}

private func GetCheapestPlayer(warenum)
{
  // Spieler mit billigstem Angebot ermitteln
  var cheapest = GetCheapestPrice(warenum);
  if (Equal(cheapest, 10000))
    return(-2);   // keiner bietet das an
  var player, cplayer, splayer;        // splayer = Zahl der Spieler mit billigstem Preis
  var i = -1;
  var j = GetPlayerCount();
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
  {
    if ((i ?? 0) >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (!OffersWare(warenum, player))             continue;
    if (!GetAvailableAmount(warenum, player))     continue;
    if ((GetWarePrice(warenum, player) ?? 0) > (cheapest ?? 0)) continue;
    // mehrere gleiche
    S42MigrationStep(splayer, 1);
    if (!Random(splayer))              // bei mehreren Spielern mit gleichen Preisen entscheidet der Zufall
      cplayer = player;
  }
  return(cplayer);
}

private func GetAvailableAmount(warenum, player)
{
  // KI-Händler
  if (Equal(player, -1))
  {
    var ait = FindObject(SAIT);
    if (!ait) return(nil);
    return(Local(warenum, ait));
  }
  var ID = GetWareID(warenum);
  // alle Handelsbasen abklappern
  var count, uplink;
  while (uplink = FindObjectOwner(UPLT, player, nil,nil,nil,nil,nil,"Turn",nil, uplink))
    S42MigrationAssign(count, "+=", (ContentsCount(ID, GetActionTarget(nil,uplink)) ?? 0));
  // der Uplink darf sich selbst nicht vergessen
  if (Equal(player, GetOwner()))
    S42MigrationAssign(count, "+=", (ContentsCount(ID, GetActionTarget()) ?? 0));
  return(count);
}

private func AllAvailableAmount(warenum, IgnoreOwner)
{
  // liefert die Gesamtmenge einer Ware aller Spieler, inkl. des KI-Händlers
  var count, player;
  var i = -1;
  var j = GetPlayerCount();
  // erstmal alle Spieler durchgehen
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
  {
    player = GetPlayerByIndex(i);
    if (And(IgnoreOwner, (Equal(player, GetOwner())))) continue;
    if (!OffersWare(warenum, player))          continue;
    S42MigrationAssign(count, "+=", (GetAvailableAmount(warenum, player) ?? 0));
  }
  return(count);
}

private func FindWare(warenum, player)
{
  // kein Angebot dieses Spielers da?
  if (!GetWarePrice(warenum,player)) return(nil);
  var ID = GetWareID(warenum);
  // alle Handelsbasen abklappern
  var uplink, ware;
  while (uplink = FindObjectOwner(UPLT, player, nil,nil,nil,nil,nil,"Turn",nil, uplink))
    if (ware = FindContents(ID, GetActionTarget(nil,uplink)))
      return(ware);
  return(nil);
}

private func SellNothing(par0, selection)
{
  Sound("Error");
  return(nil);
}


