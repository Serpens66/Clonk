/*-- Landepadpanel--*/

#strict 3

//tolle Waffenliste!
local aWare;
local aCount;
local dock;

func Initialize() {
  aWare = CreateArray();
  aCount = CreateArray();
  dock = CreateObject(LPF_,-90,18);
}


public func SortWare() {
  if(!Equal(GetLength(aWare), GetLength(aCount)))
    return(Log("Waffenautomat-PANICK. Den Devs melden!"));
  
  var length;
  while(Par(S42MigrationStep(length, 1, true)));
  
  var ware = CreateArray(), count = CreateArray();
  
  //Nach Kategorie sortieren
  for(var y = nil; (y ?? 0) < (length ?? 0); S42MigrationStep(y, 1, true)) {
    ware[(y ?? 0)] = CreateArray();
    count[(y ?? 0)] = CreateArray();
    for(var x = nil; (x ?? 0) < (GetLength(aWare) ?? 0); S42MigrationStep(x, 1, true)) {
      if(DefinitionCall(aWare[(x ?? 0)],Par(y))) {
        AddToArray(ware[(y ?? 0)],aWare[(x ?? 0)]);
        AddToArray(count[(y ?? 0)],aCount[(x ?? 0)]);
      }
    }
  }
  
  //Nach Preis sortieren
  for(var y = nil; (y ?? 0) < (length ?? 0); S42MigrationStep(y, 1, true)) {
    for(var x = nil; (x ?? 0) < (GetLength(ware[(y ?? 0)]) ?? 0); S42MigrationStep(x, 1, true)) {
      for(var z = (x ?? 0)+1; (z ?? 0) < (GetLength(ware[(y ?? 0)]) ?? 0); S42MigrationStep(z, 1, true)) {
        if((GetDefValue(ware[(y ?? 0)][(z ?? 0)]) ?? 0) < (GetDefValue(ware[(y ?? 0)][(x ?? 0)]) ?? 0)) {
          var tmp = ware[(y ?? 0)][(x ?? 0)];
          ware[(y ?? 0)][(x ?? 0)] = ware[(y ?? 0)][(z ?? 0)];
          ware[(y ?? 0)][(z ?? 0)] = tmp;
          tmp = count[(y ?? 0)][(x ?? 0)];
          count[(y ?? 0)][(x ?? 0)] = count[(y ?? 0)][(z ?? 0)];
          count[(y ?? 0)][(z ?? 0)] = tmp;
        }
      }
    }
  }
  
  //Wieder sortiert ins Array tun
  aWare = aCount = CreateArray();
  var cnt;
  for(var y = nil; (y ?? 0) < (length ?? 0); S42MigrationStep(y, 1, true)) {
    for(var x = nil; (x ?? 0) < (GetLength(ware[(y ?? 0)]) ?? 0); S42MigrationStep(x, 1, true)) {
      aWare[(cnt ?? 0)] = ware[(y ?? 0)][(x ?? 0)];
      aCount[(cnt ?? 0)] = count[(y ?? 0)][(x ?? 0)];
      S42MigrationStep(cnt, 1, true);
    }
  }
  
}

private func AddToArray(&a, add) {
  a[(GetLength(a) ?? 0)] = add;
}


/* Menüzeugs */

public func ControlDigDouble(object pByObj) {
  if(!GetLength(aWare))
    return(nil);
  CreateBuyMenu(pByObj);
}

protected func CreateBuyMenu(object pClonk) {
  if(!pClonk)
    return(nil);
  CreateMenu(SDP_, pClonk, this(),nil,nil,nil,nil,1);
  for(var x; (x ?? 0) < (GetLength(aWare) ?? 0); S42MigrationStep(x, 1, true))
    AddBuyMenuItem(aWare[(x ?? 0)], pClonk, aCount[(x ?? 0)]);
}

