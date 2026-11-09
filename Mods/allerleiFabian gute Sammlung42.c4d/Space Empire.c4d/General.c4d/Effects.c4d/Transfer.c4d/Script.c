/*-- Transfer --*/

#strict

local xferCaller, xferTarget,transferclonk,building;

// dieses Script sollte per #include in Objekte
// eingebunden werden, die den Transfer unterstützen


/* Transfer-Menü */

private func TransferMenu(caller)
{
  // Menü öffnen
  xferCaller = caller;
  if (GetOwner(xferCaller) != GetOwner())
    return(1);
  CreateMenu(GetID(),xferCaller,0,0,"Transfer",0,1);
  // Gebäude mit Uplink
  AddBase(UPLB);
  AddBase(UPLT);
  // Fahrzeuge
  Add2Menu(SHTL);
  Add2Menu(CVLA);
  Add2Menu(SUB1);
  Add2Menu(TANK);
  Add2Menu(CAN_);
  Add2Menu(CCD_);
  Add2Menu(CGC_);
  Add2Menu(MAM_);
  Add2Menu(TSP_);
  Add2Menu(TCN_);
  Add2Menu(CVFC); //Raumschifffabrik, Raumschiffe werden in Hangar verschoben
}

/*
private func AddBase(uplinkID)
{
  var upl, obj;
  while (upl = FindObject(uplinkID,0,0,-1,-1, 0,"Turn",0,0, upl))
{
if(upl ->~ StatusFreigabe()){
  if(!(Hostile(GetOwner(upl),GetOwner())))
  {
    obj = GetActionTarget(0,upl);
    if (ObjectDistance(obj) > 500)
      return(1);

if(GetOwner(obj)==GetOwner()){
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,xferCaller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,xferCaller, 0,upl, "Transfer hier heraus",2,1);}

if(GetOwner(obj)!=GetOwner()){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,xferCaller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,xferCaller, 0,upl, "Transfer hier heraus",2,1);}
  }
}

if(!(upl ->~ StatusFreigabe())){
  while (upl = FindObjectOwner(uplinkID, GetOwner(), 0,0,-1,-1, 0,"Turn",0, upl))
  {
    obj = GetActionTarget(0,upl);
    if (ObjectDistance(obj) > 500)
      return(1);
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,xferCaller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,xferCaller, 0,upl, "Transfer hier heraus",2,1);
  }

}
}
*/


private func AddBase(uplinkID)
{
  var upl, obj;
  while (upl = FindObject(uplinkID,0,0,-1,-1, 0,"Turn",0,0, upl))
  {
if((Hostile(GetOwner(upl),GetOwner()))) return(0);
    obj = GetActionTarget(0,upl);
    if (ObjectDistance(obj) > 500)
      return(1);

if(GetOwner(obj)==GetOwner()){
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,xferCaller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,xferCaller, 0,upl, "Transfer hier heraus",2,1);}

if(upl->~StatusFreigabe())
if(GetOwner(obj)!=GetOwner()){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,xferCaller, 0,upl, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,xferCaller, 0,upl, "Transfer hier heraus",2,1);}
  }
}



private func Add2Menu(objID)
{
  var obj;
  while (obj = FindObject(objID,0,0,-1,-1, OCF_Fullcon(), 0,0,0, obj))
   { if((Hostile(GetOwner(obj),GetOwner()))) return(1);
      if (ObjectDistance(obj) > 500) return(1);

if (!(Contained(obj)) && GetOwner(obj)==GetOwner()) {
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,xferCaller, 0,obj, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,xferCaller, 0,obj, "Transfer hier heraus",2,1);}

if(!(Contained(obj)) && GetOwner(obj)!=GetOwner()){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,xferCaller, 0,obj, "Transfer hier hinein",2,0);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,xferCaller, 0,obj, "Transfer hier heraus",2,1);}
    }
}


private func TransferIn(par0, target, selection)
{
  // Objekte holen
  xferTarget = target;
  var xferT2 = target;
  if (target->~IsUplink())
    xferT2 = GetActionTarget(0,target);
  CreateMenu(GetID(xferTarget), xferCaller);
  AddMenuItem("Senden-Modus aktivieren", "TransferOut", XFER,xferCaller, 0,xferTarget, "Wechseln in den Senden-Modus",2,1);
  // nichts versendbares
  if (!(FindObject(0,0,0,0,0,OCF_Collectible(),0,0,xferT2)) && !FindObject2(Find_Func("IsTransferabl")))
    return(1);
  AddMenuItem("Alles holen", "TransferInAll", XFER,xferCaller, 0,0, "Alles hier herein transferieren",2,2);
  // Auflistung

for(var clonk in FindObjects(Find_Container(xferT2),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
{ AddMenuItem(Format("Den %v holen",GetName(clonk)), "DoTransferIn", GetID(clonk),xferCaller, 0,clonk);}

for(var vehicle in FindObjects(Find_Container(xferT2), Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_))))
{ AddMenuItem(Format("Das %v holen",GetName(vehicle)), "DoTransferIn", GetID(vehicle),xferCaller, 0,vehicle);}


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
      AddMenuItem("%s holen", "DoTransferIn", objID,xferCaller, count,num);
      ++num;
    }
  }
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while (selection >= num)
    --selection;
  SelectMenuItem(selection,xferCaller);
}

