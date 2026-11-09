/*-- KI-Händler --*/

#strict 3

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
  SetPosition(nil,nil);
}

public func AddOne()
{
  // Anzahl hochzählen
  S42MigrationStep(count, 1);
  if ((count ?? 0) > 5) count = 5;
  // mbase neu berechnen
  if (Equal(count, 2))  mbase = 50;
  if (Equal(count, 3))  mbase = 35;
  if (Equal(count, 4))  mbase = 25;
  if (Equal(count, 5))  mbase = 15;
}


/* Timer-Call */

protected func DoTrade()
{
  // Kauf bei Spielern
  var warelist = FindObject(STWL);
  if (!warelist)
    warelist = CreateObject(STWL,nil,nil,-1);
  var listsize = warelist->GetListSize();
  var warenum = Random(listsize); 
  // spezielle Funktion kümmert sich da schon drum
  while (BuyPlayerOffers(warenum));
  // nein, hier fehlt nichts und obige Zeile ist völlig korrekt
  
  // Angebot ausgleichen
  warenum = Random(listsize);
  var mvalue = GetMValue(warenum);
  // zufälliger Wert, der addiert oder abgezogen wird
  var c = ((Random((mvalue ?? 0)/3) ?? 0) + 50) / 100;
  c = Max(c, 1);     // mindestens 1
  // addieren oder subtrahieren?
  if ((Random(mvalue) ?? 0) < (Local(warenum) ?? 0)*100) S42MigrationAssign(Local(warenum), "-=", (c ?? 0));
                                      else S42MigrationAssign(Local(warenum), "+=", (c ?? 0));
  // Angebotsmenge zwischen Null und MValue (gerundet)
  Local(warenum) = BoundBy(Local(warenum), nil, ((mvalue ?? 0)+50)/100);
}

private func BuyPlayerOffers(warenum)
{
  // solange kaufen, wie es sich für den KI-Händler lohnt
  var player = GetCheapestPlayer(warenum);
  if (Equal(player, -1))
    return(nil);
  var offer = FindObjectOwner(STPO, player);
  var value = GetValue(nil, Local(warenum, FindObject(STWL)));
  var price, ware, bool;
  while (1)
  {
    // neuer eigener Preis nach dem Kauf
    price = (Local(warenum) ?? 0) * (Sqrt(10000 + (value ?? 0)*10000) ?? 0) / (15*(count ?? 0));
    price = (value ?? 0) * (200 - (price ?? 0)) / 100;                                  // Berechnung siehe unten (GetPrice)
    if (!price)
      price = 1;
    // Kaufpreis darf nicht größer als der neuer Preis sein
    if ((Local(warenum, offer) ?? 0) > (price ?? 0))
    {
      if (bool) CreateObject(SE08,nil,nil,player);
      return(nil);
    }
    // Ware beim Spieler finden
    ware = FindWare(warenum, player);
    if (!ware)
    {
      if (bool) CreateObject(SE08,nil,nil,player);
      return(1);
    }
    // Geschäft abwickeln
    PlayerMessage(player, "%s|an KI-Spieler verkauft", Contained(ware), GetName(ware));
    bool = 1;
    RemoveObject(ware);
    S42MigrationStep(Local(warenum), 1);
    DoWealth(player, Local(warenum, offer));
    Sound("Cash");
  }
  return(nil);
}


/* Hilfsfunktionen & Aufrufe */

private func GetAvailableAmount(warenum, player)
{
  // alle Handelsbasen abklappern
  var ID = Local(warenum, FindObject(STWL));
  var count, uplink;
  while (uplink = FindObjectOwner(UPLT, player, nil,nil,nil,nil,nil,"Turn",nil, uplink))
    S42MigrationAssign(count, "+=", (ContentsCount(ID, GetActionTarget(nil,uplink)) ?? 0));
  return(count);
}

private func FindWare(warenum, player)
{
  // alle Handelsbasen abklappern
  var ID = Local(warenum, FindObject(STWL));
  var uplink, ware;
  while (uplink = FindObjectOwner(UPLT, player, nil,nil,nil,nil,nil,"Turn",nil, uplink))
    if (ware = FindContents(ID, GetActionTarget(nil,uplink)))
      return(ware);
  return(nil);
}

private func GetCheapestPlayer(warenum)
{
  // alle Spieler überprüfen, um billigsten zu finden
  var player, cplayer, splayer;   // splayer = Zahl der Spieler mit billigstem Preis
  var offer, price;
  var cheapest = 10000;           // dieser Preis wird garantiert unterboten
  var i = -1;
  var j = GetPlayerCount();
  for (i; (i ?? 0) < (j ?? 0); S42MigrationStep(i, 1))
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
    if ((price ?? 0) > (cheapest ?? 0)) continue;
    if ((price ?? 0) < (cheapest ?? 0))
    {
      splayer = 1;
      cheapest = price;
      cplayer = player;
      continue;
    }
    S42MigrationStep(splayer, 1);
    if (!Random(splayer))   // bei mehreren Spielern mit gleichen Preisen entscheidet der Zufall
      cplayer = player;
  }
  // überhaupt kein Angebot vorhanden?
  if (Equal(cheapest, 10000))
    return(-1);
  return(cplayer);
}

private func GetMValue(num)
{
  // Richtwert für bestimmte Ware ermitteln (100fache Genauigkeit)
  var ID = Local(num, FindObject(STWL));
 // return(mbase*10000 / Sqrt(10000 + GetValue(0,ID)*10000));              // mvalue = mbase / Sqrt(1 + value)
  return(15*10000 / (Sqrt(10000 + (GetValue(nil,ID) ?? 0)*10000) ?? 0));
}

public func GetPrice(num)
{
  // gibt den Preis für eine Ware zurück
  if (!Local(num)) return(nil);     // kein Angebot
  var ID = Local(num, FindObject(STWL));
  var n = (Local(num) ?? 0) * (Sqrt(10000 + (GetValue(nil,ID) ?? 0)*10000) ?? 0) / (mbase ?? 0);       // n = amount * Sqrt(1 + value) / mbase
  var price = (GetValue(nil,ID) ?? 0) * (200 - (n ?? 0)) / 100;                          // price = value * (2 - n)
  if (!price) return(1);
  return(price);
}