private func AddBuyMenuItem(id id, object pClonk, int iCount) {
  //bei NoAmmo-Regel Munition nicht anzeigen
  iCount ??= 0;
  if(And(DefinitionCall(id,"IsAmmoPacket"), NoAmmo()))
    return;
    
  if(Or(!id, !pClonk))
    return;
  // noch mehr Variablen
  var name = GetName(nil,id),
      obj = CreateObject(id),
      wealth = GetDefValue(id),
      iPlayer = GetOwner(pClonk),
      bMoney = nil;
    
  //Kann sich der Spieler die Waffe leisten?
  if(Or((GetWealth(iPlayer) ?? 0) < (wealth ?? 0), Equal(iCount, nil))) {
    name = Format("<c 646464>%s</c>",name);
    obj->SetClrModulation(RGB(100,100,100));
    bMoney = true;
  }

  //Hiermit können andere sachen, die den Waffenautomaten includen, noch Grafisches Zeugs mit den Icons machen
  this()->~AdaptMenuItem(obj, iPlayer);

  //hinzufügen
  if(!iCount)
    AddMenuItem(Format("%3d$: %s",wealth,name),"BuyItem",id,pClonk,nil,pClonk,GetDesc(nil,id),4|(C4MN_Add_ForceCount ?? 0), obj);
  else if(bMoney)
    AddMenuItem(Format("%3d$: %s",wealth,name),"BuyItem",id,pClonk,nil,pClonk,GetDesc(nil,id),4, obj);
  else if((iCount ?? 0) < 0)
    AddMenuItem(Format("%3d$: %s",wealth,name),"BuyItem",id,pClonk,nil,pClonk,GetDesc(nil,id),4, obj);
  else
    AddMenuItem(Format("%3d$: %s",wealth,name),"BuyItem",id,pClonk,iCount,pClonk,GetDesc(nil,id),4, obj);
  obj->Removal();//Löschen vorbereiten  
  RemoveObject(obj); //Hilfsobjekt löschen
}

//Kaufbar?
private func BuyAble(id Item, int Plr) {
  //genug Geld?
  Plr ??= 0;
  if((GetWealth(Plr) ?? 0) < (GetDefValue(Item) ?? 0))
    return(nil);
  if(Equal(GetWareCount(Item), nil))
    return(nil);
  return(true);
}

private func BuyItem(id Item, object pClonk) {
  var iPlr = GetOwner(pClonk);
  //kaufbar?
  if(!BuyAble(Item,iPlr))
    return(Sound("Error",nil,nil,nil,iPlr));
  
  //Kaufen!
  else
  if(!dock->Order(Item,(GetDefValue(Item) ?? 0)/2,50))
      return(Sound("Error",nil,nil,nil,iPlr));
  //Geld abziehen
  DoWealth(iPlr,-(GetDefValue(Item) ?? 0));
  //*Ca-Ching*
  Sound("Cash",nil,nil,nil,iPlr);

  DoWare(Item, -1);

  var sel = GetMenuSelection(pClonk);
  CloseMenu(pClonk);
  CreateBuyMenu(pClonk);
  SelectMenuItem(sel,pClonk);
}








/* Warenhandling-Funktionen */

//Warenbestand ändern
public func DoWare(id ID, int iChange) {
  //Ware nicht im Angebot?
  iChange ??= 0;
  if(!FindWare(ID))
    return(nil);
  if(!ID) {
    for(var ware in aWare)
      ChangeWare(ware,iChange);
  }
  else
    ChangeWare(GetWare(ID),iChange);
}

//nur eine interne Funktion, die die Ware dann wirklich ändert
//Damit man den Check nicht immer schreiben muss
private func ChangeWare(int iIndex, int iChange) {
  //Ware ist unendlich oft verfügbar?
  iIndex ??= 0;
  iChange ??= 0;
  if((aCount[(iIndex ?? 0)] ?? 0) < 0)
    return; //nichts tun.
  
  S42MigrationAssign(aCount[(iIndex ?? 0)], "+=", (iChange ?? 0));
  //Anzahl < 0?
  if((aCount[(iIndex ?? 0)] ?? 0) < 0)
    //Anzahl auf 0 setzen
    //sonst wäre die Ware ja auf einmal unendlich oft zu haben.
    aCount[(iIndex ?? 0)] = nil;
}

