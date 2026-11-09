/*-- Turm --*/

#strict

#include BT5C // Bunker - Treppenhausfunktion

local wehr1, wehr2,gang,gateleft,gateright,gateleft2,gateright2,pWall;

func Zumachen()
{
//fürs abschließen
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);
}

/* Forschung */

public func GetResearchBase() { return(BT5C); }

/* Burgteil */

public func CastlePartWidth() { return(0); } // Wand hinter dem Turm gilt als Burgteil
public func StaircaseExit() { return(-31); } // Oberer Ausgang fürs Treppenhaus
public func CastlePartBasement() { return(0); } // Wand hinter dem Turm erzeugt das Fundament

// Der Aufruf von CastleChange muss verzögert erfolgen, damit das Teil zum Aufruf auch wirklich weg ist
public func CastlePartDestruction()
{
if(gang) RemoveObject(gang);
if(wehr1)RemoveObject(wehr1);
if(wehr2)RemoveObject(wehr2);
if(gateleft)RemoveObject(gateleft);
if(gateright)RemoveObject(gateright);
if(gateleft2)RemoveObject(gateleft2);
if(gateright2)RemoveObject(gateright2);
if(pWall)RemoveObject(pWall);
  // Fundament?
  if (basement) 
  	RemoveObject(basement);
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange"))
  	AddEffect("IntCPW2CastleChange", 0, 1, 2, 0, CPW2);
  return(1);
}


/* Torsteuerung */

protected func CtxControlLeft(object pClonk)
{
  // Kontextfunktion: Nur Kontrolle ermöglichen, wenn der Clonk auch tatsächlich das Tor anfasst
  // Zur Abwärtskompatibilität mit Szenarienscripten, die Tore öffnen, wird dies in eine eigene Funktion ausgelagert
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlLeft(pClonk));
}

protected func CtxControlRight(object pClonk)
{
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlRight(pClonk));
}

protected func FindGateLeft() { return(FindObject(0,0,0,0,0,0,"GateLeft",this())); }
protected func FindGateRight() { return(FindObject(0,0,0,0,0,0,"GateRight",this())); }

protected func NoFindGateLeft() { return(!FindObject(0,0,0,0,0,0,"GateLeft",this())); }
protected func NoFindGateRight() { return(!FindObject(0,0,0,0,0,0,"GateRight",this())); }


protected func ControlLeft()
{
  [$TxtLeftgate$|Image=CL5C|Condition=FindGateLeft]
  var pGate = FindGateLeft();
  if (!pGate) return(0);
  pGate->Activate();
  return(1);
}
 
protected func ControlRight()
{
  [$TxtRightgate$|Image=CL5C|Condition=FindGateRight]
  var pGate = FindGateRight();
  if (!pGate) return(0);
  pGate->Activate();
}

/* Kontext */ 
 
public func ContextGateLeft(object pClonk)
{
  [$TxtLeftgate$|Image=CL5C|Condition=FindGateLeft]
  SetCommand(pClonk, "Grab", this());
  AppendCommand(pClonk, "Call", this(), 0,0,0,0, "CtxControlLeft");  
  return(1); 
}
 
public func ContextGateRight(object pClonk)
{
  [$TxtRightgate$|Image=CR5C|Condition=FindGateRight]
  SetCommand(pClonk, "Grab", this());
  AppendCommand(pClonk, "Call", this(), 0,0,0,0, "CtxControlRight");  
  return(1); 
}

public func ContextGateLeft(object pClonk)
{
  [$RepairLeft$|Image=CL5C|Condition=NoFindGateLeft]
 gateleft = CreateObject(CL5C, -14, +10, GetOwner());
 gateleft->SetAction("GateLeft", this());
}
 
public func ContextGateRight(object pClonk)
{
  [$RepairRight$|Image=CR5C|Condition=NoFindGateRight]
 gateright = CreateObject(CR5C, +14, +10, GetOwner());
gateright->SetAction("GateRight", this());
}


/* Wenn der Turm mal runterfällt */

protected func Hit()
{
  UpdateTransferZone();
}

/* Kontext

protected func CtxControlLeft(object pClonk)
{
  // Kontextfunktion: Nur Kontrolle ermöglichen, wenn der Clonk auch tatsächlich das Tor anfasst
  // Zur Abwärtskompatibilität mit Szenarienscripten, die Tore öffnen, wird dies in eine eigene Funktion ausgelagert
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlLeft(pClonk));
}

protected func CtxControlRight(object pClonk)
{
  if (GetProcedure(pClonk) eq "PUSH")
    if (GetActionTarget(0, pClonk) == this())
      return (ControlRight(pClonk));
}



/* Initialisierung */

