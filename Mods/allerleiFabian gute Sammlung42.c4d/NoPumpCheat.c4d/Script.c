/*-- Pumpe --*/

#strict 3

#appendto PUMP
#appendto SPMP

protected func Activate(iPlr)
{
  if(!Equal(GetID(), NOPC)) return(_inherited(iPlr));
  MessageWindow(GetDesc(), iPlr);
  return(1);
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
  if(FindObject(NOPC)) if(And(pPumpTarget, (Equal(GetID(pPumpTarget), LNKT)))) if(GBackSolid((GetX(pPumpTarget) ?? 0)-(GetX() ?? 0), (GetY(pPumpTarget) ?? 0)-(GetY() ?? 0))) return(nil);
  if (Or(!pPumpTarget, (Equal(GetID(pPumpTarget), LNKT)))) return(1);
  // Kapazität für gepumptes Material im Zielobjekt prüfen
  if (!CheckCapacity(pPumpTarget, 12, pPumpSource->GetMaterial(nil,1)))
    // Keine ausreichende Kapazität: nicht pumpen
    return(nil);
  // Ansonsten: pumpen
  return(1);
}

private func CheckCapacity(pTarget, iAmount, iMaterial) { return(_inherited(pTarget, iAmount, iMaterial)); }
