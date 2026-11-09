/*-- Fahrstuhl --*/

#strict 3

// verwendet Teile vom original fahrstuhl

#include CXEC
#include ELBS

local pCase;

protected func Initialize()
{
  pCase = CreateObject(ELE_,+0,+27,GetOwner());
  SetObjectOrder(this(), pCase);
  SetAction("LiftCase",pCase);
  return(1);
}

public func RemoveCase()
{
  // Komplett, aber kein Fahrstuhlkorb: reduzieren für Wiederaufbau/Reparatur
  if (Equal(GetCon(), 100)) { SetAction("Idle"); DoCon(-1); }
  return(1);
}

// Zum externen Aufruf von Szenarienscripten
public func CreateShaft(iLength)
{
  // SolidMask des ELE_ wegschieben
  var pCase, iCaseY;
  if (pCase = GetActionTarget())
  {
    iCaseY = GetY(pCase);
    SetPosition(GetX(pCase), (GetY() ?? 0) - 20, pCase);
  }
  // Schacht freigraben
  DigFreeRect((GetX() ?? 0)-12, (GetY() ?? 0)+25, 24, iLength);
  // ELE_ zurückschieben
  if(pCase) SetPosition(GetX(pCase), iCaseY, pCase);
  return(1);
}

public func ContextPermanentModeTurnOn(pObj) 
{
  [$TxtPermanentModeTurnOn$|Image=ELE_|Condition=IsNotInPermanentMode|Desc=$TxtPermanentModeDesc$]
  pCase->DoControlAuto(pObj);
}
 
public func ContextPermanentModeTurnOff(pObj) 
{
  [$TxtPermanentModeTurnOff$|Image=ELE_|Condition=IsInPermanentMode|Desc=$TxtPermanentModeDesc$]
  pCase->DoControlAuto(pObj);
}
 
public func IsInPermanentMode()
{
  if (!pCase) return(nil);
  return (pCase->IsInPermanentMode());
}
  
public func IsNotInPermanentMode()
{
  if (!pCase) return(nil);
  return (pCase->IsNotInPermanentMode());
}
  
protected func Incineration() { if(pCase) RemoveObject(pCase); }
protected func IncinerationEx() { if(pCase) RemoveObject(pCase); }
protected func Destruction() { if(pCase) RemoveObject(pCase); return(_inherited()); }
