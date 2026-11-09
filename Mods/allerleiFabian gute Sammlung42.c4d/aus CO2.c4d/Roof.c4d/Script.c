/*-- Wand --*/

#strict 3
#include BP5C // Burgteilfunktion
//#include BT5C //Treppenhäusle
/* Initialisierung */

protected func Initialize()
{
  // Tür ist immer offen
  SetEntrance(1);
  // Transferzone
  UpdateTransferZone();
// Basisimplementation
  _inherited();
  // Fertig
  return 1;
}

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (40); }
public func CastlePartHeight() { return (46); }
public func CastlePartTop() { return (-28); }
public func CastlePartBasement() { return nil; }

/* Bei Veränderung der Burg */

public func CastleChange()
{
  UpdateTransferZone();
}

/* Bewegung im Treppenhaus */

public func ContainedUp()
{
  [$TxtUp$]
  var pCase = FindStaircaseAbove();
  if (!pCase) return nil;
  TransferContents(pCase);
  return 1;
}

public func ContainedDown()
{
  [$TxtDown$]
  var pCase = FindStaircaseBelow();
  if (!pCase) return nil;
  TransferContents(pCase);
  return 1;
}

public func ContainedLeft()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this);
  return 1;
}

public func ContainedRight()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this);
  return 1;
}

private func TransferContents(object pTarget)
{
  // Von oben runterzählen, damit Sachen wie TransferContents(this) funktionieren.
  var i;
  for (i = (ContentsCount() ?? 0) - 1; (i ?? 0) >= 0; S42MigrationStep(i, -1))
  {
    var obj = Contents(i);
    if (!obj) continue;
    Enter(pTarget, obj);
    AddCommand(obj, "Exit");
  }
}

/* Treppenhaus */

private func FindStaircase(int ox, int oy)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  ox ??= 0;
  oy ??= 0;
  var obj;
  while(obj = FindObject(nil, ox,oy, nil,nil, OCF_Fullcon, nil,nil, NoContainer(), obj) )
    if (obj->~IsStaircase())
      return obj;
  // Keins gefunden
  return nil;
}

private func FindStaircaseAbove()
{
  return FindStaircase(nil, -(CastlePartHeight() ?? 0)/2 - 10) ;
}

private func FindStaircaseBelow()
{
  return FindStaircase(nil, (CastlePartHeight() ?? 0)/2 + 10) ;
}

/* Status */

public func IsStaircase() { return 1; }
public func StaircaseExit() { return -25; } // Oberer Ausgang

/* Transfer */

public func UpdateTransferZone()
{
  // Nur mit darüberliegendem Niedergang
  if (FindStaircaseAbove() )
    if ((GetOCF() ?? 0) & (OCF_Fullcon ?? 0) )
      return SetTransferZone(-8, -40, 16, 61);
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return SetTransferZone();
}

public func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg
  tx ??= 0;
  ty ??= 0;
  if (Inside((tx ?? 0) - (GetX() ?? 0), -20, 20) && Inside((ty ?? 0) - (GetY() ?? 0), -50, -20) )
    if (S42MigrationEqual(Contained(pObj), this) || (GetY(pObj) ?? 0) > (GetY() ?? 0) - 20)
      return HandleTransferAscend(pObj, tx, ty) ;

  // Abstieg
  if (Inside((tx ?? 0) - (GetX() ?? 0), -20, 20) && Inside((ty ?? 0) - (GetY() ?? 0), -20, 37) )
    if (S42MigrationEqual(Contained(pObj), this) || (GetY(pObj) ?? 0) < (GetY() ?? 0) - 20)
      return HandleTransferDescend(pObj, tx, ty) ;

  // Transfer nicht möglich
  return nil;
}

private func HandleTransferAscend(object pObj, int tx, int ty)
{
  // Treppenhaus betreten
  tx ??= 0;
  ty ??= 0;
  if (!S42MigrationEqual(Contained(pObj), this))
    return AddCommand(pObj, "Enter", this);

  // Aufstieg
  return ContainedUp(pObj);
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Darüberliegendes Treppenhaus suchen
  tx ??= 0;
  ty ??= 0;
  var pCase = FindStaircaseAbove();
  if (!pCase)
    return HandleTransferDescendPlatform(pObj, tx, ty);
    
  if (!S42MigrationEqual(Contained(pObj), pCase))
    return AddCommand(pObj, "Enter", pCase);
  
  return pCase->ContainedDown(pObj);
} 

private func HandleTransferDescendPlatform(object pObj, int tx, int ty)
{
  tx ??= 0;
  ty ??= 0;
  return nil;
}

/* Forschung */

public func GetResearchBase() { return BP5C; }
