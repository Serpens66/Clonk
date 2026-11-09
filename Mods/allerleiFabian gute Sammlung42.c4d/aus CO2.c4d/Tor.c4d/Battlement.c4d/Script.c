/*-- Brustwehr --*/

#strict 3

public func AttachTo(object pTarget, int iDir)
{
  iDir ??= 0;
  SetAction("Attach", pTarget);
  SetDir(iDir);
  SetSolidMask(4, 15 * (GetDir() ?? 0), 3, 14);
  return(1);
}

private func CheckTarget()
{
  // Anderes Bauteil im Hintergrund
  if (FindCastlePart(nil, +1))
    return(RemoveObject());

  // Wand ist zersört worden
  if (!GetActionTarget())
    return(RemoveObject());
  if (Or(!((GetActionTarget()->GetOCF() ?? 0) & (OCF_Fullcon ?? 0)), !GetActionTarget()->~CastlePartWidth()))
    return(RemoveObject());
  return(1);
}

public func CastleChange()
{
	return(CheckTarget());
}	

private func FindCastlePart(int iOffsetX, int iOffsetY)
{
  // Anderes Bauteil in entsprechender Richtung suchen
  iOffsetX ??= 0;
  iOffsetY ??= 0;
  var pPart;
  while (pPart = FindObject(nil, iOffsetX, iOffsetY, nil,nil, nil,nil,nil, NoContainer(), pPart))
    if (pPart->~CastlePartWidth())
      return(pPart);
  // Keins gefunden
  return(nil);
}
  
private func FindStaircase(int iOffsetX, int iOffsetY)
{
  // Anderes Treppenhaus in entsprechender Richtung suchen
  iOffsetX ??= 0;
  iOffsetY ??= 0;
  var pPart;
  while (pPart = FindObject(nil, iOffsetX, iOffsetY, nil,nil, nil,nil,nil, NoContainer(), pPart))
    if ((pPart->GetOCF() ?? 0) & (OCF_Fullcon ?? 0))
      if (pPart->~IsStaircase())
      	return(pPart);
  // Keins gefunden
  return(nil);
}

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-18 + 15 * (GetDir() ?? 0), -15, 20, 30);
  // SolidMasks alter Burgen anpassen
  return(SetSolidMask(4, 14 * (GetDir() ?? 0), 3, 14));
}

protected func ControlTransfer(object pClonk, int iTx, int iTy)
{  
  // Hinweis: ControlTransfer der Brustwehr gibt immer 0 zurück,
  //          selbst wenn Bewegungskommandos gesetzt wurden, da
  //          der Clonk sich nie wirklich durch die Transferzone
  //          der Brustwehr bewegen soll.

  iTx ??= 0;
  iTy ??= 0;
  var pStairs;
  
  // Clonk ist unterhalb der Brustwehr...
  if ((pClonk->GetY() ?? 0) > (GetY() ?? 0) + 15)
    // ...und will nach oben hinter die Brustwehr
    if ((iTy ?? 0) < (GetY() ?? 0) + 15)
        if (Or((And((Equal(GetDir(), nil)), ((iTx ?? 0) > (GetX() ?? 0)))), (And((Equal(GetDir(), 1)), ((iTx ?? 0) < (GetX() ?? 0) - 2)))))
        // Soll durch das darunterliegende Treppenhaus aufsteigen
        if (pStairs = FindStaircase(10 - 20 * (GetDir() ?? 0), 10))
        {
                AddCommand(pClonk, "MoveTo", nil, pStairs->GetX(), iTy);
                AddCommand(pClonk, "MoveTo", nil, pStairs->GetX(), pStairs->GetY());
                return(nil);
       	}

  // Clonk ist oberhalb der Brustwehr...
  if ((pClonk->GetY() ?? 0) < (GetY() ?? 0) + 15)
    // ...und will nach unten
    if ((iTy ?? 0) > (GetY() ?? 0) + 14)
      // Soll durchs Treppenhaus gehen
      if (pStairs = FindStaircase(10 - 20 * (GetDir() ?? 0), 10))
      {
        AddCommand(pClonk, "MoveTo", nil, pStairs->GetX(), iTy);
        return(nil);
     	}
    
  // Clonk will direkt horizontal durch die Transferzone (ungewöhnlich)
  if (Inside((pClonk->GetY() ?? 0) - (GetY() ?? 0), -14, +14))
    if (Inside((iTy ?? 0) - (GetY() ?? 0), -14, +14))
      // Soll er drübersteigen
      {
        AddCommand(pClonk, "MoveTo", nil, GetX(), GetY());
        return(nil);
      }

  return(nil);
}
