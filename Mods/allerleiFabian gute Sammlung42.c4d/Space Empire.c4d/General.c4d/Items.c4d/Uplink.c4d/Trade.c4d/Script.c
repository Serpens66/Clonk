/*-- Uplink (Handelssystem) --*/

#strict
#include UPL0

local input, inputID;


/* Animation starten */

public func Init(target)
{
  inherited(target);
  // Nachricht
  PlayerMessage(GetOwner(), "Ausgerichtet auf|Handelssystem", this());
  Local(0) = 1;
}


/*- Handelssytem -*/

public func KeyUp(par_caller)
{
  if (inherited(par_caller))
    BuyObjects(1,0);
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
  CreateMenu(GetID(GetActionTarget()), caller, 0,0,0,0,0,1);
  var mec = CreateObject(SMEC);   // Menü-Überwachung
  mec->Init(caller,this(), "BuyObjects",1);
  // Übersicht über Kategorien
  AddMenuItem("Kategorie: Materialien", "BuyChangeCat", SYT0,caller, 0,1, "Materialien und Rohstoffe kaufen.",  2, 0 + (category != 1) * 5);
  AddMenuItem("Kategorie: Gegenstände", "BuyChangeCat", SYT0,caller, 0,2, "Grundlegende Gegenstände kaufen.",   2, 1 + (category != 2) * 5);
  AddMenuItem("Kategorie: Ausrüstung",  "BuyChangeCat", SYT0,caller, 0,3, "Ausrüstung und Munition kaufen.",    2, 2 + (category != 3) * 5);
  AddMenuItem("Kategorie: Waffen",      "BuyChangeCat", SYT0,caller, 0,4, "Tragbare Waffen kaufen.",            2, 3 + (category != 4) * 5);
  AddMenuItem("Kategorie: Fahrzeuge",   "BuyChangeCat", SYT0,caller, 0,5, "Fahrzeuge und großes Gerät kaufen.", 2, 4 + (category != 5) * 5);
  var i = 6;
  while (--i)
    mec->AddInfo(-1);
  // richtige Kategorie anwählen
  var warelist = FindObject(STWL);
  var start = warelist->CategoryStart(category);
  var size = warelist->CategorySize(category);
  // alle IDs durchgehen
  var warenum, ID;
  for (i = 0; i < size; ++i)
  {
    warenum = start+i;
    ID = GetWareID(warenum);               // Ware wird zugelassen:
    if ((category <= 2) ||                 //  - automatisch in den ersten beiden Kategorien
        GetPlrKnowledge(GetOwner(),ID))    //  - oder wenn der Bauplan erforschbar ist
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
  if (warenum == -1)
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
    if (amount && (price == cheapest))
    {
      PlayerMessage(GetOwner(), "@%s|Wert: %d Gold|<c 80ff80>Eigenes Angebot: %d Stück für je %d Gold", this(),
                    GetName(0,GetWareID(warenum)), GetValue(0,GetWareID(warenum)), amount, price);
      return(1);
    }
  }
  // andere Spieler der Reihe nach durchgehen
  var player, cplayer, splayer;
  var allamount;
  var i = -1;
  var j = GetPlayerCount();
  for (i; i < j; ++i)
  {
    if (i >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (player == GetOwner())         continue;
    if (!OffersWare(warenum, player)) continue;
    // Angebotsmenge und Preis
    amount = GetAvailableAmount(warenum, player);
    price = GetWarePrice(warenum, player);
    if (price == cheapest)
    {
      ++splayer;
      cplayer = player;
      allamount += amount;
    }
  }
  // Infos ausgeben
  var infostring;
  if (splayer == 0) infostring = "<c c0c0c0>- keine Angebote verfügbar -";
  if (splayer == 1)
    if (cplayer == -1) infostring = Format("KI-Händler: %d Stück für je %d Gold", allamount, cheapest);
                  else infostring = Format("%s: %d Stück für je %d Gold", GetPlayerName(cplayer), allamount, cheapest);
  if (splayer >= 2) infostring = Format("%d Spieler: %d Stück für je %d Gold", splayer, allamount, cheapest);
  // Info ausgeben
  PlayerMessage(GetOwner(), "@%s|Wert: %d Gold|%s", this(), GetName(0,GetWareID(warenum)), GetValue(0,GetWareID(warenum)), infostring);
}

public func BuyObjects_Closed()
{
  PlayerMessage(GetOwner(), "", this());
}


/* Kategorie wechseln */

private func BuyChangeCat(par0, category)
{
  Sound("Click");
  BuyObjects(category, category-1);
}

/* Ware gekauft */

private func BuyWare(ID, warenum)
{
  // erstmal den billigsten Spieler finden
  var player = GetCheapestPlayer(warenum);
  // keiner bietet die Ware an?
  if (player == -2)
    return(BuyWareReturn(warenum, 0));
  // eigenes Angebot das billigste?
  if (OffersWare(warenum, GetOwner()))
    if (GetAvailableAmount(warenum, GetOwner()))                                // nur wenn man selbst nicht ausverkauft ist
      if (GetWarePrice(warenum, player) == GetWarePrice(warenum, GetOwner()))   // man selbst bietet auch am billigsten an?
        return(BuyWareReturn(warenum, 0));
  // reicht das Geld?
  var price = GetWarePrice(warenum, player);
  if (GetWealth(GetOwner()) < price)
    return(BuyWareReturn(warenum, 0));
  // Kauf bei KI-Spieler
  if (player == -1)
  {
    // Ware erzeugen
    var ware = CreateContents(ID, GetActionTarget());
    --Local(warenum, FindObject(SAIT));
    // Geld überweisen
    DoWealth(GetOwner(), -price);
    // zurück zum Menü
    return(BuyWareReturn(warenum, 1));
  }
  // Ware finden und rüberbeamen
  var ware = FindWare(warenum, player);
  SetOwner(GetOwner(), ware);
  Enter(GetActionTarget(), ware);
  // Geld überweisen
  DoWealth(GetOwner(), -price);
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
  CreateMenu(GetID(GetActionTarget()),caller, 0,0, "Handelsmenü");
  AddMenuItem("Verkaufsliste", "SellList",  SYT2,caller, 0,0, "Auflistung der eigenen Angebote.");
  AddMenuItem("Neues Angebot", "SellOffer", SYT1,caller, 0,0, "Neues Verkaufsangebot erstellen.");
  var i = 2;
  if (GetPlayerCount() > 1)
  {
    AddMenuItem("Überweisung", "MoneyXfer", SYT3,caller, 0,0, "Geld an anderen Spieler überweisen.");
    ++i;
  }
  // mit ein paar leeren Einträge auffüllen
  while (i % 5)
  {
    AddMenuItem("---", "SellNothing", SPCR, caller, 0,i);
    ++i;
  }
  // zusätzliche Einträge
  AddMenuItem("Ausrichtung wechseln: Basis",  "ChangeMode", SYUP,caller, 0,0, "Das Heimatbasissystem wird gewählt. Hat alle Funktionen einer normalen Basis.",   2,2);
if(GetPlrKnowledge(GetOwner(),UUU1))
  AddMenuItem("Transfer",            "TransferMenu", SYUP,caller, 0,0, "Transfer zu anderen Basen und Fahrzeugen mit Uplink. Reichweite: 500Meter.", 2,4);
  AddMenuItem("Uplink abnehmen", "AttachTargetLost", SYUP,caller, 0,0, "Uplink deaktivieren und abnehmen.", 2,3);
  if (building->~HasWeaponSelection())
  {
    AddMenuItem("Waffenwechsel",    "SelectWeapons", SYUP,caller, 0,0, "Bewaffnung der Basis austauschen.", 2,5);
    ++i;
  }
if(freigegeben==0)
  AddMenuItem("System freigeben", "Freigabe", SYUP,caller, 0,caller, "Uplinksystem wird für alle NichtFeinde freigegeben. Diese können sowohl im Gebäude auf alles zugreifen, als auch aus ihrem eigenen Uplinksystem Dinge aus deinem System per Beam holen oder dir senden. Außerdem werden sie auch geheilt und gelöscht, was sonst nicht der Fall ist", 2,2);
if(freigegeben==1)
  AddMenuItem("System sperren", "Freigabe", SYUP,caller, 0,caller, "Uplinksystem wird wieder gesperrt, sodass nur noch der Besitzer Zugriff hat.", 2,2);
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
  CreateMenu(SYT2, caller, 0,0, "Verkaufsliste", 0,1);
  var mec = CreateObject(SMEC);         // Menü-Überwachung
  mec->Init(caller,this(), "SellList");
  // alle IDs durchgehen
  var selection;
  var i, j;
  for (i = 0; i < size; ++i)
    if (OffersWare(i, GetOwner()))
    {
      if (i == selnum)  selection = j;
      ++j;
      // Menüeintrag hinzufügen
      AddMenuItem("%s", "EditOffer", GetWareID(i), caller, 0, i);
      mec->AddInfo(i);
    }
  AddMenuItem("Neu...", "SellOffer", SYT1,caller, 0,0, "Neues Verkaufsangebot erstellen.");
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
  if (warenum == -1)
    return(PlayerMessage(GetOwner(), "", this()));
  // billigsten Preis ermitteln
  var cheapest = GetCheapestPrice(warenum, 1, GetOwner());   // Besitzer dabei ignorieren
  var price = GetWarePrice(warenum, GetOwner());
  var amount, allamount;
  // andere Spieler der Reihe nach durchgehen
  var player, cplayer, splayer;
  var i = -1;
  var j = GetPlayerCount();
  for (i; i < j; ++i)
  {
    if (i >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (player == GetOwner()) continue;
    if (!OffersWare(warenum, player)) continue;
    // Angebotsmenge und Preis
    amount = GetAvailableAmount(warenum, player);
    price = GetWarePrice(warenum, player);
    if (price == cheapest)
    {
      ++splayer;
      cplayer = player;
      allamount += amount;
    }
  }
  // Infos ausgeben
  var infostring;
  if (splayer == 0) infostring = "<c c0c0c0>- kein anderes Angebot -";
  if (splayer == 1)
    if (cplayer == -1) infostring = Format("KI-Händler: %d Stück für je %d Gold", allamount, cheapest);
                  else infostring = Format("%s: %d Stück für je %d Gold", GetPlayerName(cplayer), allamount, cheapest);
  if (splayer >= 2) infostring = Format("%d Spieler: %d Stück für je %d Gold", splayer, allamount, cheapest);
  // Einfärbung je nachdem, ob Angebot gültig
  amount = GetAvailableAmount(warenum, GetOwner());
  if (!amount) infostring = Format("<c ff8080>Eigenes Angebot: 0 Stück für je %d Gold|<c ffffff>%s", GetWarePrice(warenum, GetOwner()), infostring);
          else infostring = Format("<c 80ff80>Eigenes Angebot: %d Stück für je %d Gold|<c ffffff>%s", amount, GetWarePrice(warenum, GetOwner()), infostring);
  // Info ausgeben
  PlayerMessage(GetOwner(), "@%s|Wert: %d Gold|%s", this(), GetName(0,GetWareID(warenum)), GetValue(0,GetWareID(warenum)), infostring);
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
  for (i = 0; i < size; ++i)
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
  CreateMenu(ID, caller, 0,0, GetName(0,ID), 0,0,1);
  var mec = CreateObject(SMEC);                // Menü-Überwachung; hier nur benötigt, um Preisanzeige am Ende zu löschen
  mec->Init(caller,this(), "EditOffer", 1);    // removeothers = 1, damit PlayerMessage (siehe unten) nicht gelöscht wird
  // Ziffern
  var i;
  for (i = 0; i <= 9; i++)
    AddMenuItem(Format("Hinzufügen: %d",i), "Edit_AddNumber", SYNU, caller, 0,i,0, 2,i);

//minuszeichen
    AddMenuItem("Negativ machen", "Edit_AddMinus", PCSO, caller);

  // Steuerbefehle
  if (price)
    AddMenuItem("Altes Angebot löschen", "Edit_DeleteOffer", SYDL, caller, 0,warenum);
  AddMenuItem("Ziffer löschen", "Edit_DeleteNumber", SYBA, caller);
  AddMenuItem("Fertig", "Edit_ChangeOffer", SYAC, caller, 0,warenum);
  // Menüauswahl setzen
  if (price) SelectMenuItem(12, caller);
        else SelectMenuItem(11, caller);
  // eingegebener Wert als Message
  PlayerMessage(GetOwner(), "@<c c0c0c0>Wert der Ware:  %d Gold|<c ffffff>Eigener Preis:  %d Gold", this(), GetValue(0,ID), input);
}

public func EditOffer_Closed()
{
  PlayerMessage(GetOwner(), "", this());
}


private func Edit_AddMinus()
{
input = -input;
}

private func Edit_AddNumber(par0, number)
{
  // Ziffer hinzufügen
//  if (input >= 1000) return(1);   // höchstens vierstellig
    if (input >= 10000) return(1);   // höchstens fünfstellig
  input = input*10 + number;
  // Ausgabe
  PlayerMessage(GetOwner(), "@<c c0c0c0>Wert der Ware:  %d Gold|<c ffffff>Eigener Preis:  %d Gold", this(), GetValue(0,inputID), input);
  return(1);
}

private func Edit_DeleteNumber()
{
  // Ziffer hinzufügen
  input = input / 10;
  // Ausgabe
  PlayerMessage(GetOwner(), "@<c c0c0c0>Wert der Ware:  %d Gold|<c ffffff>Eigener Preis:  %d Gold", this(), GetValue(0,inputID), input);
  return(1);
}

private func Edit_DeleteOffer(par0, warenum)
{
  // bisheriges Angebot löschen, indem Preis auf 0 gesetzt wird
  Local(warenum, FindObjectOwner(STPO, GetOwner())) = 0;
  // zur Angebotsliste wechseln
  return(SellList());
}

private func Edit_ChangeOffer(par0, warenum)
{
  // bisheriges Angebot ändern bzw. überhaupt erstellen
  var offer = FindObjectOwner(STPO, GetOwner());
  Local(warenum, offer) = input;
  // zur Angebotsliste wechseln
  return(SellList(0,warenum));
}


/* Überweisung */

private func MoneyXfer(ID)
{
  // Betrag bestimmen
  input = 0;
  // neues Menü
  CreateMenu(ID, caller, 0,0, GetName(0,ID), 0,0,1);
  var mec = CreateObject(SMEC);                // Menü-Überwachung; hier nur benötigt, um Preisanzeige am Ende zu löschen
  mec->Init(caller,this(), "MoneyXfer", 1);    // removeothers = 1, damit PlayerMessage (siehe unten) nicht gelöscht wird
  // Ziffern
  var i;
  for (i = 0; i <= 9; i++)
    AddMenuItem(Format("Hinzufügen: %d",i), "Xfer_AddNumber", SYNU, caller, 0,i,0, 2,i);
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
  if (input >= 1000) return(1);   // höchstens vierstellig
  input = input*10 + number;
  // Ausgabe
  PlayerMessage(GetOwner(), "Geldbetrag:  %d Gold", this(), input);
}

private func Xfer_DeleteNumber()
{
  // eine Ziffer zurücknehmen
  input = input / 10;
  // Ausgabe
  PlayerMessage(GetOwner(), "Geldbetrag:  %d", this(), input);
}

private func Xfer_ChoosePlayer()
{
  // neues Menü
  CreateMenu(SGMI,caller, 0,0,"Überweisen an...",0,1);
  // alle Spieler durchgehen
  var i, player;
  var j = GetPlayerCount();
  // erstmal alle Spieler durchgehen
  for (i; i < j; ++i)
  {
    player = GetPlayerByIndex(i);
    if (player == GetOwner()) continue;
    AddMenuItem(Format("%s",GetPlayerName(player)), "Xfer_DoTransfer", SYAC,caller,0,player);
  }
}

private func Xfer_DoTransfer(par0, player)
{
  // überhaupt so viel Geld da?
  if (input > GetWealth(GetOwner()))
  {
    PlayerMessage(GetOwner(), "Nicht genug Geld!", this());
    return(1);
  }
  // Geldüberweisung
  SetWealth(GetOwner(), GetWealth(GetOwner())-input);
  SetWealth(player, GetWealth(player)+input);
  // Sound & Meldungen
  Sound("Cash");
  PlayerMessage(GetOwner(), "%d Gold an %s überwiesen", 0,input, GetPlayerName(player));
  PlayerMessage(player, "%d Gold von %s erhalten", 0,input, GetPlayerName(GetOwner()));
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
  if (player == -1)
  {
    var ait = FindObject(SAIT);
    if (!ait) return(0);
    return(ait->GetPrice(warenum));
  }
  var offerlist = FindObjectOwner(STPO, player);
  if (!offerlist) return(0);
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
  for (i; i < j; ++i)
  {
    if (i >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (doignore && (player == player2ignore)) continue;
    if (!OffersWare(warenum, player))          continue;
    if (!GetAvailableAmount(warenum, player))  continue;
    price = GetWarePrice(warenum, player);
    if (price < cheapest)
      cheapest = price;
  }
  return(cheapest);
}

private func GetCheapestPlayer(warenum)
{
  // Spieler mit billigstem Angebot ermitteln
  var cheapest = GetCheapestPrice(warenum);
  if (cheapest == 10000)
    return(-2);   // keiner bietet das an
  var player, cplayer, splayer;        // splayer = Zahl der Spieler mit billigstem Preis
  var i = -1;
  var j = GetPlayerCount();
  for (i; i < j; ++i)
  {
    if (i >= 0) player = GetPlayerByIndex(i);
           else player = -1;
    if (!OffersWare(warenum, player))             continue;
    if (!GetAvailableAmount(warenum, player))     continue;
    if (GetWarePrice(warenum, player) > cheapest) continue;
    // mehrere gleiche
    ++splayer;
    if (!Random(splayer))              // bei mehreren Spielern mit gleichen Preisen entscheidet der Zufall
      cplayer = player;
  }
  return(cplayer);
}

private func GetAvailableAmount(warenum, player)
{
  // KI-Händler
  if (player == -1)
  {
    var ait = FindObject(SAIT);
    if (!ait) return(0);
    return(Local(warenum, ait));
  }
  var ID = GetWareID(warenum);
  // alle Handelsbasen abklappern
  var count, uplink;
  while (uplink = FindObjectOwner(UPLT, player, 0,0,0,0,0,"Turn",0, uplink))
    count += ContentsCount(ID, GetActionTarget(0,uplink));
  // der Uplink darf sich selbst nicht vergessen
  if (player == GetOwner())
    count += ContentsCount(ID, GetActionTarget());
  return(count);
}

private func AllAvailableAmount(warenum, IgnoreOwner)
{
  // liefert die Gesamtmenge einer Ware aller Spieler, inkl. des KI-Händlers
  var count, player;
  var i = -1;
  var j = GetPlayerCount();
  // erstmal alle Spieler durchgehen
  for (i; i < j; ++i)
  {
    player = GetPlayerByIndex(i);
    if (IgnoreOwner && (player == GetOwner())) continue;
    if (!OffersWare(warenum, player))          continue;
    count += GetAvailableAmount(warenum, player);
  }
  return(count);
}

private func FindWare(warenum, player)
{
  // kein Angebot dieses Spielers da?
  if (!GetWarePrice(warenum,player)) return(0);
  var ID = GetWareID(warenum);
  // alle Handelsbasen abklappern
  var uplink, ware;
  while (uplink = FindObjectOwner(UPLT, player, 0,0,0,0,0,"Turn",0, uplink))
    if (ware = FindContents(ID, GetActionTarget(0,uplink)))
      return(ware);
  return(0);
}

private func SellNothing(par0, selection)
{
  Sound("Error");
  return(0);
}