protected func Initialize()
{
  // Hintergrund-Wand erzeugen
  pWall = CreateObject(BPW3, 0, +30, GetOwner());   // geändert in staticobject
  // An Hintergrund-Wand ausrichten
  SetPosition(pWall->GetX(), pWall->GetY() + 7);
  // Brustwehren erzeugen
wehr1 = CreateObject(CP5C, -21, -27, GetOwner());
wehr1 ->AttachTo(pWall, DIR_Left());
wehr2 = CreateObject(CP5C, +22, -27, GetOwner());
wehr2 ->AttachTo(pWall, DIR_Right());
  // Niedergang erzeugen
gang = CreateObject(CE5C, 0, -31, GetOwner());
  // Tür ist immer offen
  SetEntrance(1);
  // Turmspezifische Teile erzeugen
  InitializeTower();
  // Transferzone setzen
  UpdateTransferZone();
  // Objekte im Umkreis ihre Überprüfungen durchführen lassen
  CastlePartInitialize();
  // Fertig
  return(1);
}

private func InitializeTower()
{
  // Tore erzeugen
gateleft2 = CreateObject(CL5C, -14, +10, GetOwner());
gateleft2->SetAction("GateLeft", this());
gateright2 = CreateObject(CR5C, +14, +10, GetOwner());
gateright2->SetAction("GateRight", this());
}

/* Transfer */

public func UpdateTransferZone()
{
  // Mit darüberliegendem Niedergang
  if (FindStaircaseAbove())
    if (GetOCF() & OCF_Fullcon())
      return(SetTransferZone(-19, -47, 38, 61));
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return(SetTransferZone(-19, +10, 38, 4));
}

public func ControlTransfer(object pObj, int iTx, int iTy)
{ 
	//Log("Zugbrücke - ControlTransfer");
  // Aufstieg
  if (Inside(iTx - GetX(), -20, +20) && Inside(iTy - GetY(), -50, -20))
    if ((pObj->Contained() == this()) || (GetY(pObj) > GetY() - 20))
      return(HandleTransferAscend(pObj, iTx, iTy)); // -> BT5C
  // Abstieg
  if (Inside(iTx - GetX(), -20, +20) && Inside(iTy - GetY(), -20, +37))
    if ((pObj->Contained() == this()) || (GetY(pObj) < GetY() - 20))
      return(HandleTransferDescend(pObj, iTx, iTy)); // -> BT5C
  // Ausgang rechts
  if (Inside(iTx - GetX(), +16, +30) && Inside(iTy - GetY(), -20, +37))
    return(HandleTransferGateRight(pObj, iTx, iTy));
  // Ausgang links
  if (Inside(iTx - GetX(), -30, -16) && Inside(iTy - GetY(), -20, +37))
    return(HandleTransferGateLeft(pObj, iTx, iTy));

 
    
  // Transfer nicht möglich
	//Log("can't handle");
  return(0);
}
  
private func HandleTransferGateRight(object pObj, int iTx, int iTy)
{
	//Log("HandleTransferGateRight");
  // Tor suchen
  var pGate = FindGateRight();
  if (pGate->GetY() - GetY() < 0)
    if ((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this()))
      return(AddCommand(pObj, "UnGrab", this()));      
  // Tor ist offen: erledigt
  if (pGate->GetY() - GetY() < 0) return(0);
  // Clonk soll den Turm anfassen
  if (!((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this())))
    return(AddCommand(pObj, "Grab", this()));
  // Tor bewegt sich: warten
  if (pGate->GetComDir() == COMD_Up()) return(1);
  // Tor öffnen
  return(ControlRight(pObj));
}

private func HandleTransferGateLeft(object pObj, int iTx, int iTy)
{
	//Log("HandleTransferGateLeft");
  // Tor suchen
  var pGate = FindGateLeft();
  // Tor ist offen: Turm loslassen
  if (pGate->GetY() - GetY() < 0) 
    if ((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this()))
      return(AddCommand(pObj, "UnGrab", this()));      
  // Tor ist offen: erledigt
  if (pGate->GetY() - GetY() < 0) return(0);
  // Clonk soll den Turm anfassen
  if (!((pObj->GetAction() S= "Push") && (pObj->GetActionTarget() == this())))
    return(AddCommand(pObj, "Grab", this()));
  // Tor bewegt sich: warten
  if (pGate->GetComDir() == COMD_Up()) return(1);
  // Tor öffnen
  return(ControlLeft(pObj));
}


