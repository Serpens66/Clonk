/*-- Niedergang --*/

#strict 3

local Blocked; // Treppenhausfunktionalität blockieren

/* Initialisierung */

protected func Initialize()
{
  // Eingang ist immer frei
  SetEntrance(1);
  // Treppenhausfunktionalität aktiv
  Blocked = nil;
  // Gleich prüfen, ob es Sinn macht
  CheckBack();
  // Für Neuaufbau: Schloss aktualisieren
  UpdateAllCastleParts();
  return(1);
}

/* Bewegung im Treppenhaus */

public func ContainedDown()
{
  [$TxtDown$]
  // Darunterliegendes Treppenhaus suchen
  var pStaircase;
  if (!(pStaircase = FindStaircase(nil, 13 / 2 + 10))) return(nil);
  // Inhalt ins Zielobjekt und verlassen
  var pObj;
  while (pObj = Contents())
  {
  	pObj->Enter(pStaircase);
  	AddCommand(pObj, "Exit");
  }
  return(1);
}

public func ContainedLeft()
{
  [$TxtExit$]
  // Inhalt raus
  for (var i = nil; Contents(i); S42MigrationStep(i, 1, true))
  	AddCommand(Contents(i), "Exit");
  return(1);
}

public func ContainedRight()
{
  [$TxtExit$]
  // Inhalt raus
  for (var i = nil; Contents(i); S42MigrationStep(i, 1, true))
  	AddCommand(Contents(i), "Exit");
  return(1);
}

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

/* Status */

public func IsStaircase() { return(!Blocked); }

/* KEIN TimerCall */

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

public func CastleChange() { return(CheckBack()); }

// Bei Aufschlag prüfen, ob es überhaupt noch Sinn macht!
protected func Hit()
  {
  // Alles aktualisieren für TransferZone-Änderungen...
  // Eigenes CheckBack steckt gleich mit drin
  UpdateAllCastleParts();
  }


private func FindCastlePart(int iOffsetX, int iOffsetY)
{
  // Anderes Bauteil in entsprechender Richtung suchen
  iOffsetX ??= 0;
  iOffsetY ??= 0;
  var pPart; 
  while (pPart = FindObject(nil, iOffsetX,iOffsetY,nil,nil, OCF_Fullcon,nil,nil, NoContainer(), pPart))
  	if (pPart->~CastlePartWidth())
      return(pPart);
  // Keins gefunden
  return(nil);
}

/* Objekt wird entfernt */

protected func Destruction()
{
  // Inhalt auswerfen, damit man nicht einfach umgebracht wird...
  var pObj;
  while (pObj = Contents())	
  	pObj->Exit();
  // Treppenhausfunktionalität blockieren...
  Blocked = 1; // (könnte statdessen auch zeitverzögertes CastleChange aufrufen)
  // ...und darunterliegendes Treppenhaus aktualisieren
  var pStaircase;
  if (pStaircase = FindStaircase(nil, 13 / 2 + 10))
  	pStaircase->~CastleChange();
  return(1);
}
