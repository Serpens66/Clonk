/*-- Uplink (Scriptvorlage) UPL0 --*/

#strict 3

local caller, building;
local xferTarget,freigegeben;
local pseudoflagge;

// Local(0): Funktionsmodus:  0 = Basis,  1 = Handelssystem
// Local(1): noch zu vergebende "Heilpunkte", bis wieder Geld fällig wird


/* Animation starten */

public func Init(target)
{
  // Arbeit aufnehmen
  building = target;
  SetAction("Turn", building);
  SetCategory(2);

// für den Basiskauf und damit sich keine Flagge aufs Gebäude setzt, eine unsichtbare Flagge erstellen, die sozusagen an den Uplink gebunden ist.
// durch SetCommand Buy klappt dann auch der Basiskauf über die Flagge und lagged nicht so.
if(!pseudoflagge)
  {
   pseudoflagge=CreateContents(FLAG,building,1);
   SetCategory(2,pseudoflagge);
   SetVisibility(VIS_None,pseudoflagge);
   pseudoflagge->~ Werdepseudoflagge();
  }

  // globale Warenliste erstellen, falls diese nicht vorhanden ist
  if (!FindObject(STWL))
    CreateObject(STWL,nil,nil,-1);
  // leere Angebotsliste für diesen Spieler erstellen, falls keine vorhanden
  if (!FindObjectOwner(STPO, GetOwner()))
    CreateObject(STPO,nil,nil,GetOwner());
}


/* Timer-Call */

protected func CheckBuilding()
{
  // Gebäude weg?
  if (Or(ActIdle(), !GetActionTarget()))
  {
    AttachTargetLost();
    return(nil);
  }
  return(1);
}

protected func AttachTargetLost()
{ 
  // unsichtbare Flagge auch entfernen
  if(pseudoflagge)
  RemoveObject(pseudoflagge);
  CloseMenu(caller);
  ChangeDef(UPLI);
  SetCategory(2576);
}

protected func Heal()
{
  var clonk;
  while (clonk = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember, nil,nil, building, clonk))

if(Equal(freigegeben, 1)) //dann alle außer Feinde heilen
  if ( !(Hostile(GetOwner(caller),GetOwner()) ) )
    {
      // Flammen totschlagen
      if (OnFire(clonk))
        Extinguish(clonk);
      // medizinische Hilfe
      if ((GetEnergy(clonk) ?? 0) < (GetPhysical("Energy",nil,clonk) ?? 0)/1000)
      {
        // Geld bezahlen, wenn nötig
        if (!Local(1))
          if (GetWealth(GetOwner()))
          {
            Local(1) = 50;
            DoWealth(GetOwner(), -5);
            Sound("UnCash");
          }
        else return(nil);
        // Clonk heilen
        S42MigrationStep(Local(1), -1);
        DoEnergy(+2, clonk);
      }
    }

if(Equal(freigegeben, nil)) // dann nur Besitzer heilen
 while (clonk = FindObject(nil, nil,nil,nil,nil, OCF_CrewMember, nil,nil, building, clonk))
    if (Equal(GetOwner(clonk), GetOwner()))
 {
      // Flammen totschlagen
      if (OnFire(clonk))
        Extinguish(clonk);
      // medizinische Hilfe
      if ((GetEnergy(clonk) ?? 0) < (GetPhysical("Energy",nil,clonk) ?? 0)/1000)
      {
        // Geld bezahlen, wenn nötig
        if (!Local(1))
          if (GetWealth(GetOwner()))
          {
            Local(1) = 50;
            DoWealth(GetOwner(), -5);
            Sound("UnCash");
          }
        else return(nil);
        // Clonk heilen
        S42MigrationStep(Local(1), -1);
        DoEnergy(+2, clonk);
      }
    }

}


/* Ausrichtung wechseln */

public func ChangeMode()
{
  var target = building;
  Sound("Click");
  if (Local(nil)) ChangeDef(UPLB);
           else ChangeDef(UPLT);
  this()->Init(target);
}


/* Transfer */

