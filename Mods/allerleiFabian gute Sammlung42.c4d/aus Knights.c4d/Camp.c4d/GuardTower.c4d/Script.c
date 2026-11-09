/*-- Holz-Wachturm --*/

#strict 3
#include BAS3

local exit;
local battlements;

/*-- Ausrichtung von Burgbauteilen --*/

public func CastlePartWidth() { return (46); }
public func CastlePartOffsetX() { return(2); }
public func CastlePartHeight() { return (106); }
public func CastlePartTop() { return (-4); }

/* Initialisierung */

protected func Initialize()
{
  SetEntrance(1);
  SetSolidMask(96,nil,48,106,nil,nil);
  // Niedergang
  exit = CreateObject(CSTE, nil, -3, GetOwner() );
  exit->Activate(this(), true);
  UpdateTransferZone();
  UpdatePhase();
}

/* TimerCall */

private func CheckContents()
{
  while(Contents() ) Exit(Contents(), nil,-2,nil, nil);
}

/* Steuerung */

protected func ControlLeft()
{
  [$TxtLeftgate$]
  if(!!SEqual(GetAction(), "Idle")) return(SetAction("LeftClosed") );
  if(!!SEqual(GetAction(), "LeftClosed")) return(AllOpen() );
  if(!!SEqual(GetAction(), "RightClosed")) return(SetAction("BothClosed") );
  if(!!SEqual(GetAction(), "BothClosed")) return(SetAction("RightClosed") );
}

protected func ControlRight()
{
  [$TxtRightgate$]
  if(!!SEqual(GetAction(), "Idle")) return(SetAction("RightClosed"));
  if(!!SEqual(GetAction(), "RightClosed")) return(AllOpen());
  if(!!SEqual(GetAction(), "LeftClosed")) return(SetAction("BothClosed"));
  if(!!SEqual(GetAction(), "BothClosed")) return(SetAction("LeftClosed"));
}

/* Status */

private func RightGateOpen()
{
  if(!!SEqual(GetAction(), "RightClosed")) return(nil);
  if(!!SEqual(GetAction(), "BothClosed")) return(nil);
  return(1);
}

private func LeftGateOpen()
{
  if(!!SEqual(GetAction(), "LeftClosed")) return(nil);
  if(!!SEqual(GetAction(), "BothClosed")) return(nil);
  return(1);
}

/* Tor-Aktionen */

private func AllOpen()
{
  SetAction("Idle");
  Sound("WoodenGate");
  UpdatePhase();
}

private func LeftClosed()
{
  Sound("WoodenGate");
  UpdatePhase();
}

private func RightClosed()
{
  Sound("WoodenGate");
  UpdatePhase();
}

private func BothClosed()
{
  Sound("WoodenGate");
  UpdatePhase();
}

private func UpdatePhase()
{
  var gatephase = nil;
  if(!!SEqual(GetAction(), "BothClosed")) gatephase = 1;
  else if(!!SEqual(GetAction(), "LeftClosed")) gatephase = 2;
  else if(!!SEqual(GetAction(), "RightClosed")) gatephase = 3;
  
  SetSolidMask(96 + (48 * (gatephase ?? 0) + 192 * (battlements ?? 0)), nil, 48, 106, nil, nil);
}

public func CastleChange()
{
  UpdatePhase();
}

/* Wenn der Turm mal runterfällt */

protected func Hit()
{
  UpdateTransferZone();
}

/* Zerstoerung */

protected func Incineration()
{
  if(exit) RemoveObject(exit);
}

protected func IncinerationEx() { Incineration(); }
protected func Destruction() { Incineration(); }

/* Transfer */

protected func UpdateTransferZone()
{
  SetTransferZone(-23,-20,46,72);
}

