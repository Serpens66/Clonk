/*-- Transfer --*/

#strict 3

local xferCaller, xferTarget,transferclonk,building;

// dieses Script sollte per #include in Objekte
// eingebunden werden, die den Transfer unterstützen


/* Transfer-Menü */

private func TransferMenu(caller)
{
  // Menü öffnen
  xferCaller = caller;
  if (!Equal(GetOwner(xferCaller), GetOwner()))
    return(1);
  CreateMenu(GetID(),xferCaller,nil,nil,"Transfer",nil,1);
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
  while (upl = FindObject(uplinkID,nil,nil,-1,-1, nil,"Turn",nil,nil, upl))
  {
if((Hostile(GetOwner(upl),GetOwner()))) return(nil);
    obj = GetActionTarget(nil,upl);
    if ((ObjectDistance(obj) ?? 0) > 500)
      return(1);

if(Equal(GetOwner(obj), GetOwner())){
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,xferCaller, nil,upl, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,xferCaller, nil,upl, "Transfer hier heraus",2,1);}

if(upl->~StatusFreigabe())
if(!Equal(GetOwner(obj), GetOwner())){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,xferCaller, nil,upl, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,xferCaller, nil,upl, "Transfer hier heraus",2,1);}
  }
}



private func Add2Menu(objID)
{
  var obj;
  while (obj = FindObject(objID,nil,nil,-1,-1, OCF_Fullcon, nil,nil,nil, obj))
   { if((Hostile(GetOwner(obj),GetOwner()))) return(1);
      if ((ObjectDistance(obj) ?? 0) > 500) return(1);

if (And(!(Contained(obj)), Equal(GetOwner(obj), GetOwner()))) {
    AddMenuItem(Format("Holen <- %s", GetName(obj)), "TransferIn", XFER,xferCaller, nil,obj, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s",GetName(obj)), "TransferOut",XFER,xferCaller, nil,obj, "Transfer hier heraus",2,1);}

if(And(!(Contained(obj)), !Equal(GetOwner(obj), GetOwner()))){
    AddMenuItem(Format("Holen <- %s von %s", GetName(obj), GetPlayerName(GetOwner(obj))), "TransferIn", XFER,xferCaller, nil,obj, "Transfer hier hinein",2,nil);
    AddMenuItem(Format("Senden -> %s von %s",GetName(obj), GetPlayerName(GetOwner(obj))), "TransferOut",XFER,xferCaller, nil,obj, "Transfer hier heraus",2,1);}
    }
}


private func TransferIn(par0, target, selection)
{
  // Objekte holen
  xferTarget = target;
  var xferT2 = target;
  if (target->~IsUplink())
    xferT2 = GetActionTarget(nil,target);
  CreateMenu(GetID(xferTarget), xferCaller);
  AddMenuItem("Senden-Modus aktivieren", "TransferOut", XFER,xferCaller, nil,xferTarget, "Wechseln in den Senden-Modus",2,1);
  // nichts versendbares
  if (And(!(FindObject(nil,nil,nil,nil,nil,OCF_Collectible,nil,nil,xferT2)), !FindObject2(Find_Func("IsTransferabl"))))
    return(1);
  AddMenuItem("Alles holen", "TransferInAll", XFER,xferCaller, nil,nil, "Alles hier herein transferieren",2,2);
  // Auflistung

for(var clonk in FindObjects(Find_Container(xferT2),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
{ AddMenuItem(Format("Den %v holen",GetName(clonk)), "DoTransferIn", GetID(clonk),xferCaller, nil,clonk);}

for(var vehicle in FindObjects(Find_Container(xferT2), Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_))))
{ AddMenuItem(Format("Das %v holen",GetName(vehicle)), "DoTransferIn", GetID(vehicle),xferCaller, nil,vehicle);}


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
      AddMenuItem("%s holen", "DoTransferIn", objID,xferCaller, count,num);
      S42MigrationStep(num, 1);
    }
  }
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while ((selection ?? 0) >= (num ?? 0))
    S42MigrationStep(selection, -1);
  SelectMenuItem(selection,xferCaller);
}