private func TransferMenu()
{
  // Menü öffnen
  CreateMenu(GetID(building),caller,nil,nil,"Transfer",nil,1);
  // Gebäude mit Uplink
  AddBase(UPLB);
  AddBase(UPLT);
  // Fahrzeuge
  Add2Menu(SHTL);
  Add2Menu(TANK);
  Add2Menu(CVLA);
  Add2Menu(SUB1);
  Add2Menu(CAN_);
  Add2Menu(CCD_);
  Add2Menu(CGC_);
  Add2Menu(MAM_);
  Add2Menu(TSP_);
  Add2Menu(TCN_);
  Add2Menu(CVFC); //Raumschifffabrik, Raumschiffe werden in Hangar verschoben

}



private func AddBase(uplinkID)
{
  var upl, obj;
  while (upl = FindObject(uplinkID,nil,nil,-1,-1, nil,"Turn",nil,nil, upl))
  {
if((Hostile(GetOwner(upl),GetOwner()))) return(nil);
  
    obj = GetActionTarget(nil,upl);

if(Equal(GetOwner(obj), GetOwner())){
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,caller, nil,upl, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,caller, nil,upl, "Transfer hier heraus",2,1);}

if(upl->~StatusFreigabe())
if(!Equal(GetOwner(obj), GetOwner())){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,caller, nil,upl, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,caller, nil,upl, "Transfer hier heraus",2,1);}
  }
}

/*
// alte Funktion
private func AddBase(uplinkID)
{
  var upl, obj;
  while (upl = FindObjectOwner(uplinkID, GetOwner(), 0,0,-1,-1, 0,"Turn",0, upl))
  {
    obj = GetActionTarget(0,upl);
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,caller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,caller, 0,upl, "Transfer hier heraus",2,1);
  }
}
*/



private func Add2Menu(objID)
{
  var obj;
  while (obj = FindObject(objID,nil,nil,-1,-1, OCF_Fullcon, nil,nil,nil,obj))
   { if((Hostile(GetOwner(obj),GetOwner()))) return(1);
      if ((ObjectDistance(obj) ?? 0) > 500) return(1);

if (And(!(Contained(obj)), Equal(GetOwner(obj), GetOwner()))) {
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,caller, nil,obj, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,caller, nil,obj, "Transfer hier heraus",2,1);}

if(And(!(Contained(obj)), !Equal(GetOwner(obj), GetOwner()))){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,caller, nil,obj, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,caller, nil,obj, "Transfer hier heraus",2,1);}
    }
}


private func TransferIn(par0, target, selection)
{
  // Objekte holen
  xferTarget = target;
  var xferT2 = target;
  if (target->~IsUplink())
    xferT2 = GetActionTarget(nil,target);
  CreateMenu(GetID(xferTarget), caller);
  AddMenuItem("Senden-Modus aktivieren", "TransferOut", XFER,caller, nil,xferTarget, "Wechseln in den Senden-Modus",2,1);
  // nichts versendbares
if (And(!(FindObject(nil,nil,nil,nil,nil,OCF_Collectible,nil,nil,xferT2)), !FindObject2(Find_Func("IsTransferabl"))))
    return(1);
  AddMenuItem("Alles holen", "TransferInAll", XFER,caller, nil,nil, "Alles hier herein transferieren",2,2);
  // Auflistung

for(var clonk in FindObjects(Find_Container(xferT2),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
    {AddMenuItem(Format("Den %v senden",GetName(clonk)), "DoTransferIn", GetID(clonk),caller, nil,clonk);}

for(var vehicle in FindObjects(Find_Container(xferT2),Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_) )  ))
    {AddMenuItem(Format("Das %v senden",GetName(vehicle)), "DoTransferIn", GetID(vehicle),caller, nil,vehicle);}



  var i, count, bool;
  var obj, objID;
  var num = 2;
  while (obj = Contents(i,xferT2))
  {
    bool =  (FindObjects(Find_Or(Find_Func("IsTransferabl"),Find_OCF(OCF_Collectible))));
    objID = GetID(obj);

//if(GetDefCoreVal("NoGet", "DefCore", objID)) return(1);

    count = 1;
    S42MigrationStep(i, 1);
    while (And((obj = Contents(i,xferT2)), (Equal(GetID(obj), objID))))
    {
      S42MigrationStep(count, 1);
      S42MigrationStep(i, 1);
    }

    if(And(And(And(And(And(And(And(And(And(And(And(And(And(!Equal(objID, CLNK), !Equal(objID, HZCK)), !Equal(objID, SHTL)), !Equal(objID, TANK)), !Equal(objID, CVLA)), !Equal(objID, SUB1)), !Equal(objID, CAN_)), !Equal(objID, CCD_)), !Equal(objID, CGC_)), !Equal(objID, MAM_)), !Equal(objID, TSP_)), !Equal(objID, TCN_)), !Equal(objID, SCR_)), (!(GetDefCoreVal("NoGet", "DefCore", objID)))) )  // keine Clonks, die sind bereits nach Namen sortiert und keine
  if (bool)
    {
      AddMenuItem("%s holen", "DoTransferIn", objID,caller, count,num);
      S42MigrationStep(num, 1);
    }
  }
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while ((selection ?? 0) >= (num ?? 0))
    S42MigrationStep(selection, -1);
  SelectMenuItem(selection,caller);
}