private func TransferOut(par0, target, selection)
{
  // Objekte senden
  xferTarget = target;
  var xferT2 = target;
  if (target->~IsUplink())
    xferT2 = GetActionTarget(0,target);
  CreateMenu(GetID(), xferCaller);
  AddMenuItem("Holen-Modus aktivieren", "TransferIn", XFER,xferCaller, 0,xferTarget, "Wechseln in den Holen-Modus",2,0);
  // nichts versendbares
  if (!(FindObject(0,0,0,0,0,OCF_Collectible(),0,0,this())) &&  !FindObject2(Find_Func("IsTransferabl")))
    return(1);
  AddMenuItem("Alles senden", "TransferOutAll", XFER,xferCaller, 0,0, "Alles hier heraus transferieren",2,3);
  // Auflistung

for(var clonk in FindObjects(Find_Container(),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
{ AddMenuItem(Format("Den %v senden",GetName(clonk)), "DoTransferOut", GetID(clonk),xferCaller, 0,clonk);}

for(var vehicle in FindObjects(Find_Container(),Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_))))
{ AddMenuItem(Format("Das %v senden",GetName(vehicle)), "DoTransferOut", GetID(vehicle),xferCaller, 0,vehicle);}

  var i, count, bool;
  var obj, objID;
  var num = 2;
  while (obj = Contents(i))
  {
    bool =  (FindObjects(Find_Or(Find_Func("IsTransferabl"),Find_OCF(OCF_Collectible))));
    objID = GetID(obj);

//if(GetDefCoreVal("NoGet", "DefCore", objID)) return(1);

    count = 1;
    ++i;
    while ((obj = Contents(i)) && (GetID(obj) == objID))
    {
      ++count;
      ++i;
    }
    if(objID!=CLNK && objID!=HZCK && objID!=SHTL && objID!=TANK && objID!=CVLA && objID!=SUB1 && objID!=CAN_ && objID!=CCD_ && objID!=CGC_ && objID!=MAM_ && objID!=TSP_ && objID!=TCN_ && objID!=SCR_ && (!(GetDefCoreVal("NoGet", "DefCore", objID))) )  // keine Clonks, die sind bereits nach Namen sortiert und keine 
    if (bool)
    {
      AddMenuItem("%s senden", "DoTransferOut", objID,xferCaller, count,num);
      ++num;
    }
}
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while (selection >= num)
    --selection;
  SelectMenuItem(selection, xferCaller);
}

private func DoTransferIn(objID, num)
{
if (objID!=CLNK && objID!=HZCK && objID!=SHTL && objID!=TANK && objID!=CVLA && objID!=SUB1 && objID!=CAN_ && objID!=CCD_ && objID!=CGC_ && objID!=MAM_ && objID!=TSP_ && objID!=TCN_ && objID!=SCR_)
{
  // Fehlerprüfung
  var obj;
  if (xferTarget->~IsUplink()) obj = FindContents(objID, GetActionTarget(0,xferTarget));
                          else obj = FindContents(objID, xferTarget);
  if (!obj || ObjectDistance(xferTarget) > 500)
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, GetX(xferTarget)-GetX(), GetY(xferTarget)-GetY(), GetOwner());
  transport->Init(this());
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferIn(0,xferTarget,num));}

if (objID==CLNK || objID==HZCK|| objID==SHTL || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK || objID==TANK){
  var obj=num;
  if (!obj || ObjectDistance(xferTarget) > 500)
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
if (objID==CLNK || objID==HZCK|| objID==SHTL || objID==TANK || objID==CVLA || objID==SUB1 || objID==CAN_ || objID==CCD_ || objID==CGC_ || objID==MAM_ || objID==TSP_ || objID==TCN_ || objID==SCR_)
{
  // Fehlerprüfung
  var obj = FindContents(objID);
  if (!obj || ObjectDistance(xferTarget) > 500)
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
  if (!obj || ObjectDistance(xferTarget) > 500)
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
  if (ObjectDistance(xferTarget) > 500)
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
  if (ObjectDistance(xferTarget) > 500)
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,0,0,GetOwner());
  transport->Init(xferTarget);
  var i, obj;
  while (obj = Contents(i))
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

 for (var obj in FindObjects(Find_Container(this())))
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

if(GetID(obj)==SHTL || GetID(obj)==TANK || GetID(obj)==CVLA || GetID(obj)==SUB1 || GetID(obj)==CAN_ || GetID(obj)==CCD_ || GetID(obj)==CGC_ || GetID(obj)==MAM_ || GetID(obj)==TSP_ || GetID(obj)==TCN_ || GetID(obj)==SCR_)
{
var hangaar= FindObject2(Find_ID(CVFH),Find_Distance(200));
    if(hangaar)Enter(hangaar, obj); if(inhalt) CreateContents(inhalt,obj);}
 }

}

