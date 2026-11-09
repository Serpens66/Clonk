/*--- Abflussrohr ---*/

#strict 3

protected func Initialize()
{
  SetAction("Connect");  
  SetVertex(nil,nil,GetX()); SetVertex(nil,1,GetY());
  SetVertex(1,nil,GetX()); SetVertex(1,1,GetY());
  SetPosition(nil, nil, this());
}

private func Transfer() // Abflussrohr führt zur Zeit keine Prüfung von AcceptTransfer beim Zielobjekt durch...
{
  var i;
  // Angeschlossene Objekte ermitteln
  var pPumpSource = GetActionTarget(nil);
  var pPumpTarget = GetActionTarget(1);
  // Kein Quellobjekt
  if (!pPumpSource) return(nil);
  // Quellobjekt hat nicht genug Energie
  if (!EnergyCheck(1000, pPumpSource)) return(nil);
  // Zielobjekt vorhanden
  if (pPumpTarget)
  {
    // Materialtransfer
    for (i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1, true))
      ObjectInsertMaterial(ObjectExtractLiquid(pPumpSource), pPumpTarget);
  }
  // Kein Zielobjekt vorhanden
  else
  {
    // Materialtransfer
    for (i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1, true))
      InsertMaterial(ObjectExtractLiquid(pPumpSource), GetVertex(50,nil), GetVertex(50,1));
  }
}

public func LineBreak(bool fNoMsg)
{
  fNoMsg ??= false;
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}
  
public func BreakMessage()
{
  // Angeschlossene Objekte ermitteln
  var pPumpSource = GetActionTarget(nil);
  var pPumpTarget = GetActionTarget(1);
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  if (Equal(GetID(pPumpSource), LNKT))
    Message("$TxtLinebroke$", pPumpSource);
  else
    Message("$TxtLinebroke$", pPumpTarget);
}
