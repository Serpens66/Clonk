/*-- Fahrstuhlkorb --*/

#strict

//#include ELEC
#appendto ELEC 
#appendto ELE_   //Metallfahrstuhl
#appendto BELC  //Transportfahrstuhl
#appendto ELC2 // Seilbahn

local isTrapped;
local AutoMode;       // Dauerbetrieb
local AutoMoveDir;    // Dauerbetrieb Richtung
local RangeTop;       // Obergrenze
local RangeBottom;    // Untergrenze
local timer;          // Timer

local oElev;          // Das Fahrstuhl-Objekt
  
private func Drilling()
{
  if (isTrapped) {LoosenCase(); return(1);}
 return(_inherited());
}

private func SelfMovement()
{
  if (isTrapped) {LoosenCase(); return(1);}
 return(_inherited());
}


/* Steuerung */

protected func ControlCommand(strCommand, oTarget, iTx, iTy)
{
  if (isTrapped) {LoosenCase();	return();}
 return(_inherited(strCommand, oTarget, iTx, iTy));
}

private func DoControlDown(pObj)
{  
  if (isTrapped) {LoosenCase();	return();}
 return(_inherited(pObj));
}

private func DoControlUp(pObj)
{
  if (isTrapped) {LoosenCase();return();}
 return(_inherited(pObj));
}

private func DoControlStop(pObj)
{
  if (isTrapped) {LoosenCase();return();}
 return(_inherited(pObj));
}

private func DoControlRight(pObj)
{
  if (isTrapped) {LoosenCase();return();}
 return(_inherited(pObj));
}

private func DoControlLeft(pObj)
{
  if (isTrapped) {LoosenCase();return();}
 return(_inherited(pObj));
}

private func DoControlDig(pObj)
{
  if (isTrapped) {LoosenCase();return();}
 return(_inherited(pObj));
}

public func DoControlAuto(pObj)
{
  if (isTrapped) {LoosenCase();return();}
 return(_inherited(pObj));
}  

public func LoosenCase() {
var dieid;
if(GetID()==ELEC) dieid=438Z;
if(GetID()==ELE_) dieid=538Z;
if(GetID()==BELC) dieid=638Z;
if(GetID()==ELC2) dieid=738Z;
  CreateObject(dieid,0,13);
  RemoveObject();
}

public func GetTrapped() {
  return isTrapped;
}

public func Trap() {
  isTrapped = true;
}

public func WireCut() {
  Trap();
}

func Elevator(int iNum)
{
return(_inherited(iNum));
}

func NoEnergy(bool fCheck)
{
return(_inherited(fCheck));
}

func Passenger()
{
return(_inherited());
}

func IsElevator()
{
return(_inherited());
}

func Halt()
{
if (isTrapped) {LoosenCase();return();}
return(_inherited());
}

func SetMoveTo(int iSaveX)
{
if (isTrapped) {LoosenCase();return();}
return(_inherited(iSaveX));
}

func GetAutoModeString()
{
return(_inherited());
}


public func IsCuttable() {return(true);}