/*-- Uplink (Scriptvorlage) UPL0 --*/

#strict

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
    CreateObject(STWL,0,0,-1);
  // leere Angebotsliste für diesen Spieler erstellen, falls keine vorhanden
  if (!FindObjectOwner(STPO, GetOwner()))
    CreateObject(STPO,0,0,GetOwner());
}


/* Timer-Call */

protected func CheckBuilding()
{
  // Gebäude weg?
  if (ActIdle() || !GetActionTarget())
  {
    AttachTargetLost();
    return(0);
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
  while (clonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, building, clonk))

if(freigegeben==1) //dann alle außer Feinde heilen
  if ( !(Hostile(GetOwner(caller),GetOwner()) ) )
    {
      // Flammen totschlagen
      if (OnFire(clonk))
        Extinguish(clonk);
      // medizinische Hilfe
      if (GetEnergy(clonk) < GetPhysical("Energy",0,clonk)/1000)
      {
        // Geld bezahlen, wenn nötig
        if (!Local(1))
          if (GetWealth(GetOwner()))
          {
            Local(1) = 50;
            DoWealth(GetOwner(), -5);
            Sound("UnCash");
          }
        else return(0);
        // Clonk heilen
        --Local(1);
        DoEnergy(+2, clonk);
      }
    }

if(freigegeben==0) // dann nur Besitzer heilen
 while (clonk = FindObject(0, 0,0,0,0, OCF_CrewMember(), 0,0, building, clonk))
    if (GetOwner(clonk) == GetOwner())
 {
      // Flammen totschlagen
      if (OnFire(clonk))
        Extinguish(clonk);
      // medizinische Hilfe
      if (GetEnergy(clonk) < GetPhysical("Energy",0,clonk)/1000)
      {
        // Geld bezahlen, wenn nötig
        if (!Local(1))
          if (GetWealth(GetOwner()))
          {
            Local(1) = 50;
            DoWealth(GetOwner(), -5);
            Sound("UnCash");
          }
        else return(0);
        // Clonk heilen
        --Local(1);
        DoEnergy(+2, clonk);
      }
    }

}


/* Ausrichtung wechseln */

public func ChangeMode()
{
  var target = building;
  Sound("Click");
  if (Local(0)) ChangeDef(UPLB);
           else ChangeDef(UPLT);
  this()->Init(target);
}


/* Transfer */

private func TransferMenu()
{
  // Menü öffnen
  CreateMenu(GetID(building),caller,0,0,"Transfer",0,1);
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
  while (upl = FindObject(uplinkID,0,0,-1,-1, 0,"Turn",0,0, upl))
  {
if((Hostile(GetOwner(upl),GetOwner()))) return(0);
  
    obj = GetActionTarget(0,upl);

if(GetOwner(obj)==GetOwner()){
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,caller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,caller, 0,upl, "Transfer hier heraus",2,1);}

if(upl->~StatusFreigabe())
if(GetOwner(obj)!=GetOwner()){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,caller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,caller, 0,upl, "Transfer hier heraus",2,1);}
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
  while (obj = FindObject(objID,0,0,-1,-1, OCF_Fullcon(), 0,0,0,obj))
   { if((Hostile(GetOwner(obj),GetOwner()))) return(1);
      if (ObjectDistance(obj) > 500) return(1);

if (!(Contained(obj)) && GetOwner(obj)==GetOwner()) {
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,caller, 0,obj, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,caller, 0,obj, "Transfer hier heraus",2,1);}

if(!(Contained(obj)) && GetOwner(obj)!=GetOwner()){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,caller, 0,obj, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,caller, 0,obj, "Transfer hier heraus",2,1);}
    }
}


private func TransferIn(par0, target, selection)
{
  // Objekte holen
  xferTarget = target;
  var xferT2 = target;
  if (target->~IsUplink())
    xferT2 = GetActionTarget(0,target);
  CreateMenu(GetID(xferTarget), caller);
  AddMenuItem("Senden-Modus aktivieren", "TransferOut", XFER,caller, 0,xferTarget, "Wechseln in den Senden-Modus",2,1);
  // nichts versendbares
if (!(FindObject(0,0,0,0,0,OCF_Collectible(),0,0,xferT2)) && !FindObject2(Find_Func("IsTransferabl")))
    return(1);
  AddMenuItem("Alles holen", "TransferInAll", XFER,caller, 0,0, "Alles hier herein transferieren",2,2);
  // Auflistung

for(var clonk in FindObjects(Find_Container(xferT2),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
    {AddMenuItem(Format("Den %v senden",GetName(clonk)), "DoTransferIn", GetID(clonk),caller, 0,clonk);}

for(var vehicle in FindObjects(Find_Container(xferT2),Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_) )  ))
    {AddMenuItem(Format("Das %v senden",GetName(vehicle)), "DoTransferIn", GetID(vehicle),caller, 0,vehicle);}



  var i, count, bool;
  var obj, objID;
  var num = 2;
  while (obj = Contents(i,xferT2))
  {
    bool =  (FindObjects(Find_Or(Find_Func("IsTransferabl"),Find_OCF(OCF_Collectible))));
    objID = GetID(obj);

//if(GetDefCoreVal("NoGet", "DefCore", objID)) return(1);

    count = 1;
    ++i;
    while ((obj = Contents(i,xferT2)) && (GetID(obj) == objID))
    {
      ++count;
      ++i;
    }

    if(objID!=CLNK && objID!=HZCK && objID!=SHTL && objID!=TANK && objID!=CVLA && objID!=SUB1 && objID!=CAN_ && objID!=CCD_ && objID!=CGC_ && objID!=MAM_ && objID!=TSP_ && objID!=TCN_ && objID!=SCR_ && (!(GetDefCoreVal("NoGet", "DefCore", objID))) )  // keine Clonks, die sind bereits nach Namen sortiert und keine 
  if (bool)
    {
      AddMenuItem("%s holen", "DoTransferIn", objID,caller, count,num);
      ++num;
    }
  }
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while (selection >= num)
    --selection;
  SelectMenuItem(selection,caller);
}

