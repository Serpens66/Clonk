/*-- Pumpe --*/

#strict 3

local Barrel;
local TransferReady;

#include CXEC
#include BAS1


/* Initialisierung */

protected func Initialize()
{
  SetAction("Pump");
  Barrel = CreateContents(BARL);
  TransferReady = nil;
}

/* Status */

public func AcceptTransfer()
{
  return(TransferReady); 
}  

/* Pumpkontrolle */

private func Waiting()
{
  if (ReadyToPump())
  {
    SetAction("Pump");
    TransferReady = 1;
  }
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
}

private func Pumping()
{
  if (!ReadyToPump())
  {
    SetAction("Wait");
    TransferReady = nil;
  }
  if(FindObject(ENNN))
      Message(Format("%v",GetEnergy()),this());
}

private func ReadyToPump()
{
  // Zuflussrohr suchen
  var pSourcePipe = FindObject(SPIP,nil,nil,nil,nil,nil,"Connect",this());
  // Kein Zuflussrohr da: nicht pumpen
  if (!pSourcePipe) return(nil);
  // Zufluss-Leitungsbausatz suchen
  var pPumpSource = GetActionTarget(1, pSourcePipe);
  // Zuflussrohr ist gar nicht oder nicht an einen Leitungsbausatz angeschlossen: strange?!
  if (Or(!pPumpSource, (!Equal(GetID(pPumpSource), LNKT)))) return(1);
  // Abflussrohr suchen
  var pDrainPipe = FindObject(DPIP,nil,nil,nil,nil,nil,"Connect",this());
  // Kein Abflussrohr da: pumpen
  if (!pDrainPipe) return(1);
  // Abfluss-Zielobjekt suchen
  var pPumpTarget = GetActionTarget(1, pDrainPipe);
  // Abflussrohr ist gar nicht oder an einen Leitungsbausatz angeschlossen: pumpen
  if (Or(!pPumpTarget, (Equal(GetID(pPumpTarget), LNKT)))) return(1);
  // Kapazität für gepumptes Material im Zielobjekt prüfen
  if (!CheckCapacity(pPumpTarget, 12, pPumpSource->GetMaterial(nil,1)))
    // Keine ausreichende Kapazität: nicht pumpen
    return(nil);
  // Ansonsten: pumpen
  return(1);
}

private func CheckCapacity(pTarget, iAmount, iMaterial)
{
  //Log("%s %d %d", pTarget->GetName(), iAmount, iMaterial);
  // Teilweise gefüllte Fässer suchen
  var BarrelID = GetBarrelType(iMaterial);
  var objBarrel;
  // Wenn es keinen Fasstyp für diese Flüssigkeit gibt, kann auch nicht gepumpt werden!
  if (!BarrelID) return;
  while (objBarrel = FindObject(BarrelID,nil,nil,nil,nil,nil,nil,nil,pTarget,objBarrel))
    if (!objBarrel->BarrelIsFull())
      if (((S42MigrationAssign(iAmount, "-=", (objBarrel->BarrelMaxFill() ?? 0) - (objBarrel->GetAmount() ?? 0))) ?? 0) <= 0)
        // Noch Kapazität vorhanden
        return(1);
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

protected func Destruction() {
 if(Barrel) RemoveObject(Barrel); return(_inherited() ); }
protected func Incineration() { if(Barrel) RemoveObject(Barrel); return(_inherited() ); }
protected func IncinerationEx() { if(Barrel) RemoveObject(Barrel); return(_inherited() ); }
