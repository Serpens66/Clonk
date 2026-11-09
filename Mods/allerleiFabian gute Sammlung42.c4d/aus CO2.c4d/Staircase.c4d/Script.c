/*-- Treppenhaus --*/

#strict 3

#include BP5C // Burgteilfunktion

local gang;

protected func Initialize()
{
  // Tür ist immer offen
  SetEntrance(1);
  // Transferzone
  UpdateTransferZone();
	// Basisimplementation
  _inherited();
  // Niedergang erzeugen
gang = CreateObject(CE5C, nil, -24, GetOwner());
}

func Zumachen()
{
//fürs abschließen
  if(Equal(zuu, 1)) SetEntrance(nil);
  if(Equal(zuu, nil)) SetEntrance(1);
}


// Der Aufruf von CastleChange muss verzögert erfolgen, damit das Teil zum Aufruf auch wirklich weg ist
public func CastlePartDestruction()
{
if(gang) RemoveObject(gang);
  // Fundament?
  if (basement) 
  	RemoveObject(basement);
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange"))
        AddEffect("IntCPW2CastleChange", nil, 1, 2, nil, CPW2);
  return(1);
}


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
  if (!pCase) return(nil);
  TransferContents(pCase);
  return(1);
}

public func ContainedDown()
{
  [$TxtDown$]
  var pCase = FindStaircaseBelow();
  if (!pCase) return(nil);
  TransferContents(pCase);
  return(1);
}

public func ContainedLeft()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this());
  return(1);
}

public func ContainedRight()
{
  [$TxtExit$]
  // Inhalt raus
  TransferContents(this());
  return(1);
}

private func TransferContents(object pTarget)
{
  // Von oben runterzählen, damit Sachen wie TransferContents(this()) funktionieren.
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
      return(obj);
  // Keins gefunden
  return(nil);
}

private func FindStaircaseAbove()
{
  return(FindStaircase(nil, -(CastlePartHeight() ?? 0)/2 - 10) );
}

private func FindStaircaseBelow()
{
  return(FindStaircase(nil, (CastlePartHeight() ?? 0)/2 + 10) );
}

/* Status */

public func IsStaircase() { return(1); }
public func StaircaseExit() { return(-25); } // Oberer Ausgang

/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (40); }
public func CastlePartHeight() { return (74); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BSC1); }

/* Transfer */

public func UpdateTransferZone()
{
  // Nur mit darüberliegendem Niedergang
  if (FindStaircaseAbove() )
    if ((GetOCF() ?? 0) & (OCF_Fullcon ?? 0) )
      return(SetTransferZone(-8, -40, 16, 61));
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return(SetTransferZone());
}

public func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg
  tx ??= 0;
  ty ??= 0;
  if (And(Inside((tx ?? 0) - (GetX() ?? 0), -20, 20), Inside((ty ?? 0) - (GetY() ?? 0), -50, -20)) )
    if (Or(Equal(Contained(pObj), this()), (GetY(pObj) ?? 0) > (GetY() ?? 0) - 20))
      return(HandleTransferAscend(pObj, tx, ty) );

  // Abstieg
  if (And(Inside((tx ?? 0) - (GetX() ?? 0), -20, 20), Inside((ty ?? 0) - (GetY() ?? 0), -20, 37)) )
    if (Or(Equal(Contained(pObj), this()), (GetY(pObj) ?? 0) < (GetY() ?? 0) - 20))
      return(HandleTransferDescend(pObj, tx, ty) );

  // Transfer nicht möglich
  return(nil);
}

private func HandleTransferAscend(object pObj, int tx, int ty)
{
  // Treppenhaus betreten
  tx ??= 0;
  ty ??= 0;
  if (!Equal(Contained(pObj), this()))
    return(AddCommand(pObj, "Enter", this()));

  // Aufstieg
  return(ContainedUp(pObj));
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Darüberliegendes Treppenhaus suchen
  tx ??= 0;
  ty ??= 0;
  var pCase = FindStaircaseAbove();
  if (!pCase)
    return(HandleTransferDescendPlatform(pObj, tx, ty));
    
  if (!Equal(Contained(pObj), pCase))
    return(AddCommand(pObj, "Enter", pCase));
  
  return(pCase->ContainedDown(pObj));
} 

/* Direkter Abstieg von der Platform
 Ohne Treppenhaus läuft da eigentlich gar nix
 Wäre ja noch schöner, wenn Mausschummler einfach die Sprengpfeile aus CoFuT holen könnten...
 Damit ist der Pathfinder allerdings am Ende. Meist fehlt das Treppenhaus aber eh nur da,
 wo man nicht rein sollte */

private func HandleTransferDescendPlatform(object pObj, int tx, int ty)
{
  tx ??= 0;
  ty ??= 0;
  return(nil);
}

/* Forschung */

public func GetResearchBase() { return(B2C5); }