private func TransferOut(par0, target, selection)
{
  // Objekte senden
  xferTarget = target;
  CreateMenu(GetID(building), caller);
  AddMenuItem("Holen-Modus aktivieren", "TransferIn", XFER,caller, 0,xferTarget, "Wechseln in den Holen-Modus",2,0);
  // nichts versendbares
  if (!(FindObject(0,0,0,0,0,OCF_Collectible(),0,0,building)) &&  !FindObject2(Find_Func("IsTransferabl")))
    return(1);
  AddMenuItem("Alles senden", "TransferOutAll", XFER,caller, 0,0, "Alles hier heraus transferieren",2,3);
  // Auflistung

for(var clonk in FindObjects(Find_Container(building),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
    {AddMenuItem(Format("Den %v senden",GetName(clonk)), "DoTransferOut", GetID(clonk),caller, 0,clonk);}

for(var vehicle in FindObjects(Find_Container(building),Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_))))
    {AddMenuItem(Format("Das %v senden",GetName(vehicle)), "DoTransferOut", GetID(vehicle),caller, 0,vehicle);}

  var i, count, bool;
  var obj, objID;
  var num = 2;
  while (obj = Contents(i,building))
  {
    bool =  (FindObjects(Find_Or(Find_Func("IsTransferabl"),Find_OCF(OCF_Collectible))));
    objID = GetID(obj);

// if(GetDefCoreVal("NoGet", "DefCore", objID)) return(1);

    count = 1;
    ++i;
    while ((obj = Contents(i,building)) && (GetID(obj) == objID))
    {
      ++count;
      ++i;
    }

    if(objID!=CLNK && objID!=HZCK && objID!=SHTL && objID!=TANK && objID!=CVLA && objID!=SUB1 && objID!=CAN_ && objID!=CCD_ && objID!=CGC_ && objID!=MAM_ && objID!=TSP_ && objID!=TCN_ && objID!=SCR_ && (!(GetDefCoreVal("NoGet", "DefCore", objID))) )  // keine Clonks, die sind bereits nach Namen sortiert und keine "NoGet" Objekte, wie Licht usw.
    if (bool)
    {
      AddMenuItem("%s senden", "DoTransferOut", objID,caller, count,num);
      ++num;
    }
}
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while (selection >= num)
    --selection;
  SelectMenuItem(selection, caller);
}

private func DoTransferIn(objID, num)
{
if (objID!=CLNK && objID!=HZCK && objID!=SHTL && objID!=TANK && objID!=CVLA && objID!=SUB1 && objID!=CAN_ && objID!=CCD_ && objID!=CGC_ && objID!=MAM_ && objID!=TSP_ && objID!=TCN_ && objID!=SCR_)
{
  // Fehlerprüfung
  var obj;
  if (xferTarget->~IsUplink()) obj = FindContents(objID, GetActionTarget(0,xferTarget));
                          else obj = FindContents(objID, xferTarget);
  if (!obj || (ObjectDistance(xferTarget) > 500 && !xferTarget->~IsUplink()))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, GetX(xferTarget)-GetX(), GetY(xferTarget)-GetY(), GetOwner());
  transport->Init(this());
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferIn(0,xferTarget,num));}

if (objID==CLNK || objID==HZCK|| objID==SHTL || objID==TANK || objID==CVLA || objID==SUB1 || objID==CAN_ || objID==CCD_ || objID==CGC_ || objID==MAM_ || objID==TSP_ || objID==TCN_ || objID==SCR_)
{
  // Fehlerprüfung
  var obj=num;
  if (!obj || (ObjectDistance(xferTarget) > 500 && !xferTarget->~IsUplink()))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, GetX(xferTarget)-GetX(), GetY(xferTarget)-GetY(), GetOwner());
  transport->Init(this());
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferIn(0,xferTarget));}
}

