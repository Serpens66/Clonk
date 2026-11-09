/*--- Bohrturm ---*/

#strict 3

#include CXEC

#include DRBS

/* Lokale Variablen */

local pBarrel;
local pDrillHead;

/* Initialisierung */

protected func Initialize()
{
  // Internes Fass als Zwischenlager erzeugen
  pBarrel = CreateContents(BARL);
  return(1);
}

public func LineConnectType()
{
  // Schon ein Abflussrohr da? Dann machen wir mit Energie weiter.
  if (FindObject(DPIP, nil,nil,nil,nil, nil, "Connect", this()) )
    return(PWRL);
  // Sonst Abfluss.
  return(DPIP);
}

private func PipeHeadCheck()
{
  // Bei Bedarf Bohrkopf und Leitung erzeugen
  if (!pDrillHead) 
  {              
    // Bohrkopf
    pDrillHead = CreateObject(PIPH, nil, 36, GetOwner());
    SetAction("Pump", pDrillHead);
    ObjectSetAction(pDrillHead, "Stop", this());
    CreateObject(DPLI, nil, nil, -1)->DPLI::Init(this(), pDrillHead);
  }
  return(pDrillHead);
}

/* Status */

public func AcceptTransfer()
{
  return (!!SEqual(GetAction(), "Pump"));
}

/* Steuerung */

private func StartDrillHead(object pCaller)
{
  PipeHeadCheck();
  Sound("Click");
  if (!EnergyCheck(1000)) return(nil);
  SetComDir(COMD_Down, pDrillHead);
  ObjectSetAction(pDrillHead, "Drill");
  SetPlrView(pCaller->GetController(), pDrillHead);
  DigFreeRect((GetX(pDrillHead) ?? 0)-1, (GetY() ?? 0) + (GetObjHeight() ?? 0) / 2, 3, (GetY(pDrillHead) ?? 0) - (GetY() ?? 0) - (GetObjHeight() ?? 0) / 2);
}

protected func ControlDownSingle(pCaller)
{
  [$CtrlDrillDesc$|Image=PIPH|Method=Classic]
  if(!GetPlrJumpAndRunControl(pCaller->GetController()))
    StartDrillHead(pCaller);
}

protected func ControlDown(pCaller)
{
  [$CtrlDrillDesc$|Image=PIPH|Method=JumpAndRun]
  SetPlrView(pCaller->GetController(), pDrillHead);
  if(GetPlrJumpAndRunControl(pCaller->GetController()))
    StartDrillHead(pCaller);
}

protected func ControlUp(pCaller)
{
  [$CtrlRetractDesc$|Image=PIPH]
  PipeHeadCheck();
  Sound("Click");
  if (!EnergyCheck(1000)) return(nil);
  SetComDir(COMD_Up, pDrillHead);
  ObjectSetAction(pDrillHead, "Move");
  SetPlrView(pCaller->GetController(),pDrillHead);
}

private func StopDrillHead(object pCaller)
{
  Sound("Click");
  ObjectSetAction(pDrillHead, "Stop");
  SetComDir(COMD_Stop, pDrillHead);
  SetYDir(nil, pDrillHead);
  SetPlrView(pCaller->GetController(), pDrillHead);
}

protected func ControlLeft(pCaller)
{
  [$CtrlStopDesc$|Image=PIPH|Method=Classic]
  if(!GetPlrJumpAndRunControl(pCaller->GetController()))
    StopDrillHead(pCaller);
  else
  {
    AddCommand(pCaller, "Call", this(), nil,nil,nil,nil, "ContMoveLeft");
    AddCommand(pCaller, "UnGrab");
  }
}

protected func ControlRight(pCaller)
{
  [$CtrlStopDesc$|Image=PIPH|Method=Classic]
  if(!GetPlrJumpAndRunControl(pCaller->GetController()))
    StopDrillHead(pCaller);
  else
  {
    AddCommand(pCaller, "Call", this(), nil,nil,nil,nil, "ContMoveRight");
    AddCommand(pCaller, "UnGrab");
  }
}

public func ContMoveLeft(object caller) { caller->SetComDir(COMD_Left); }
public func ContMoveRight(object caller) { caller->SetComDir(COMD_Right); }

protected func ControlUpReleased(pCaller)
{
  return(StopDrillHead(pCaller));
}

protected func ControlDownReleased(pCaller)
{
  return(StopDrillHead(pCaller));
}

/* Pumpkontrolle */

protected func Waiting()
{
  if (ReadyToPump())
    SetAction("Pump", pDrillHead);
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
}

protected func Pumping()
{
  if (!ReadyToPump())
    SetAction("Wait", pDrillHead);
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
}

protected func ReadyToPump()
{
  // Abflussrohr suchen
  var pDrainPipe = FindObject(DPIP,nil,nil,nil,nil,nil,"Connect",this());
  // Kein Abflussrohr da: pumpen
  if (!pDrainPipe) return(1);
  // Abflussrohr ist gar nicht oder an einen Leitungsbausatz angeschlossen: pumpen
  var pPumpTarget = GetActionTarget(1, pDrainPipe);
  if (Or(!pPumpTarget, (Equal(GetID(pPumpTarget), LNKT)))) return(1);
  // Kapazität für gepumptes Material im Zielobjekt prüfen
  if (!CheckCapacity(pPumpTarget, 12, GetMaterial(nil, (GetY(pDrillHead) ?? 0) - (GetY() ?? 0))))
    // Keine ausreichende Kapazität: nicht pumpen
    return(nil);
  // Ansonsten: pumpen
  return(1);
}

private func CheckCapacity(pTarget, iAmount, iMaterial)
{
  // Teilweise gefüllte Fässer suchen
  var BarrelID = GetBarrelType(iMaterial);
  var objBarrel;
  if (BarrelID)
  {
    while (objBarrel = FindObject(BarrelID,nil,nil,nil,nil,nil,nil,nil,pTarget,objBarrel))
      if (!objBarrel->BarrelIsFull())
        if (((S42MigrationAssign(iAmount, "-=", (objBarrel->BarrelMaxFill() ?? 0) - (objBarrel->GetAmount() ?? 0))) ?? 0) <= 0)
          // Noch Kapazität vorhanden
          return(1);
  }
  // Leere Fässer suchen
  BarrelID = EmptyBarrelID();
  objBarrel = nil;
  if (BarrelID)
  {
    while (objBarrel = FindObject(BarrelID,nil,nil,nil,nil,nil,nil,nil,pTarget,objBarrel))
      if (((S42MigrationAssign(iAmount, "-=", (objBarrel->BarrelMaxFill() ?? 0))) ?? 0) <= 0)
        // Noch Kapazität vorhanden
        return(1);
  }
  // Keine Kapazität vorhanden
  return(nil);
}

/* Zerstörung */

protected func Incineration() { if (pBarrel) RemoveObject(pBarrel); pBarrel=nil; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=nil; }
protected func IncinerationEx() { if (pBarrel) RemoveObject(pBarrel); pBarrel=nil; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=nil; }
protected func Destruction() 
{ if (pBarrel) RemoveObject(pBarrel); pBarrel=nil; if (pDrillHead) RemoveObject(pDrillHead); pDrillHead=nil; return(_inherited() ); }


/* Forschung */

public func GetResearchBase() { return(PUMP); }