private func TransferOut(par0, target, selection)
{
  // Objekte senden
  xferTarget = target;
  CreateMenu(GetID(building), caller);
  AddMenuItem("Holen-Modus aktivieren", "TransferIn", XFER,caller, nil,xferTarget, "Wechseln in den Holen-Modus",2,nil);
  // nichts versendbares
  if (And(!(FindObject(nil,nil,nil,nil,nil,OCF_Collectible,nil,nil,building)),  !FindObject2(Find_Func("IsTransferabl"))))
    return(1);
  AddMenuItem("Alles senden", "TransferOutAll", XFER,caller, nil,nil, "Alles hier heraus transferieren",2,3);
  // Auflistung

for(var clonk in FindObjects(Find_Container(building),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
    {AddMenuItem(Format("Den %v senden",GetName(clonk)), "DoTransferOut", GetID(clonk),caller, nil,clonk);}

for(var vehicle in FindObjects(Find_Container(building),Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_))))
    {AddMenuItem(Format("Das %v senden",GetName(vehicle)), "DoTransferOut", GetID(vehicle),caller, nil,vehicle);}

  var i, count, bool;
  var obj, objID;
  var num = 2;
  while (obj = Contents(i,building))
  {
    bool =  (FindObjects(Find_Or(Find_Func("IsTransferabl"),Find_OCF(OCF_Collectible))));
    objID = GetID(obj);

// if(GetDefCoreVal("NoGet", "DefCore", objID)) return(1);

    count = 1;
    S42MigrationStep(i, 1);
    while (And((obj = Contents(i,building)), (Equal(GetID(obj), objID))))
    {
      S42MigrationStep(count, 1);
      S42MigrationStep(i, 1);
    }

    if(And(And(And(And(And(And(And(And(And(And(And(And(And(!Equal(objID, CLNK), !Equal(objID, HZCK)), !Equal(objID, SHTL)), !Equal(objID, TANK)), !Equal(objID, CVLA)), !Equal(objID, SUB1)), !Equal(objID, CAN_)), !Equal(objID, CCD_)), !Equal(objID, CGC_)), !Equal(objID, MAM_)), !Equal(objID, TSP_)), !Equal(objID, TCN_)), !Equal(objID, SCR_)), (!(GetDefCoreVal("NoGet", "DefCore", objID)))) )  // keine Clonks, die sind bereits nach Namen sortiert und keine "NoGet" Objekte, wie Licht usw.
    if (bool)
    {
      AddMenuItem("%s senden", "DoTransferOut", objID,caller, count,num);
      S42MigrationStep(num, 1);
    }
}
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while ((selection ?? 0) >= (num ?? 0))
    S42MigrationStep(selection, -1);
  SelectMenuItem(selection, caller);
}