private func DoTransferOut(objID, num)
{
if (objID!=CLNK && objID!=HZCK && objID!=SHTL && objID!=TANK && objID!=CVLA && objID!=SUB1 && objID!=CAN_ && objID!=CCD_ && objID!=CGC_ && objID!=MAM_ && objID!=TSP_ && objID!=TCN_ && objID!=SCR_)
{
  // Fehlerprüfung
  var obj = FindContents(objID, building);
  if (!obj || (ObjectDistance(xferTarget) > 500 && !xferTarget->~IsUplink()))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,0,0,GetOwner());
  transport->Init(xferTarget);
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferOut(0,xferTarget,num));}

if (objID==CLNK || objID==HZCK|| objID==SHTL || objID==TANK || objID==CVLA || objID==SUB1 || objID==CAN_ || objID==CCD_ || objID==CGC_ || objID==MAM_ || objID==TSP_ || objID==TCN_ || objID==SCR_)
{
  // Fehlerprüfung
  var obj = num;
  if (!obj || (ObjectDistance(xferTarget) > 500 && !xferTarget->~IsUplink()))
    return(Message("ClonkTransfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,0,0,GetOwner());
  transport->Init(xferTarget);
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferOut(0,xferTarget));}

}

private func TransferInAll()
{
  // Fehlerprüfung
  if (ObjectDistance(xferTarget) > 500 && !xferTarget->~IsUplink())
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, GetX(xferTarget)-GetX(), GetY(xferTarget)-GetY(), GetOwner());
  transport->Init(this());
  if (xferTarget->~IsUplink())
    xferTarget = GetActionTarget(0,xferTarget);
  var i, obj;
  while (obj = Contents(i,xferTarget))
//  if (GetOCF(obj) & OCF_Collectible())
    if (GetOCF(obj) & OCF_Collectible() || FindObject2(Find_Func("IsTransferabl")))
      Enter(transport, obj);
    else
      ++i;
}

private func TransferOutAll()
{
  // Fehlerprüfung
  if (ObjectDistance(xferTarget) > 500 && !xferTarget->~IsUplink())
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,0,0,GetOwner());
  transport->Init(xferTarget);
  var i, obj;
  while (obj = Contents(i,building))
 // if (GetOCF(obj) & OCF_Collectible())
    if (GetOCF(obj) & OCF_Collectible() || FindObject2(Find_Func("IsTransferabl")))
      Enter(transport, obj);
    else
      ++i;
}


/* Transfer angekommen */

public func TransferArrival()
{
  // Inhalt ins Gebäude
  var obj;
  while (obj = Contents())
{
if (GetID(obj)==SHTL) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(SHTL,0,0,owner);}
if (GetID(obj)==TANK) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(TANK,0,0,owner);}
if (GetID(obj)==CVLA) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(CVLA,0,0,owner);}
if (GetID(obj)==SUB1) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(SUB1,0,0,owner);}
if (GetID(obj)==CAN_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(CAN_,0,0,owner);}
if (GetID(obj)==CCD_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(CCD_,0,0,owner);}
if (GetID(obj)==CGC_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(CGC_,0,0,owner);}
if (GetID(obj)==MAM_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(MAM_,0,0,owner);}
if (GetID(obj)==TSP_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(TSP_,0,0,owner);}
if (GetID(obj)==TCN_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(TCN_,0,0,owner);}
if (GetID(obj)==SCR_) {var owner=GetOwner(obj);var inhalt=Contents(0,obj);RemoveObject(obj);var obj=CreateObject(SCR_,0,0,owner);}

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

if(freigegeben==1)
  if ( Hostile(GetOwner(caller),GetOwner()) )
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nicht für Feinde zugänglich!", this());
    Sound("Error",0,0,0, GetOwner(caller)+1);
    return(0);
  }

if(freigegeben==0)
  if (GetOwner(caller) != GetOwner())
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nur für Besitzer freigegeben!", this());
    Sound("Error",0,0,0, GetOwner(caller)+1);
    return(0);
  }

  return(1);
}




public func KeyDig(par_caller)
{
  caller = par_caller;
  // nur eigene Clonks und evlt. Verbündete, je nach Freigabestatus

if(freigegeben==1)
  if ( Hostile(GetOwner(caller),GetOwner()) )
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nicht für Feinde zugänglich!", this());
    Sound("Error",0,0,0, GetOwner(caller)+1);
    return(0);
  }

if(freigegeben==0)
  if (GetOwner(caller) != GetOwner())
  {
    PlayerMessage(GetOwner(caller), "Uplink-Funktionen|nur für Besitzer freigegeben!", this());
    Sound("Error",0,0,0, GetOwner(caller)+1);
    return(0);
  }

  return(1);
}

func StatusFreigabe()
{
if(freigegeben==1) return(1);
return(0);
}

func Freigabe(eins, caller)
{
if(freigegeben==0) {freigegeben=1;PlayerMessage(GetOwner(caller), "Uplinksystem wurde freigegeben!", caller);return();}
if(freigegeben==1) {freigegeben=0;PlayerMessage(GetOwner(caller), "Uplinksystem wurde gesperrt!", caller);return();}
}


/* Uplink-Eigenschaft */

public IsUplink: return(1);