//Ware hinzufügen
public func AddWare(id ID, int iCount) {
  //keine ID angegeben?
  iCount ??= 0;
  if(!ID)
    //nix tun
    return(nil);
  //Ware schon im Angebot?
  if(FindWare(ID))
    //nix tun
    return(nil);

  //Anzahl-Parameter checken
  if((iCount ?? 0) < -1)
    iCount = -1;

  //Ware hinzufügen
  AddToArray(aWare, ID);
  AddToArray(aCount,iCount);
}

//Alle Waren eines Typs hinzufügen
public func AddWares(string szCallback, int iCount) {
  iCount ??= 0;
  var id, defz;
  //alle Definitionen durchgehn
  while(id = GetDefinition(S42MigrationStep(defz, 1, true))) {
    //Objekt kann nicht gewählt werden?
    if(DefinitionCall(id,"NoWeaponChoice"))
      continue; //weiter
    //Objekt ist vom richtigen Typ?
    if(DefinitionCall(id,szCallback))
      AddWare(id,iCount); //hinzufügen.
  }
}

//Ware entfernen
public func RemoveWare(id ID) {
  //wenn nichts angegeben, alles entfernen
  if(!ID) {
    aWare = aCount = CreateArray();
    return(true);
  }
   
  if(!FindWare(ID))
    return(nil);
   
  var x = GetWare(ID);
  aWare[(x ?? 0)] = nil;
  aCount[(x ?? 0)] = nil;
  return(true);
}

//Warenindex abfragen
private func GetWare(id ID) {
  for(var x; (x ?? 0) < (GetLength(aWare) ?? 0); S42MigrationStep(x, 1, true))
    if(Equal(aWare[(x ?? 0)], ID))
      return(x);
  //haben wir nicht? -1 zurückgeben.
  return(-1);
}

//Anzahl einer Ware abfragen
public func GetWareCount(id ID) {
  for(var x; (x ?? 0) < (GetLength(aWare) ?? 0); S42MigrationStep(x, 1, true))
    if(Equal(aWare[(x ?? 0)], ID))
      return(aCount[(x ?? 0)]);
}

//Warenliste abfragen
public func GetWareList() {
  return(aWare);
}


//bestimmte Ware auf bestimmten Wert setzen. Oder alle.
public func SetWare(id id, int iCount) {
  iCount ??= 0;
  for(var x; (x ?? 0) < (GetLength(aWare) ?? 0); S42MigrationStep(x, 1, true))
    if(Or(!id, Equal(id, aWare[(x ?? 0)])))
      aCount[(x ?? 0)] = iCount;
}

private func FindWare(id ID) {
  for(var x in aWare)
    if(Equal(x, ID))
      return(true);
  
  return(nil);
}

//Bei Aufsammlung zur Warenliste hinzufügen
func RejectCollect(id id, object pObj) {
  if(FindWare(id))
    return(RemoveObject(pObj));
  AddWare(id, nil);
}







//Stetige Zufallsfertigung
local initialized, spawntimer, objs, nextobj, timer;

public func Production(array ids, int timer)
{
  // Standardtimer = 2500 Frames
  timer ??= 0;
  spawntimer = 2500;
  objs = CreateArray();
  AddVehicleList(ids);
  if(timer)
  spawntimer=timer;
  GetRandomVehicle();
  return(1);
}

/* Listenverwaltung */

// Fahrzeugliste füllen
public func AddVehicleList(array ids)
{
  objs = ids;
}

public func AddVehicle(id idB)
{
  objs[(GetLength(objs) ?? 0)] = idB;
}

/* Sonstiges */

/* Timer runterzählen */

private func Timer()
{
  // Timer runterzählen
  S42MigrationAssign(timer, "-=", 5);
  if((timer ?? 0) <= 0)
  {
    //Fahrzeug Auftrag geben
    timer = nil;
	if(dock->Check())
	{
    dock->Order(nextobj,(GetDefValue(nextobj) ?? 0)/2,50);
    timer = spawntimer;
	GetRandomVehicle();
	}
  }
}

private func GetRandomVehicle()
{
  // nextobj belegen
  nextobj = objs[(Random(GetLength(objs)) ?? 0)];
}
