private func DoTransferIn(objID, num)
{
if (And(And(And(And(And(And(And(And(And(And(And(And(!Equal(objID, CLNK), !Equal(objID, HZCK)), !Equal(objID, SHTL)), !Equal(objID, TANK)), !Equal(objID, CVLA)), !Equal(objID, SUB1)), !Equal(objID, CAN_)), !Equal(objID, CCD_)), !Equal(objID, CGC_)), !Equal(objID, MAM_)), !Equal(objID, TSP_)), !Equal(objID, TCN_)), !Equal(objID, SCR_)))
{
  // Fehlerprüfung
  var obj;
  if (xferTarget->~IsUplink()) obj = FindContents(objID, GetActionTarget(nil,xferTarget));
                          else obj = FindContents(objID, xferTarget);
  if (Or(!obj, (And((ObjectDistance(xferTarget) ?? 0) > 500, !xferTarget->~IsUplink()))))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, (GetX(xferTarget) ?? 0)-(GetX() ?? 0), (GetY(xferTarget) ?? 0)-(GetY() ?? 0), GetOwner());
  transport->Init(this());
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferIn(nil,xferTarget,num));}

if (Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Equal(objID, CLNK), Equal(objID, HZCK)), Equal(objID, SHTL)), Equal(objID, TANK)), Equal(objID, CVLA)), Equal(objID, SUB1)), Equal(objID, CAN_)), Equal(objID, CCD_)), Equal(objID, CGC_)), Equal(objID, MAM_)), Equal(objID, TSP_)), Equal(objID, TCN_)), Equal(objID, SCR_)))
{
  // Fehlerprüfung
  var obj=num;
  if (Or(!obj, (And((ObjectDistance(xferTarget) ?? 0) > 500, !xferTarget->~IsUplink()))))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, (GetX(xferTarget) ?? 0)-(GetX() ?? 0), (GetY(xferTarget) ?? 0)-(GetY() ?? 0), GetOwner());
  transport->Init(this());
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferIn(nil,xferTarget));}
}

private func DoTransferOut(objID, num)
{
if (And(And(And(And(And(And(And(And(And(And(And(And(!Equal(objID, CLNK), !Equal(objID, HZCK)), !Equal(objID, SHTL)), !Equal(objID, TANK)), !Equal(objID, CVLA)), !Equal(objID, SUB1)), !Equal(objID, CAN_)), !Equal(objID, CCD_)), !Equal(objID, CGC_)), !Equal(objID, MAM_)), !Equal(objID, TSP_)), !Equal(objID, TCN_)), !Equal(objID, SCR_)))
{
  // Fehlerprüfung
  var obj = FindContents(objID, building);
  if (Or(!obj, (And((ObjectDistance(xferTarget) ?? 0) > 500, !xferTarget->~IsUplink()))))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,nil,nil,GetOwner());
  transport->Init(xferTarget);
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferOut(nil,xferTarget,num));}

if (Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Equal(objID, CLNK), Equal(objID, HZCK)), Equal(objID, SHTL)), Equal(objID, TANK)), Equal(objID, CVLA)), Equal(objID, SUB1)), Equal(objID, CAN_)), Equal(objID, CCD_)), Equal(objID, CGC_)), Equal(objID, MAM_)), Equal(objID, TSP_)), Equal(objID, TCN_)), Equal(objID, SCR_)))
{
  // Fehlerprüfung
  var obj = num;
  if (Or(!obj, (And((ObjectDistance(xferTarget) ?? 0) > 500, !xferTarget->~IsUplink()))))
    return(Message("ClonkTransfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,nil,nil,GetOwner());
  transport->Init(xferTarget);
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferOut(nil,xferTarget));}

}

private func TransferInAll()
{
  // Fehlerprüfung
  if (And((ObjectDistance(xferTarget) ?? 0) > 500, !xferTarget->~IsUplink()))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, (GetX(xferTarget) ?? 0)-(GetX() ?? 0), (GetY(xferTarget) ?? 0)-(GetY() ?? 0), GetOwner());
  transport->Init(this());
  if (xferTarget->~IsUplink())
    xferTarget = GetActionTarget(nil,xferTarget);
  var i, obj;
  while (obj = Contents(i,xferTarget))
//  if (GetOCF(obj) & OCF_Collectible())
    if (Or((GetOCF(obj) ?? 0) & (OCF_Collectible ?? 0), FindObject2(Find_Func("IsTransferabl"))))
      Enter(transport, obj);
    else
      S42MigrationStep(i, 1);
}