protected func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg zur Plattform
  tx ??= 0;
  ty ??= 0;
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), -24, 24), Inside((ty ?? 0) - (GetY() ?? 0), -24, 24)) )
    if(Or(Contained(pObj), (GetY(pObj) ?? 0) > ((GetY() ?? 0) + 10)) )
      return(HandleTransferPlatform(pObj, tx, ty) );

  // Abstieg von der Plattform
  if(!Contained(pObj) )
    if(And(Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -27, 27), Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -30, nil)) )
      if(And(Inside((tx ?? 0) - (GetX() ?? 0), -24, 24), Inside((ty ?? 0) - (GetY() ?? 0), 15, 53)) )
        return(HandleTransferDescend(pObj, tx, ty) );

  // Ausgang rechts
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), 20, 28), Inside((ty ?? 0) - (GetY() ?? 0), nil, 53)) )
    return(HandleTransferGateRight(pObj, tx, ty) );

  // Ausgang links
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), -28, -20), Inside((ty ?? 0) - (GetY() ?? 0), nil, 53)) )
    return(HandleTransferGateLeft(pObj, tx, ty) );

  // Rausklettern rechts
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), 20, 28), Inside((ty ?? 0) - (GetY() ?? 0), -30, -1)) )
    AddCommand(pObj, "MoveTo", nil, (GetX() ?? 0) + 22, (GetY() ?? 0) - 15);

  // Rausklettern links
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), -28, -20), Inside((ty ?? 0) - (GetY() ?? 0), -30, -1)) )
    AddCommand(pObj, "MoveTo", nil, (GetX() ?? 0) - 22, (GetY() ?? 0) - 15);

  // Transfer nicht möglich
  return(nil);
}

private func HandleTransferPlatform(object pObj, int tx, int ty)
{
  // Clonk ist auf der Plattform: erledigt
  tx ??= 0;
  ty ??= 0;
  if(!Contained(pObj) )
    if(And(Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -24, 24), Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -50, nil)) )
      return(nil);

  // Clonk ist im Turm: aufsteigen
  if(Equal(Contained(pObj), this()) )
    return(Exit(pObj, nil, -2) );

  // Clonk soll den Turm betreten
  return(AddCommand(pObj, "Enter", this()) );
}
 
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Clonk ist im Turm: warten
  tx ??= 0;
  ty ??= 0;
  if(Equal(Contained(pObj), this()) ) return(1);

  // Clonk soll noch in die Mitte der Plattform laufen
  if(And(!Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -20, 20), !Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -20, nil)) )
    return(AddCommand(pObj, "MoveTo", nil, GetX(), (GetY() ?? 0) - 10) );

  // Clonk in den Turm versetzen (geschummelt: Tastaturclonks können dies nicht :)
  Enter(this(), pObj);
  
  // Clonk soll den Turm verlassen
  return(AddCommand(pObj, "Exit") );
}
 
private func HandleTransferGateRight(object pObj, int tx, int ty)
{
  // Tor ist offen: Turm loslassen
  tx ??= 0;
  ty ??= 0;
  if(RightGateOpen() )
    if(And(!!SEqual(GetAction(pObj), "Push"), Equal(GetActionTarget(nil, pObj), this())) )
      return(AddCommand(pObj, "UnGrab", this()) );
    else
      // Tor ist offen und losgelassen: erledigt
      return(nil);

  // Clonk soll den Turm anfassen
  if(Or(!SEqual(GetAction(pObj), "Push"), !Equal(GetActionTarget(nil, pObj), this())) )
    return(AddCommand(pObj, "Grab", this()) );
    
  return(ControlRight(pObj) );
}

private func HandleTransferGateLeft(object pObj, int tx, int ty)
{
  // Tor ist offen: Turm loslassen
  tx ??= 0;
  ty ??= 0;
  if(LeftGateOpen())
    if(And(!!SEqual(GetAction(pObj), "Push"), Equal(GetActionTarget(nil, pObj), this())) )
      return(AddCommand(pObj, "UnGrab", this()) );
    else
      // Tor ist offen und Clonk fasst es nicht an: erledigt    
      return(nil);
  
  // Clonk soll den Turm anfassen
  if(Or(!SEqual(GetAction(pObj), "Push"), !Equal(GetActionTarget(nil, pObj), this())) )
    return(AddCommand(pObj, "Grab", this()) );

  // Tor öffnen
  return(ControlLeft(pObj) );
}

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
