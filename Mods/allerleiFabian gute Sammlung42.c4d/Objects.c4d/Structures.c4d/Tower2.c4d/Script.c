/*-- Wachturm --*/

#strict 3

#include BAS3
#include DOOR

local exit, gate_left, gate_right;

/* Initialisierung */

protected func Initialize() {
  // Tore erzeugen
  gate_left  = CreateObject(GATL, -14, +10, GetOwner());
  ObjectSetAction(gate_left,  "GateLeft",  this());
  gate_right = CreateObject(GATR, +15, +10, GetOwner());
  ObjectSetAction(gate_right, "GateRight", this());
  // Ausgang erzeugen
  exit = CreateObject(CSTE, nil, -20, GetOwner());
  exit->Activate(this(), true);
  // Transferzone setzen
  UpdateTransferZone();
  return(1);
}



/* Kontext */ 

protected func Incineration()
{
  if (exit)       RemoveObject(exit);
  if (gate_left)  RemoveObject(gate_left);
  if (gate_right) RemoveObject(gate_right);
}

protected func IncinerationEx() { Incineration(); }
protected func Destruction() { Incineration(); _inherited(); }
 
public func ContextGateLeft(caller) {
  [$TxtLeftgate$|Image=GATL|Condition=IsFullCon]
  SetCommand(caller, "Grab", this());
  AppendCommand(caller, "Call", this(), nil, nil, nil, nil, "ControlLeft");
  return(1); 
}
  
public func ContextGateRight(caller) {
  [$TxtRightgate$|Image=GATL|Condition=IsFullCon]
  SetCommand(caller, "Grab", this());
  AppendCommand(caller, "Call", this(), nil, nil, nil, nil, "ControlRight");
  return(1);
}
  
private func IsFullCon() { return((GetCon() ?? 0) >= 100); }

/* Steuerung */

protected func ControlLeft(caller) {
  [$TxtLeftgate$]
  if (Hostile(GetController(caller), GetOwner())) return(nil);
  var gate = FindObject(nil, nil, nil, nil, nil, nil, "GateLeft", this());
  if (!gate) return(nil);
  gate->Activate();
  return(1);
}
  
protected func ControlRight(caller) {
  [$TxtRightgate$]
  if (Hostile(GetController(caller), GetOwner())) return(nil);
  var gate = FindObject(nil, nil, nil, nil, nil, nil, "GateRight", this());
  if (!gate) return(nil);
  gate->Activate();
  return(1);
}
 
/* Inhalt auf die Plattform versetzen */

protected func DoorClosed() {
  while (Contents()) Exit(Contents(), nil, -20, nil, nil);
}

/* Wenn der Turm mal runterfällt */

protected func Hit() {
  UpdateTransferZone();
}

/* Transfer */

protected func UpdateTransferZone() {
  SetTransferZone(-23, -40, 46, 72);
}

protected func ControlTransfer(object pObj, int tx, int ty)
{
  // Aufstieg zur Plattform
  tx ??= 0;
  ty ??= 0;
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), -24, 24), Inside((ty ?? 0) - (GetY() ?? 0), -50, -24)) )
    if((GetY(pObj) ?? 0) > (GetY() ?? 0) - 24)
      return(HandleTransferPlatform(pObj, tx, ty) );
  
  // Abstieg von der Plattform    
  if(And(Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -24, 24), Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -50, -24)) )
    if(And(Inside((tx ?? 0) - (GetX() ?? 0), -24, 24), Inside((ty ?? 0) - (GetY() ?? 0), -24, 32)) )
      return(HandleTransferDescend(pObj, tx, ty) );
      
  // Ausgang rechts
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), 20, 28), Inside((ty ?? 0) - (GetY() ?? 0), -24, 32)) )
    return(HandleTransferGateRight(pObj, tx, ty) );
  
  // Ausgang links
  if(And(Inside((tx ?? 0) - (GetX() ?? 0), -28, 20), Inside((ty ?? 0) - (GetY() ?? 0), -24, 32)) )
    return(HandleTransferGateLeft(pObj, tx, ty) );

  return(nil);
}

private func HandleTransferPlatform(object pObj, int tx, int ty)
{
  // Clonk ist auf der Plattform: erledigt
  tx ??= 0;
  ty ??= 0;
  if(And(Inside((GetX(pObj) ?? 0) - (GetX() ?? 0), -24, 24), Inside((GetY(pObj) ?? 0) - (GetY() ?? 0), -40, -24)) )
    return(nil);

  // Clonk ist im Turm: warten
  if(Equal(Contained(pObj), this()) )
    return(1);

  // Clonk soll den Turm betreten
  return(AddCommand(pObj,"Enter",this()) );
}
  
private func HandleTransferDescend(object pObj, int tx, int ty)
{
  // Clonk ist im Turm: warten
  tx ??= 0;
  ty ??= 0;
  if(Equal(Contained(pObj), this()) )
    return(1);
  // Clonk in den Turm versetzen (geschummelt: Tastaturclonks können dies nicht :)
  Enter(this(),pObj);
  // Clonk soll den Turm verlassen
  return(AddCommand(pObj,"Exit") );
}
 
private func HandleTransferGateRight(object pObj, int tx, int ty)
{
  // Tor suchen
  tx ??= 0;
  ty ??= 0;
  if (!gate_right) return(nil);
  
  // Tor ist offen: Turm loslassen
  if((GetY(gate_right) ?? 0) - (GetY() ?? 0) < 0)
    if(And(!!SEqual(GetAction(pObj), "Push"), Equal(GetActionTarget(nil,pObj), this())) )
      return(AddCommand(pObj, "UnGrab", this()) );
       
  // Tor ist offen: Erledigt
  if((GetY(gate_right) ?? 0) - (GetY() ?? 0) < 0) return(nil);
  
  // Clonk soll den Turm anfassen
  if(Or((!SEqual(GetAction(pObj), "Push")), (!Equal(GetActionTarget(nil, pObj), this()))) )
    return(AddCommand(pObj, "Grab", this()) );
    
  // Tor bewegt sich: warten
  if (Equal(GetComDir(gate_right), COMD_Up)) return(1);

  // Tor öffnen
  return(ControlRight(pObj));
}

private func HandleTransferGateLeft(object pObj, int tx, int ty)
{
  // Tor suchen
  tx ??= 0;
  ty ??= 0;
  if (!gate_left) return(nil);
 
  // Tor ist offen: Turm loslassen
  if((GetY(gate_left) ?? 0) - (GetY() ?? 0) < 0)
    if(And(!!SEqual(GetAction(pObj), "Push"), Equal(GetActionTarget(nil, pObj), this())) )
      return(AddCommand(pObj, "UnGrab", this()) );
      
  // Tor ist offen: Erledigt
  if((GetY(gate_left) ?? 0) - (GetY() ?? 0) < 0) return(nil);
  
  // Clonk soll den Turm anfassen
  if( Or((!SEqual(GetAction(pObj), "Push")), (!Equal(GetActionTarget(nil, pObj), this()))) )
    return(AddCommand(pObj, "Grab", this()) );
  
  // Tor bewegt sich: warten
  if(Equal(GetComDir(gate_left), COMD_Up) ) return(1);
  
  // Tor öffnen
  return(ControlLeft(pObj) );
}

// Loren nicht im Turm ausleeren
public func NoLorryEjection(object lorry) { return true; }