private func TransferOutAll()
{
  // Fehlerprüfung
  if (And((ObjectDistance(xferTarget) ?? 0) > 500, !xferTarget->~IsUplink()))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,nil,nil,GetOwner());
  transport->Init(xferTarget);
  var i, obj;
  while (obj = Contents(i,building))
 // if (GetOCF(obj) & OCF_Collectible())
    if (Or((GetOCF(obj) ?? 0) & (OCF_Collectible ?? 0), FindObject2(Find_Func("IsTransferabl"))))
      Enter(transport, obj);
    else
      S42MigrationStep(i, 1);
}


/* Transfer angekommen */

public func TransferArrival()
{
  // Inhalt ins Gebäude
  var obj;
  while (obj = Contents())
{
if (Equal(GetID(obj), SHTL)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(SHTL,nil,nil,owner);}
if (Equal(GetID(obj), TANK)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(TANK,nil,nil,owner);}
if (Equal(GetID(obj), CVLA)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(CVLA,nil,nil,owner);}
if (Equal(GetID(obj), SUB1)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(SUB1,nil,nil,owner);}
if (Equal(GetID(obj), CAN_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(CAN_,nil,nil,owner);}
if (Equal(GetID(obj), CCD_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(CCD_,nil,nil,owner);}
if (Equal(GetID(obj), CGC_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(CGC_,nil,nil,owner);}
if (Equal(GetID(obj), MAM_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(MAM_,nil,nil,owner);}
if (Equal(GetID(obj), TSP_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(TSP_,nil,nil,owner);}
if (Equal(GetID(obj), TCN_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(TCN_,nil,nil,owner);}
if (Equal(GetID(obj), SCR_)) {var owner=GetOwner(obj);var inhalt=Contents(nil,obj);RemoveObject(obj);var obj=CreateObject(SCR_,nil,nil,owner);}

    Enter(building, obj); if(inhalt) CreateContents(inhalt,obj);}
}




/* Waffenwechsel des Gebäudes aufrufen */

public func SelectWeapons()
{
  // Aufruf an Gebäude
  if (building)
    building->~SelectWeapons(caller);
}


/* Steuerung */

public func KeyUp(par_caller)
{
  caller = par_caller;
  // nur eigene Clonks und evlt. Verbündete, je nach Freigabestatus

if(Equal(freigegeben, 1))
  if ( Hostile(GetOwner(caller),GetOwner()) )
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nicht für Feinde zugänglich!", this());
    Sound("Error",nil,nil,nil, (GetOwner(caller) ?? 0)+1);
    return(nil);
  }

if(Equal(freigegeben, nil))
  if (!Equal(GetOwner(caller), GetOwner()))
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nur für Besitzer freigegeben!", this());
    Sound("Error",nil,nil,nil, (GetOwner(caller) ?? 0)+1);
    return(nil);
  }

  return(1);
}




public func KeyDig(par_caller)
{
  caller = par_caller;
  // nur eigene Clonks und evlt. Verbündete, je nach Freigabestatus

if(Equal(freigegeben, 1))
  if ( Hostile(GetOwner(caller),GetOwner()) )
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nicht für Feinde zugänglich!", this());
    Sound("Error",nil,nil,nil, (GetOwner(caller) ?? 0)+1);
    return(nil);
  }

if(Equal(freigegeben, nil))
  if (!Equal(GetOwner(caller), GetOwner()))
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nur für Besitzer freigegeben!", this());
    Sound("Error",nil,nil,nil, (GetOwner(caller) ?? 0)+1);
    return(nil);
  }

  return(1);
}

func StatusFreigabe()
{
if(Equal(freigegeben, 1)) return(1);
return(nil);
}

func Freigabe(eins, caller)
{
if(Equal(freigegeben, nil)) {freigegeben=1;PlayerMessage(GetOwner(caller), "Uplinksystem wurde freigegeben!", caller);return;}
if(Equal(freigegeben, 1)) {freigegeben=nil;PlayerMessage(GetOwner(caller), "Uplinksystem wurde gesperrt!", caller);return;}
}


/* Uplink-Eigenschaft */

public func IsUplink(...) { return(1);
}

