/*--- Bohrkopf ---*/

#strict 3

protected func DestructCheck()
{
  if (!FindLine()) RemoveObject();
}

private func FindDerrick()
{
  var pDerrick = FindObject(SDRK,nil,nil,nil,nil,nil,"Pump",this());
  if (pDerrick) return(pDerrick);
  return (FindObject(SDRK,nil,nil,nil,nil,nil,"Wait",this()));
}

private func FindLine()
{
  return(1);
}

private func Processing()
{
  // Normale Geschwindigkeit
  SetPhysical("Float", 150, 2);
  // Materialtransfer
  Transfer();
  // An der Oberkante: anhalten
  if ((GetY() ?? 0) < (GetY(FindDerrick()) ?? 0) + 36)
  {
    SetPosition(GetX(), (GetY(FindDerrick()) ?? 0) + 36);
    SetAction("Stop");
    SetComDir(COMD_Stop);
    SetYDir(nil);
    Sound("Click");
  }
}

protected func ContactBottom()
{
  SetAction("Stop");
  SetComDir(COMD_Stop);
  SetYDir(nil);
  Sound("Click");
}

private func Transfer()
{
  // Zielobjekt ermitteln
  var pPumpTarget = GetActionTarget(nil);
  // Kein Zielobjekt  
  if (!pPumpTarget) return(nil);
  // Zielobjekt erlaubt keinen Transfer
  if (!pPumpTarget->~AcceptTransfer()) return(nil);
  // Zielobjekt hat nicht genug Energie  
  if (And((!EnergyCheck(1000, pPumpTarget)), ObjectCount(ENRG))) return(nil);
  // Materialtransfer
  for (var i = nil; (i ?? 0) < 3; S42MigrationStep(i, 1, true))
    ObjectInsertMaterial(ExtractLiquid(nil, nil), pPumpTarget);
  // Blubbern
  if (!Random(5)) Bubble(nil, nil);
  // Fertig
  return(1);
}

private func Drilling()
{
  // Material prüfen
  var mat = GetMaterial(nil,3);
  // Nicht durch SolidMasks bohren
  if (Equal(Material("Vehicle"), mat))
  {
    Sound("MetalHit3");
    return(nil);
  }
  // Schneller in freien Bereichen
  if (Or((Equal(Material("Tunnel"), mat)), (Equal(Material("Sky"), mat))))
  {
    SetPhysical("Float", 150, 2);
    return(1);
  }
  // Langsamer in festen Materialien
  if (!GetMaterialVal("DigFree", "Material", mat))
    if (!GetMaterialVal("Instable", "Material", mat))
    {
      var iSpeed = 25;
      // Etwas hardgecodete Erkennung
      if (Equal(Material("Granite"), mat))
        iSpeed = 10;
      // Langsamere Geschwindigkeit
      SetPhysical("Float", (iSpeed ?? 0) + (Random(2 * (iSpeed ?? 0)) ?? 0), 2);
      FreeRect((GetX() ?? 0) - 1, GetY(), 3, 4, C4M_Solid);
      return(1);
    }
  // Normale Geschwindigkeit
  SetPhysical("Float", 100, 2);
  return(1);
}