private func TransferOut(par0, target, selection)
{
  // Objekte senden
  xferTarget = target;
  var xferT2 = target;
  if (target->~IsUplink())
    xferT2 = GetActionTarget(nil,target);
  CreateMenu(GetID(), xferCaller);
  AddMenuItem("Holen-Modus aktivieren", "TransferIn", XFER,xferCaller, nil,xferTarget, "Wechseln in den Holen-Modus",2,nil);
  // nichts versendbares
  if (And(!(FindObject(nil,nil,nil,nil,nil,OCF_Collectible,nil,nil,this())),  !FindObject2(Find_Func("IsTransferabl"))))
    return(1);
  AddMenuItem("Alles senden", "TransferOutAll", XFER,xferCaller, nil,nil, "Alles hier heraus transferieren",2,3);
  // Auflistung

for(var clonk in FindObjects(Find_Container(),Find_Or(Find_ID(HZCK),Find_ID(CLNK))))
{ AddMenuItem(Format("Den %v senden",GetName(clonk)), "DoTransferOut", GetID(clonk),xferCaller, nil,clonk);}

for(var vehicle in FindObjects(Find_Container(),Find_Or(Find_Or(Find_ID(SHTL),Find_ID(TANK),Find_ID(CVLA),Find_ID(SUB1),Find_ID(CAN_),Find_ID(CCD_),Find_ID(CGC_),Find_ID(MAM_),Find_ID(TSP_),Find_ID(TCN_)),Find_ID(SCR_))))
{ AddMenuItem(Format("Das %v senden",GetName(vehicle)), "DoTransferOut", GetID(vehicle),xferCaller, nil,vehicle);}

  var i, count, bool;
  var obj, objID;
  var num = 2;
  while (obj = Contents(i))
  {
    bool =  (FindObjects(Find_Or(Find_Func("IsTransferabl"),Find_OCF(OCF_Collectible))));
    objID = GetID(obj);

//if(GetDefCoreVal("NoGet", "DefCore", objID)) return(1);

    count = 1;
    S42MigrationStep(i, 1);
    while (And((obj = Contents(i)), (Equal(GetID(obj), objID))))
    {
      S42MigrationStep(count, 1);
      S42MigrationStep(i, 1);
    }
    if(And(And(And(And(And(And(And(And(And(And(And(And(And(!Equal(objID, CLNK), !Equal(objID, HZCK)), !Equal(objID, SHTL)), !Equal(objID, TANK)), !Equal(objID, CVLA)), !Equal(objID, SUB1)), !Equal(objID, CAN_)), !Equal(objID, CCD_)), !Equal(objID, CGC_)), !Equal(objID, MAM_)), !Equal(objID, TSP_)), !Equal(objID, TCN_)), !Equal(objID, SCR_)), (!(GetDefCoreVal("NoGet", "DefCore", objID)))) )  // keine Clonks, die sind bereits nach Namen sortiert und keine
    if (bool)
    {
      AddMenuItem("%s senden", "DoTransferOut", objID,xferCaller, count,num);
      S42MigrationStep(num, 1);
    }
}
//if (selection && GetType(selection)==C4V_C4Object()) selection=1;
  while ((selection ?? 0) >= (num ?? 0))
    S42MigrationStep(selection, -1);
  SelectMenuItem(selection, xferCaller);
}

private func DoTransferIn(objID, num)
{
if (And(And(And(And(And(And(And(And(And(And(And(And(!Equal(objID, CLNK), !Equal(objID, HZCK)), !Equal(objID, SHTL)), !Equal(objID, TANK)), !Equal(objID, CVLA)), !Equal(objID, SUB1)), !Equal(objID, CAN_)), !Equal(objID, CCD_)), !Equal(objID, CGC_)), !Equal(objID, MAM_)), !Equal(objID, TSP_)), !Equal(objID, TCN_)), !Equal(objID, SCR_)))
{
  // Fehlerprüfung
  var obj;
  if (xferTarget->~IsUplink()) obj = FindContents(objID, GetActionTarget(nil,xferTarget));
                          else obj = FindContents(objID, xferTarget);
  if (Or(!obj, (ObjectDistance(xferTarget) ?? 0) > 500))
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS, (GetX(xferTarget) ?? 0)-(GetX() ?? 0), (GetY(xferTarget) ?? 0)-(GetY() ?? 0), GetOwner());
  transport->Init(this());
  Enter(transport, obj);
  // Menü wieder öffnen
  return(TransferIn(nil,xferTarget,num));}

if (Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Equal(objID, CLNK), Equal(objID, HZCK)), Equal(objID, SHTL)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK)), Equal(objID, TANK))){
  var obj=num;
  if (Or(!obj, (ObjectDistance(xferTarget) ?? 0) > 500))
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
if (Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Equal(objID, CLNK), Equal(objID, HZCK)), Equal(objID, SHTL)), Equal(objID, TANK)), Equal(objID, CVLA)), Equal(objID, SUB1)), Equal(objID, CAN_)), Equal(objID, CCD_)), Equal(objID, CGC_)), Equal(objID, MAM_)), Equal(objID, TSP_)), Equal(objID, TCN_)), Equal(objID, SCR_)))
{
  // Fehlerprüfung
  var obj = FindContents(objID);
  if (Or(!obj, (ObjectDistance(xferTarget) ?? 0) > 500))
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
  if (Or(!obj, (ObjectDistance(xferTarget) ?? 0) > 500))
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
  if ((ObjectDistance(xferTarget) ?? 0) > 500)
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
  if ((ObjectDistance(xferTarget) ?? 0) > 500)
    return(Message("Transfer nicht möglich", this()));
  // Transport starten
  var transport = CreateObject(TRNS,nil,nil,GetOwner());
  transport->Init(xferTarget);
  var i, obj;
  while (obj = Contents(i))
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

 for (var obj in FindObjects(Find_Container(this())))
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

if(Or(Or(Or(Or(Or(Or(Or(Or(Or(Or(Equal(GetID(obj), SHTL), Equal(GetID(obj), TANK)), Equal(GetID(obj), CVLA)), Equal(GetID(obj), SUB1)), Equal(GetID(obj), CAN_)), Equal(GetID(obj), CCD_)), Equal(GetID(obj), CGC_)), Equal(GetID(obj), MAM_)), Equal(GetID(obj), TSP_)), Equal(GetID(obj), TCN_)), Equal(GetID(obj), SCR_)))
{
var hangaar= FindObject2(Find_ID(CVFH),Find_Distance(200));
    if(hangaar)Enter(hangaar, obj); if(inhalt) CreateContents(inhalt,obj);}
 }

}

