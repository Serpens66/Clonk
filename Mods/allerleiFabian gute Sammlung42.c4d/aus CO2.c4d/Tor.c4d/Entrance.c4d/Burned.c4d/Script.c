/*-- Abgebrannter Niedergang --*/

#strict 3

/* Initialisierung */

protected func Incineration()
  {
  // Zur Sicherheit: Eingang blockieren
  SetEntrance(nil);
  // Burg aktualisieren (TransferZone des Treppenhauses darunter)
  UpdateAllCastleParts();
  }


/* Castle-Checks */

private func FindStaircase(int iOffsetX, int iOffsetY)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  iOffsetX ??= 0;
  iOffsetY ??= 0;
  var pStaircase;
  while (pStaircase = FindObject(nil, iOffsetX,iOffsetY,nil,nil, nil,nil,nil, NoContainer(), pStaircase))
    if ((pStaircase->GetOCF() ?? 0) & (OCF_Fullcon ?? 0))
    	if (pStaircase->~IsStaircase())
        return(pStaircase);
  // Keins gefunden
  return(nil);
}

private func CheckBack(bool fImmediateRemoval)
{
  // Nicht, wenn es noch fällt
  fImmediateRemoval ??= false;
  if (!GetContact(nil, -1, 15)) // CNAT_All
    // Aber später nochmal checken (fertig runtergefallen)
    return(ScheduleCall(this(), "CheckBack", 37));
  // Anderes Bauteil im Hintergrund
  if (Or(FindCastlePart(nil, -10), !FindStaircase(nil, 16)))
    if (fImmediateRemoval)
      return(RemoveObject());
    else
      // Ansonsten: Nächstes Frame nochmal checken und entfernen
      // Verzögert für hängende SolidMasks?
      ScheduleCall(this(), "CheckBack", 1, 1, true);
}

// Bei Aufschlag oder Schlossänderung prüfen, ob es überhaupt noch Sinn macht!
public func CastleChange() { return(CheckBack()); }
protected func Hit() { return(CheckBack()); }

private func FindCastlePart(int iOffsetX, int iOffsetY)
{
  // Anderes Bauteil in entsprechender Richtung suchen
  iOffsetX ??= 0;
  iOffsetY ??= 0;
  var pPart; 
  while (pPart = FindObject(nil, iOffsetX,iOffsetY,nil,nil, OCF_Fullcon,nil,nil, NoContainer(), pPart))
    if (!pPart->OnFire())
      if (pPart->~CastlePartWidth())
        return(pPart);
  // Keins gefunden
  return(nil);
}
