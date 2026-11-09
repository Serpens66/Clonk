/*-- KI-Händler --*/

#strict

local count;     // Zahl der gewählten KI-Händler
local mbase;     // Faktor zur Berechnung von Mengen-Richtwerten
                 // die nummerierten Locals enthalten die Angebotsmenge

/* Initialisierung */

protected func Initialize()
{
  // KI-Händler zählen
  var ait = FindObject(SAIT);
  if (ait)
  {
    ait->AddOne();
    return(RemoveObject());
  }
  count = 1;
  mbase = 75;
  SetPosition(0,0);
}

public func AddOne()
{
  // Anzahl hochzählen
  ++count;
  if (count > 5) count = 5;
  // mbase neu berechnen
  if (count==2)  mbase = 50;
  if (count==3)  mbase = 35;
  if (count==4)  mbase = 25;
  if (count==5)  mbase = 15;
}


/* Timer-Call */

protected func DoTrade()
{
  // Kauf bei Spielern
  var warelist = FindObject(STWL);
  if (!warelist)
    warelist = CreateObject(STWL,0,0,-1);
  var listsize = warelist->GetListSize();
  var warenum = Random(listsize); 
  // spezielle Funktion kümmert sich da schon drum
  while (BuyPlayerOffers(warenum));
  // nein, hier fehlt nichts und obige Zeile ist völlig korrekt
  
  // Angebot ausgleichen
  warenum = Random(listsize);
  var mvalue = GetMValue(warenum);
  // zufälliger Wert, der addiert oder abgezogen wird
  var c = (Random(mvalue/3) + 50) / 100;
  c = Max(c, 1);     // mindestens 1
  // addieren oder subtrahieren?
  if (Random(mvalue) < Local(warenum)*100) Local(warenum) -= c;
                                      else Local(warenum) += c;
  // Angebotsmenge zwischen Null und MValue (gerundet)
  Local(warenum) = BoundBy(Local(warenum), 0, (mvalue+50)/100);
}

private func BuyPlayerOffers(warenum)
{
  // solange kaufen, wie es sich für den KI-Händler lohnt
  var player = GetCheapestPlayer(warenum);
  if (player == -1)
    return(0);
  var offer = FindObjectOwner(STPO, player);
  var value = GetValue(0, Local(warenum, FindObject(STWL)));
  var price, ware, bool;
  while (1)
  {
    // neuer eigener Preis nach dem Kauf
    price = Local(warenum) * Sqrt(10000 + value*10000) / (15*count);
    price = value * (200 - price) / 100;                                  // Berechnung siehe unten (GetPrice)
    if (!price)
      price = 1;
    // Kaufpreis darf nicht größer als der neuer Preis sein
    if (Local(warenum, offer) > price)
    {
      if (bool) CreateObject(SE08,0,0,player);
      return(0);
    }
    // Ware beim Spieler finden
    ware = FindWare(warenum, player);
    if (!ware)
    {
      if (bool) CreateObject(SE08,0,0,player);
      return(1);
    }
    // Geschäft abwickeln
    PlayerMessage(player, "%s|an KI-Spieler verkauft", Contained(ware), GetName(ware));
    bool = 1;
    RemoveObject(ware);
    ++Local(warenum);
    DoWealth(player, Local(warenum, offer));
    Sound("Cash");
  }
  return(0);
}


/* Hilfsfunktionen & Aufrufe */

private func GetAvailableAmount(warenum, player)
{
  // alle Handelsbasen abklappern
  var ID = Local(warenum, FindObject(STWL));
  var count, uplink;
  while (uplink = FindObjectOwner(UPLT, player, 0,0,0,0,0,"Turn",0, uplink))
    count += ContentsCount(ID, GetActionTarget(0,uplink));
  return(count);
}

private func FindWare(warenum, player)
{
  // alle Handelsbasen abklappern
  var ID = Local(warenum, FindObject(STWL));
  var uplink, ware;
  while (uplink = FindObjectOwner(UPLT, player, 0,0,0,0,0,"Turn",0, uplink))
    if (ware = FindContents(ID, GetActionTarget(0,uplink)))
      return(ware);
  return(0);
}

private func GetCheapestPlayer(warenum)
{
  // alle Spieler überprüfen, um billigsten zu finden
  var player, cplayer, splayer;   // splayer = Zahl der Spieler mit billigstem Preis
  var offer, price;
  var cheapest = 10000;           // dieser Preis wird garantiert unterboten
  var i = -1;
  var j = GetPlayerCount();
  for (i; i < j; ++i)
  {
    player = GetPlayerByIndex(i);
    // bietet Spieler die Ware überhaupt an?
    offer = FindObjectOwner(STPO, player);
    if (!offer) continue;
    // Preis ermitteln
    price = Local(warenum, offer);
    if (!price) continue;
    // Spieler muss auch noch Ware übrig haben
    if (!GetAvailableAmount(warenum, player)) continue;
    // billigster Spieler?
    if (price > cheapest) continue;
    if (price < cheapest)
    {
      splayer = 1;
      cheapest = price;
      cplayer = player;
      continue;
    }
    ++splayer;
    if (!Random(splayer))   // bei mehreren Spielern mit gleichen Preisen entscheidet der Zufall
      cplayer = player;
  }
  // überhaupt kein Angebot vorhanden?
  if (cheapest == 10000)
    return(-1);
  return(cplayer);
}

private func GetMValue(num)
{
  // Richtwert für bestimmte Ware ermitteln (100fache Genauigkeit)
  var ID = Local(num, FindObject(STWL));
 // return(mbase*10000 / Sqrt(10000 + GetValue(0,ID)*10000));              // mvalue = mbase / Sqrt(1 + value)
  return(15*10000 / Sqrt(10000 + GetValue(0,ID)*10000));
}

public func GetPrice(num)
{
  // gibt den Preis für eine Ware zurück
  if (!Local(num)) return(0);     // kein Angebot
  var ID = Local(num, FindObject(STWL));
  var n = Local(num) * Sqrt(10000 + GetValue(0,ID)*10000) / mbase;       // n = amount * Sqrt(1 + value) / mbase
  var price = GetValue(0,ID) * (200 - n) / 100;                          // price = value * (2 - n)
  if (!price) return(1);
  return(price);
}

