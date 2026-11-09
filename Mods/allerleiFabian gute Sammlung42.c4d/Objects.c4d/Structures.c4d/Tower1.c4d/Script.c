/*-- Kleiner Turm --*/

#strict 3

#include DOOR


/* Tür */

private func SoundDoorOpen()
{
  Sound("DoorOpen");
}

private func SoundDoorClose()
{
  Sound("DoorClose");
}

/* Aufstieg */

protected func CheckContents()
{
  while (Contents()) Exit(Contents(), nil,-10,nil, nil);
}

/* Initialisierung */

protected func Initialize()
{
  UpdateTransferZone();
}

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-19,-28,38,48);
}

protected func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg zur Plattform
  tx ??= 0;
  ty ??= 0;
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), -20, 20), Inside((ty ?? 0) - (GetY() ?? 0), -38, -12)) )
    if((GetY(pObj) ?? 0) > (GetY() ?? 0) - 12)
      return(HandleTransferAscend(pObj, tx, ty) );

  // Abstieg von der Plattform
  if(And(Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -20, 20), Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -38, -12)) )
    if(And(Inside((tx ?? 0) - (GetX() ?? 0), -20, 20), Inside((ty ?? 0) - (GetY() ?? 0), -12, 20)) )
      return(HandleTransferDescend(pObj, tx, ty) );

  // Transfer nicht möglich
  return(nil);
}

private func HandleTransferAscend(object pObj, int tx, int ty)
{
  tx ??= 0;
  ty ??= 0;
  if(And(Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -20, 20), Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -38, -12)) )
    return(nil);
    
  if(Equal(Contained(pObj), this()) )
    return(1);

  // Clonk soll den Turm betreten
  return (AddCommand(pObj,"Enter",this()));
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  tx ??= 0;
  ty ??= 0;
  if(Equal(Contained(pObj), this()) )
    return(1);
  Enter(this(), pObj);

  return(AddCommand(pObj,"Exit"));
}

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
