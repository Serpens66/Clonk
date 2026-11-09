/*--- Zuflussrohr ---*/

#strict 3

protected func Initialize()
{
  SetAction("Connect");  
  SetVertex(nil, nil, GetX()); SetVertex(nil, 1, GetY());
  SetVertex(1, nil, GetX()); SetVertex(1, 1, GetY());
  SetPosition(nil, nil, this());
}

protected func Transfer()
{
  // Zielobjekt suchen
  var pPumpTarget = GetActionTarget(nil);
  // Kein Zielobjekt  
  if (!pPumpTarget) return(nil);
  // Zielobjekt akzeptiert keinen Transfer
  if (!pPumpTarget->~AcceptTransfer()) return(nil);
  // Zielobjekt hat nicht genug Energie (könnte man auch in AcceptTransfer verlegen...)
  if (!EnergyCheck(1000,pPumpTarget)) return(nil);
  // Materialtransfer
  for (var i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1, true))
  {
    var mat = ObjectExtractLiquid(GetActionTarget(1));
    if (Equal(mat, -1)) mat = ExtractLiquid(GetVertex(50, nil), GetVertex(50, 1));
    ObjectInsertMaterial(mat, pPumpTarget);
  }
  // Blubbern  
  if (!Random(5)) Bubble(GetVertex(50,nil), GetVertex(50,1));
}

public func LineBreak(bool fNoMsg)
{
  fNoMsg ??= false;
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}

private func BreakMessage()
{
  var pPumpTarget = GetActionTarget(nil);
  if (!Equal(GetID(pPumpTarget), LNKT))
    pPumpTarget = GetActionTarget(1);
  Message("$TxtLinebroke$", pPumpTarget);
}
