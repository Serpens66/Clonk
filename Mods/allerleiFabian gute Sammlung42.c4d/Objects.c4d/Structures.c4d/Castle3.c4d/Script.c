/*-- Schloss --*/

#strict 3

#include BAS9
#include DOOR
#include XFER  // für Uplink Transfer

local entr_left, entr_right;

protected func Initialize()
{
  // Niedergänge erzeugen
  entr_left = CreateObject(CSTE, -31, +12, GetOwner());
  entr_left->Activate(this(), nil);
  entr_right = CreateObject(CSTE, +5, -11, GetOwner());
  entr_right->Activate(this(), nil);
  UpdateTransferZone();
}

protected func Incineration()
{
  // Niedergänge entfernen
  if (entr_left) RemoveObject(entr_left);
  if (entr_right) RemoveObject(entr_right);
  return(_inherited() );
}

protected func IncinerationEx()
{
  // Niedergänge entfernen
  if (entr_left) RemoveObject(entr_left);
  if (entr_right) RemoveObject(entr_right);
  return(_inherited() );
}

protected func Destruction()
{
  // Niedergänge entfernen
  if (entr_left) RemoveObject(entr_left);
  if (entr_right) RemoveObject(entr_right);
  return(_inherited() );
}
  
protected func UpdateTransferZone()
{
  SetTransferZone(-50, -45, 85, 120);
}

protected func OnOwnerChanged() 
{ 
  // Besitzer der Niedergänge wechseln
  if (entr_left) entr_left->SetOwner(GetOwner());
  if (entr_right) entr_left->SetOwner(GetOwner());
}

/* Aufstieg zur Plattform */

public func ContainedLeft(pClonk)
{
  [$TxtAscendLeft$|Image=ASCN:1]
  if (!Hostile(GetOwner(), pClonk->GetOwner()))
    Exit(pClonk, -30, +10);
  return(1);
}

public func ContainedRight(pClonk)
{
  [$TxtAscendRight$|Image=ASCN:0]
  if (!Hostile(GetOwner(), pClonk->GetOwner()))
    Exit(pClonk, +5, -12);
  return(1);
}

protected func ContextLeftPlatform(object pClonk)
{
  [$TxtAscendLeft$|Image=ASCN:1|Condition=IsComplete]
  pClonk->ContainedCall("ContainedLeft", this());
}

protected func ContextRightPlatform(object pClonk)
{
  [$TxtAscendRight$|Image=ASCN:0|Condition=IsComplete]
  pClonk->ContainedCall("ContainedRight", this());
}

/* Kontext */

private func IsComplete()
{
  return ((GetCon() ?? 0) >= 100);
}

/* Türsteuerung */

private func SoundOpenDoor()
{
  return(Sound("GateOpen"));
}

private func SoundCloseDoor()
{
  return(Sound("GateClose"));
}

/* Transfer */

protected func ControlTransfer(pObj, tx, ty)
{
  // Feind? Guck doch selbst, wie du weiterkommst :P
  if(Hostile(GetOwner(), GetOwner(pObj)) ) return(nil);
  
  // Abstieg von der linken Plattform
  if(And(Inside(GetX(pObj), (GetX() ?? 0)-45, (GetX() ?? 0)-20), Inside(GetY(pObj), GetY(), (GetY() ?? 0)+20)) )
    if((ty ?? 0) > (GetY() ?? 0)+20 )
      return(HandleTransferDescendLeft(pObj, tx, ty) );
  // Abstieg von der rechten Plattform
  if(And(Inside(GetX(pObj), (GetX() ?? 0)-15, (GetX() ?? 0)+15), Inside(GetY(pObj), (GetY() ?? 0)-22, (GetY() ?? 0)-2)) )
    if((ty ?? 0) > (GetY() ?? 0)-2 )
      return(HandleTransferDescendRight(pObj, tx, ty) );
  // Aufstieg zur linken Plattform
  if(And(Inside(tx,(GetX() ?? 0)-45, (GetX() ?? 0)-20), Inside(ty, GetY(), (GetY() ?? 0)+20)) )
    return(HandleTransferAscendLeft(pObj, tx, ty) );
  // Aufstieg zur rechten Plattform
  if(And(Inside(tx,(GetX() ?? 0)-15, (GetX() ?? 0)+15), Inside(ty, (GetY() ?? 0)-22, (GetY() ?? 0)-2)) )
    return(HandleTransferAscendRight(pObj, tx, ty) );
  return(nil);
}

private func HandleTransferAscendLeft(pObj, tx, ty)
{
  // Schon am Ziel?
  if(And(Inside(GetX(pObj), (GetX() ?? 0)-45, (GetX() ?? 0)-20), Inside(GetY(pObj), GetY(), (GetY() ?? 0)+20)) )
    return(nil);

  // Turm betreten (wenn noch nicht geschehen) und nach links raus
  AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "ContainedLeft");
  if(!Equal(Contained(pObj), this()) )
    if(!(And(Inside(GetX(pObj), (GetX() ?? 0)-15, (GetX() ?? 0)+15), Inside(GetY(pObj), (GetY() ?? 0)-22, (GetY() ?? 0)-2)) ))
      AddCommand(pObj, "Enter", this() );
    else
      AddCommand(pObj, "Enter", entr_right);
  return(1);
}

private func HandleTransferAscendRight(pObj, tx, ty)
{
  // Schon am Ziel?
  if(And(Inside(GetX(pObj), (GetX() ?? 0)-15, (GetX() ?? 0)+15), Inside(GetY(pObj), (GetY() ?? 0)-22, (GetY() ?? 0) - 2)) )
    return(nil);
    
  // Turm betreten und nach rechts raus
  AddCommand(pObj, "Call", this(), nil,nil,nil,nil, "ContainedRight");
  if(!Equal(Contained(pObj), this()) )
    if(!(And(Inside(GetX(pObj), (GetX() ?? 0)-45, (GetX() ?? 0)-20), Inside(GetY(pObj), GetY(), (GetY() ?? 0)+20)) ))
      AddCommand(pObj, "Enter", this() );
    else
      AddCommand(pObj, "Enter", entr_left);
  return(1);
}

private func HandleTransferDescendLeft(pObj, tx, ty)
{
  // Nicht mehr auf der Plattform? Prima.
  if(Or(!(And(Inside(GetX(pObj), (GetX() ?? 0)-45, (GetX() ?? 0)-20), Inside(GetY(pObj), GetY(), (GetY() ?? 0)+20)) ), (Equal(Contained(pObj), this()))) )
    return(nil);
  
  return(AddCommand(pObj, "Enter", entr_left) );
}

private func HandleTransferDescendRight(pObj, tx, ty)
{
  // Nicht mehr auf der Plattform? Prima.
  if(Or(!(And(Inside(GetX(pObj), (GetX() ?? 0)-15, (GetX() ?? 0)+15), Inside(GetY(pObj), (GetY() ?? 0)-22, (GetY() ?? 0)-2)) ), (Equal(Contained(pObj), this()))) )
    return(nil);
  
  return(AddCommand(pObj, "Enter", entr_right) );
}
  
/* Forschung */

public func GetResearchBase() { return(CST2); }

